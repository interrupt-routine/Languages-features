`const` in JavaScript and `final` in Java prevent reassignment of a variable :

```javascript
const x = 3;
x = 5; // Uncaught TypeError: Assignment to constant variable.
```

It does not protect against mutation of an object (including arrays):
```javascript
const array = [0, 1, 2];
array = [3, 4, 5]; // Error
array[0] = 666;
console.log(array); // [666, 1, 2]
```

One could infer from this that the `const/final` in these languages is of a different nature than the `const` of C/C++, which does prevent mutation:

```c
const int array[] =  {1, 2, 3};
array[0]= 666; // Error: assignment of read-only location ‘array[0]’
```

This is not actually the case.
Internally, an object in Java / Javascript is equivalent to the following C construct :

```c
typedef struct Object {
  int property1;
  int property2;
  int (*method1) (int, int);
  ...
 } Object;

Object *new_object (int property1, ...)
{
  Object *object = malloc(sizeof *object);
  object->property1 = property1;
  // ...
  return object;
}
 ```
 
 An object variable is in fact a *reference* to an object (a *pointer* to that object in C).
 Therefore, declaring the object to be `const` is equivalent to specifying a pointer as `const` in C:
 
 ```c
 Object *const object = new_object();
 object->property1 = 666; // valid: we are modyfing the object, not the pointer itself
 object = new_object();   // error: assignment of read-only variable ‘object’
 ```
 
 which is NOT the same as a pointer to a const object ! 
 
 ```c
 const Object *object = new_object();
 object->property1 = 666; // error: assignment of member ‘property1’ in read-only object
 object = new_object();   // valid: we are modifying the pointer, not what it points to
 ```
 
In particular, since objects are also passed by reference in Java/JavaScript, this means that we cannot prevent a function from modifying an object.
The `final` specifier for a parameter is not very useful in Java :

```java
public void mutateArray (final int array[]) {
  array[0] = 666;     // this works fine (sadly)
  array = new int[5]; // Error : The final local variable 'array' cannot be assigned.
}
```

C equivalent:
```c
void mutateArray (size_t array_length, int *const array)
{
  array[0] = 666;         // works
  array = new_array(...); // error: assignment of read-only parameter ‘array’
}
```
