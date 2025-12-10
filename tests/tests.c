#include "../fzac.h"

#include "Window_Creation\Window_Creation.h"

int
main(void)
{
  printf("We are not your kind!\n");

  window_create(NULL, S("My Cool Window"), 1280, 720, 20, 20);

  while (1);
  
  return 0;
}
