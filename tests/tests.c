#include "../fzac.h"

#include "Window.h"
#include "Input.h"

function void
entry_point(Command_Line* command_line)
{
  console_attach();

  b32 alive = true;
  while (alive)
  {
    alive = update_window_events();

  }
}
