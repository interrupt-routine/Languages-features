
* Some constructors can be called without `new`:
  ```javascript
  const array = Array(20);
  // equivalent to
  const array = new Array(20);
  ```
  How it works :

  If a JavaScript function is called without `new`, `this` points to the global object (or `undefined` in strict mode).
  A constructor can thus check how it was called:
  ```javascript
  function Array (...args) {
    if (this === globalThis || this === undefined)
      return new Array(...args);
    this.foo = bar;
    // ...
  }
  ```


