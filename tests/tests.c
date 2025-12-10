#include "../fzac.h"

#include "Window_Creation\Window_Creation.h"

function void
entry_point(Command_Line* command_line)
{
  printf("We are not your kind!\n");

  printf("--- %s", command_line->executable.cstring);

  window_create(NULL, S("My Cool Window"), 1280, 720, 20, 20);

  //while (1);
}
