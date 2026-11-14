<svg width="200" height="200" xmlns="http://w3.org">
  <defs>
    <clipPath id="circleView">
      <circle cx="100" cy="100" r="100" />
    </clipPath>
  </defs>
  <image width="200" height="200" href="./assets/smallCHIMERA.png" clip-path="url(#circleView)" />
</svg>

# CHIMERA
CAN Hardare Interface for Mechanical Evaluation and Real-time Analysis



## To build and run the project
```
mkdir build

cd build

cmake ..

make
```

## Style guide
**Inspired by NASA's Power of 10 Style Guide**

### Naming Conventions
Use snake_case for variables and functions

Use PascalCase for classes and structs

Preprocessor constants should be ALL_CAPS

### Formatting
```
void foo_bar() {
  baz = qux;
}
```

### Error Handling
Use enum class error codes to return errors
Example:
```
enum class [[nodiscard]] Status {
  Success,
  SensorFail
  BadParam,
  Foo,
  Bar
};

Status always_fails() {
  return Status::SensorFail;
}

```

Avoid all exceptions until processing can NOT continue without user input.

### Classes
Don't do any work in the constructor. Only initialize variables.

Refer to memebrs using 'this->'
```
this->member_variable = foo;
```
### Memory Managment
Do not use dynamic memory allocation after initialization.
Avoid using the heap if at all possible.

Never use more than one level of dereferencing for pointers. 

No function pointers allowed.

### Loops
Loops must have fixed bounds.

No recursion.

### Preprocessing
Use only for header file declarations and simple macros


### Compilation
Compile with GCC, all warnings enabled. Code MUST compile with no warnings.



