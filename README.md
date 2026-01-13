# TODO(fz):

```
x - done
w - work in progress
```

- [ ] Code_Gen Module
- [w] Introspection
- [ ] Error (Base.h)
- [ ] Logging (Base.h)
- [x] Custom entry point
- [x] Math
- [x] Console
- [x] String
- [x] File handling
- [x] Command line handling
- [x] Thread context
- [x] Memory: Arena
- [x] Launch window
  - [x] Allow multiple windows
  - [x] Opengl context

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
## Function signatures comment.
- Comment in front of it with /**/. Must be a block comment. 
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
