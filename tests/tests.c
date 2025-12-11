#include "../fzac.h"

#include "Window.h"
#include "Input.h"

function void
entry_point(Command_Line* command_line)
{
  console_attach();

  u64* numbers = array(u64, &AllocatorStdlib);
  array_print(numbers);
  array_add(numbers, 4);
  array_print(numbers);
  array_add(numbers, 32);
  array_print(numbers);
  array_add(numbers, 90);
  array_print(numbers);
  array_add(numbers, 100);
  array_print(numbers);
  array_add(numbers, 101);
  array_print(numbers);
  array_add(numbers, 102);
  array_print(numbers);
  array_add(numbers, 103);
  array_print(numbers);
  array_add(numbers, 104);
  array_print(numbers);
  array_add(numbers, 105);
  array_print(numbers);

  printf("\n\n");

  for (u64 i = 0; i < array_size(numbers); i += 1)
  {
    printf("numbers[%lld]: %lld\n", i, numbers[i]);
  }

  b32 alive = true;
  while (alive)
  {
    alive = update_window_events();

    
  }
}
