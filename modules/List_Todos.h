#ifndef LIST_TODOS_H
#define LIST_TODOS_H

#define LT_MAX_FILES 64
#define LT_MAX_TODOS 256

typedef enum
{
  List_Todo_Kind_None = 0,
  List_Todo_Kind_TODO,
  List_Todo_Kind_BUG,
} List_Todo_Kind;

typedef struct LT_Todo LT_Todo;
struct LT_Todo
{
  List_Todo_Kind kind;
  String author;
  String comment;
  s32 line;
};

typedef struct LT_File LT_File;
struct LT_File
{
  String name;

  LT_Todo *todos;
  u32 todos_count;
  u32 todos_capacity;
};

typedef struct List_Todos List_Todos;
struct List_Todos
{
  Arena *arena;

  LT_File *files;
  u32 files_count;
  u32 files_capacity;  
};

function List_Todos list_todos(String src_directory);
function void print_list_todos(List_Todos *todos);

function List_Todos 
list_todos(String src_directory)
{
  Scratch scratch = scratch_begin(0,0);

  List_Todos todos;
  memory_zero_struct(&todos);
  
  todos.arena = arena_alloc();
  arena_array_init(todos.arena, todos.files, LT_File, LT_MAX_FILES);
  String_List files = file_get_files_in_path(todos.arena, src_directory, true);

  for (String_Node *next = files.first; next != NULL; next = next->next)
  {
    String file_being_lexed = next->value;
    if (!is_file(file_being_lexed)) continue;

    if (string_contains(file_being_lexed, S("\\Extern\\"))  ||
        string_contains(file_being_lexed, S("\\.git\\"))    ||
        string_contains(file_being_lexed, S("\\.svn\\"))    ||
        string_contains(file_being_lexed, S("\\.idea\\"))   ||
        string_contains(file_being_lexed, S("\\.vs\\"))     ||
        string_contains(file_being_lexed, S("\\.vscode\\")) ||
        string_contains(file_being_lexed, S("\\.code\\")))
    {
      continue;
    }

    String extension = file_get_extension(file_being_lexed);
    
    if (!string_equals(extension, S("c"), true) && !string_equals(extension, S("h"), true))
    {
      continue;
    }

    LT_File *lt_file;
    arena_array_push(lt_file, todos.files);
    memory_zero_struct(lt_file);
    
    lt_file->name = string_copy(todos.arena, file_being_lexed);
    arena_array_init(todos.arena, lt_file->todos, LT_Todo, LT_MAX_TODOS);

    Lexer lexer;
    lexer_init_with_single_file_path(&lexer, file_being_lexed, 0, Emit_Line_Comments|Emit_Block_Comments);

    for (;;)
    {
      Token *token = lexer_peek_token(&lexer);
      if (token->kind == Token_End_Of_File) break;
      
      if (token->kind == Token_Comment_Line || token->kind == Token_Comment_Block)
      {
        String comment = token->value;
        u64 index = 0;

        while (index < comment.count)
        {
          while (index < comment.count && comment.cstring[index] != '@')
          {
            index += 1;
          }
          if (index >= comment.count) break;
          index += 1;

          u64 kind_start = index;
          while (index < comment.count && comment.cstring[index] != '(' && comment.cstring[index] != '\n' && comment.cstring[index] != '\r')
          {
            index += 1;
          }
          if (index >= comment.count || comment.cstring[index] != '(') continue;

          String kind_string = {0};
          kind_string.cstring = comment.cstring + kind_start;
          kind_string.count   = index - kind_start;

          List_Todo_Kind kind = List_Todo_Kind_None;
          if      (string_equals(kind_string, S("TODO"), true)) kind = List_Todo_Kind_TODO;
          else if (string_equals(kind_string, S("BUG"),  true)) kind = List_Todo_Kind_BUG;
          if (kind == List_Todo_Kind_None) 
          {
            continue;
          }

          index += 1;

          u64 author_start = index;
          while (index < comment.count && comment.cstring[index] != ')' && comment.cstring[index] != '\n' && comment.cstring[index] != '\r')
          {
            index += 1;
          }
          if (index >= comment.count || comment.cstring[index] != ')') continue;

          String author_string = {0};
          author_string.cstring = comment.cstring + author_start;
          author_string.count   = index - author_start;
          index += 1;

          if (index >= comment.count || comment.cstring[index] != ':') continue;
          index += 1;

          if (index < comment.count && comment.cstring[index] == ' ')
          {
            index += 1;
          }

          u64 description_start = index;
          while (index < comment.count && comment.cstring[index] != '\n' && comment.cstring[index] != '\r')
          {
            index += 1;
          }

          String description_string = {0};
          description_string.cstring = comment.cstring + description_start;
          description_string.count   = index - description_start;

          assert(lt_file->todos_count < lt_file->todos_capacity && "todos capacity exceeded");
          LT_Todo *lt_todo = &lt_file->todos[lt_file->todos_count];
          lt_file->todos_count += 1;

          lt_todo->kind    = kind;
          lt_todo->author  = string_copy(todos.arena, author_string);
          lt_todo->comment = string_copy(todos.arena, description_string);
          lt_todo->line    = lexer.current_line_number;
        }

        lexer_eat_token(&lexer);
      }
      else
      {
        lexer_eat_token(&lexer);
      }
    }

  }

  scratch_end(&scratch);

  return todos;
}

function void
print_list_todos(List_Todos *todos)
{
  String_Builder builder = string_builder_init(kilobytes(64));
  s32 total_todos = 0;

  for (u32 i = 0; i < todos->files_count; i += 1)
  {
    LT_File *file = &todos->files[i];
    total_todos += file->todos_count;

    for (u32 t = 0; t < file->todos_count; t += 1)
    {
      LT_Todo *todo = &file->todos[t];

      string_builder_pushf(&builder, "(%s) ", todo->author.cstring);
      string_builder_pushf(&builder, "%s: %d - ", file->name.cstring, todo->line);

      switch (todo->kind)
      {
        case List_Todo_Kind_TODO: { string_builder_push(&builder, "TODO"); } break;
        case List_Todo_Kind_BUG:  { string_builder_push(&builder, "BUG"); } break;
        case List_Todo_Kind_None:
        default:
        {
          string_builder_push(&builder, "NONE/UNHANDLED");
        }
      }

      string_builder_push(&builder, " :: ");
      string_builder_push_string(&builder, todo->comment);
      string_builder_push(&builder, "\n");
    }
  }

  string_builder_pushf(&builder, "\n%d todos.", total_todos);

  Scratch scratch = scratch_begin(0,0);
  {
    String todos_string = string_builder_to_string(scratch.arena, &builder);
    string_print(todos_string);
  }
  scratch_end(&scratch);

  string_builder_free(&builder);
}

#endif // LIST_TODOS_H