* `String.match()` returns `null` instead of `[]` when there are no matches. This triggers an error when chaining calls to `filter()`, `map()` and friends.
   Always use `(str.match() ?? [])` or equivalent.

* Calling a function with `undefined` triggers default function parameters:
  ```javascript
  function f (x = 5) {
    console.log(x);
  }
  f();          // 5
  f(3);         // 3
  f(undefined); // 5
  ```
