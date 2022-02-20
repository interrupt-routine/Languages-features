
C and C++ do not have the same definition of a compile time constant.
In particular, this is important to specify the length of an array.

In standard C only `#define`d values, integer literals, and `enum` members, can be used;
in C++ something like `const int size = 64;` is also considered a compile time constant

```c
// global scope (aka file scope)

enum {SIZE_1 = 64};
#define SIZE_2 64
const int SIZE_3 = 64;

int array0[64];     // valid for both C and C++
int array1[SIZE_1]; // valid for both C and C++
int array2[SIZE_2]; // valid for both C and C++
int array3[SIZE_3]; // valid for C++, error in C : SIZE_3 is not a compile time constant

int main (void)
{
  ...
}
```

However, `C99` onwards also optionally supports `VLA`s (Variable Length Arrays)
(a bit of a misnomer, their length is fixed once set).
Their length is initialized by a variable and is therefore decided at runtime, rather than compile time
(this includes `sizeof` expressions).

VLAs are allocated on the stack (and therefore there can be no VLAs at file-scope / global scope, because
global variables are stored in the `.data` or `.rodata` segment rather than on the stack).
If a C compiler does not support VLAs, it has to `#define __STDC_NO_VLA__ 1`.
In particular, `GCC` and `clang` support them, but `MSVC` does not.
VLAs are not int C++, but some compilers support them as an extension.

```c
  const int SIZE_1 = 64;

  int array[SIZE_1]; // OK in C++, error in C : VLA at file scope

  void function (int size_2)
  {
    int array[SIZE_1]; // normal array in C++, VLA in C
    int array[size_2]; // invalid in C++, VLA in C
  }
```

If the length initializer of a VLA is <= `0`, the behavior is undefined.
