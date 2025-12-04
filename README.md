# TODO(fz):

- [ ] Update Hephaestus:
  - [ ] Remove logging
  - [ ] Use dynamic allocations instead of static
  - [ ] Remove fz_std (make it standalone)
  - [ ] Remove +1 and -1 variable splicing
  - [ ] BUG: If comments (//) are not the first thing in a template, it thinks its a hph file comment
  - [ ] Put generated files inside a 'generated' directory
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

# How to
`fzac.h` Includes the all the `base\` headers in the project. These are the 
bare minimum modules for the library which are to be included in each project.

# Structure
- `src\base\`: Core library.
- `src\modules\`: Opt-In modules provided by the library.
- `external\`: Opt-in external code imported into this library.
- `tests\`: For development, should not be included in projects. 

> `Opt-in` means that it has to be explicitly included by the user

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

----
# Ideas for project parser

### 
- Create dependency graph to make sure that the unity build actually has dependency tree and its not circular
- Generate HTML documentation.
- Capitalize documentation if not. Add period (.) at the end of documentation phrases if not there.
- List TODOs
- Mark parts of the code from: "// @Section: <Section_Name>" 
- Mark files as: "// @File: ". Contains documentation for this file
- Mark Modules as: "// @Module: ". Contains documentation for the module. Each module should have a "Main module file". If it requires multiple implementations (Such as OS stufF), switch the OS_TYPE in this main file and include it, so that the user only has to include the main file.
- Generate header files (So that we only need to worry about the implementation).