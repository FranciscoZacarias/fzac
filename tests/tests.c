#include "../fzac.h"

#include "Window.h"
#include "Input.h"

function void
entry_point(Command_Line* command_line)
{
  console_attach();
  window_create(NULL, S("My Cool Window"), 1280, 720, 20, 20);

  printf("yahoo");



  b32 alive = true;
  while (alive)
  {
    alive = update_window_events();

    
  }
}
