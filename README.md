# TODO(fz):

- [ ] Update Hephaestus:
  - [ ] Remove logging
  - [ ] Use dynamic allocations instead of static
  - [ ] Remove fz_std (make it standalone)
  - [ ] Remove +1 and -1 variable splicing
  - [ ] BUG: If comments (//) are not the first thing in a template, it thinks its a hph file comment
- [ ] Error
- [ ] Logging
- [ ] Custom entry point
- [ ] Console
- [ ] String
- [ ] File handling
- [ ] Command line handling
- [ ] Thread context
- [ ] Memory: Arena
- [ ] Data structure: Stack SLL 
- [ ] Data structure: Queue SLL 
- [ ] Data structure: Stack DLL
- [ ] Data structure: Queue DLL
- [ ] Data structure: Array
- [ ] Launch window
  - [ ] Allow multiple windows
  - [ ] Graphics context

# Coding standards

# Unity build

# Global variables
- Add 'global' before it. 
- PascalCase 
- global u32 ThisIsMyGlobalVariable;

# Functions:
- Add 'function' before it
- snake_case
- function void* this_is_my_function();

# Structs
- Forward declare it first 'typedef struct * *'. Then just 'struct * {}'
- Underscore separated PascalCase (Pascal_Case)
- typedef struct My_Example_Struct My_Example_Struct;
  struct My_Example_Struct
  {
    ...
  };

# Macros
- Depending on the type of macro:
  . PascalCase - constants (#define OS_WINDOWS 1)
  . snake_case - function 'like' macro (#define memory_copy(...)) or attributes (#define local_persist static)


# Comments
## Function signatures comment
- Comment in front of it with /**/. 
- function void my_function() /* My function that does something */

## Struct comments
- Comment in front of the forward declare with /**/.
- Comment an attribute with /**/ in front of it.
- typedef struct My_Struct My_Struct; /* My struct that does something */
  struct My_Struct
  {
    u32 a; /* Comment on attribute A */
    u32 b; /* Comment on attribute B */
  };