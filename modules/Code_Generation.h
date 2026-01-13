#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include "Introspection.h"

typedef struct Code_Gen_Function Code_Gen_Function; /* For generated functions */
struct Code_Gen_Function
{
  String declaration;
  String implementation;
};

function Code_Gen_Function generate_enum_to_string(Arena* arena, Enum_Object* enum_object); /* Returns a string with a valid implementation of an enum_to_string from the given enum */
function String generate_enum_typedef(Arena* arena, Enum_Object* enum_object);

function String
generate_enum_typedef(Arena* arena, Enum_Object* enum_object)
{
  String result = Sf(arena, "typedef "S_FMT" "S_FMT";\n", S_ARG(enum_object->type), S_ARG(enum_object->identifier));
  return result;
}

function Code_Gen_Function
generate_enum_to_string(Arena* arena, Enum_Object* enum_object)
{
  Code_Gen_Function result;
  memory_zero_struct(&result);
  if (enum_object->members_count == 0) return result;

  Scratch scratch = scratch_begin(0,0);

  String cases = S("");
  for (u32 i = 0; i < enum_object->members_count; i += 1)
  {
    Enum_Member* member = &enum_object->members[i];
    String formatted = Sf(scratch.arena, "    case "S_FMT": return \""S_FMT"\";\n", S_ARG(member->identifier), S_ARG(member->identifier));
    cases = string_join(scratch.arena, cases, formatted );
  }

  scratch_end(&scratch);

  // @NOTE(fz): We don't do default fallback because, as long as the system has no bugs, it expects all cases to be covered. If anything, the user just has to run the metaprogram again
  result.declaration =   Sf(arena, "function const char* "S_FMT"_to_string("S_FMT" e); /* @GENERATED: To string function for enum "S_FMT" */\n", S_ARG(string_to_lower(scratch.arena, enum_object->identifier)), S_ARG(enum_object->identifier), S_ARG(enum_object->identifier));
  result.implementation = Sf(arena,
    "function const char*\n"
    ""S_FMT"_to_string("S_FMT" e)\n"
    "{\n"
    "  switch(e)\n"
    "  {\n"
    ""S_FMT""
    "  }\n"
    "  return \"\";\n"
    "}\n", S_ARG(string_to_lower(scratch.arena, enum_object->identifier)), S_ARG(enum_object->identifier), S_ARG(cases));

  return result;
}

#endif // CODE_GENERATION_H