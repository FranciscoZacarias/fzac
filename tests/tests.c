#include "../fzac.h"

#include "Platform.h"
#include "Window.h"
#include "Input.h"

function void
entry_point(Command_Line* command_line)
{
  Arena* arena = arena_alloc();
  console_attach();

  String f = S("C:/Users/francisco.zacarias/Documents/Francisco/fzac/new_file.x");
  file_create(f);
  if (file_exists(f)) printf("Correct\n"); else printf("Incorrect\n");

  String d = S("some data written\nandsomemore");
  file_write(f, d.cstring, d.size);

  file_delete(f);
  if (!file_exists(f)) printf("Correct\n"); else printf("Incorrect\n");

  Path path = path_from_string(arena, S("C:\\Users\\francisco.zacarias\\Documents\\Francisco\\fzac"));
  String s = string_from_path(arena, path);

  printf("Path: "S_FMT"", S_ARG(s));

  b32 alive = true;
  while (alive)
  {
    alive = update_window_events();
  }
}
