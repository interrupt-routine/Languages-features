In C, calling a variadic macro with 0 variadic arguments can be problematic.

The following macro `eprintf()` emulates `printf()` with `stderr` instead of `stdout` as the output stream:

```c
#define eprintf(format, ...) fprintf(stderr, format, __VA_ARGS__)
```
It can be called like so:
```c
eprintf("size must be > 0, but got %d", size);
/* expands to : */
fprintf(stderr, "size must be > 0, but got %d", size);
```

What happens in case of the following call with no variadic arguments ?
```c
eprintf("some error occurred");
```
It is expanded like so:
```c
fprintf(stderr, "some error occurred", ); // <-- trailing comma !
```
The problem here is the trailing comma followed by no arguments, which produces a syntax error.
There are two ways to circumvent the problem:

* `C++20` introduced the new token `__VA_OPT__(content)` . It expands to `content` if `__VA_ARGS__` is not empty.
* 
  Thus we can redefine our `eprintf()` macro as follows;
  ```c
  #define eprintf(format, ...) fprintf(stderr, format __VA_OPT__(,) __VA_ARGS__)
  ```
  
  Unfortunately, `__VA_OPT__` has not made it into standard C, not even `C23`.
  Compilers that implement it for C++20 may make it available for C as well
  (true for `gcc` and `MSVC`, but seemlingly not for `clang`).
  
  Support for `__VA_OPT__` can be detected at compile-time :
  ```c
  // variadic macro that expands to its third argument
  #define THIRD_ARG(a, b, c, ...) c
  // variadic macro
  // if __VA_OPT__ is not supported, it expands to itself i.e.THIRD_ARG(__VA_OPT__(,), 1, 0,)
  // and thus the 3rd argument is 0 (falsy)
  // if __VA_OPT__ is supported, it expands to a comma i.e. THIRD_ARG(, , 1, 0), because
  // __VA_ARGS__ is not empty (the 4th argument is 0)
  // thus the 3rd argument is 1 (truthy)
  #define TEST_VA_OPT_SUPPORT(...) THIRD_ARG(__VA_OPT__(arg), 1, 0)
  #define VA_OPT_SUPPORTED TEST_VA_OPT_SUPPORT(?)

  #if VA_OPT_SUPPORTED
    // ... do stuff
  ```
* Some compilers (`gcc`, `clang`, `MSVC`) offer an extension that allows the token-pasting operator `##`
  to appear after a comma and before `__VA_ARGS__`, in which case the `##`
  does nothing when the variable arguments are present, but removes
  the comma when the variable arguments are not present.
  
  Thus we can redefine our `eprintf()` macro as follows;
  ```c
  #define eprintf(format, ...) fprintf(stderr, format, ## __VA_ARGS__)
  ```
  Support for this extension can be detected at compile-time:
  ```c
  // variadic macro that expands to its third argument
  #define THIRD_ARG(a, b, c, ...) c
  // variadic macro
  // if the extension is supported, it expands to THIRD_ARG(placeholder, 0, 1)
  // thus the 3rd argument is 0 (falsy)
  // if the extension is not supported, it expands to THIRD_ARG(placeholder, , 0, 1)
  // thus the 3rd argument is 1 (truthy)
  #define TEST_VA_ARGS_EXTENSION_SUPPORT(...) THIRD_ARG(placeholder, ##__VA_ARGS__, 0, 1)
  // calling the previous macro with no arguments :
  #define VA_ARGS_EXTENSION_SUPPORTED TEST_VA_ARGS_EXTENSION_SUPPORT()

  #if VA_ARGS_EXTENSION_SUPPORTED
    // do stuff
  ```
