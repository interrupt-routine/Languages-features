In JavaScript default parameters are generated at call time, in Python they are generated once and for all at function definition time:

```javascript
function arrayAdd (value, array = []) {
  array.push(value);
  return array;
}

arrayAdd(5); // [5]
arrayAdd(8); // [8]
```

```python
def lst_add (value, lst = []):
  lst.append(value)
  return lst

lst_add(5) # [5]
lst_add(8) # [5, 8]
```

a common pattern to circumvent this in Python is to use `None` as the default value and check whether the object is `None` in the function's body:

```python
def lst_add (value, lst = None):
  lst = lst or []
  lst.append(value)
  return lst

lst_add(5) # [5]
lst_add(8) # [8]
```
