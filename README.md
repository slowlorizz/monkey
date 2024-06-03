# Monkey

 QOL Library for Arduino C++


## list.hpp

### Class `List<T>`

| method | return-value | description |
| --- | --- | --- |
| `len()` | `int` | returns length of list (how many items are stored) |
| `itm(int)` | `List<T>::Item*` | returns pointer to list item at given index |
| `itr([int=0])` | `List<T>::Iterator` | returns iterator at given index, default index: 0 |
| `clear()` | `List<T>&` | Clears list (removes all items) |
| `get(int)` | `T` | Returns value from given index |
| `set(int, T)` | `List<T>&` | sets value of item at index |
| `push(T)` | `List<T>&` | apppends value at end of list |
| `push(int, T*)` | `List<T>&` | appends an array at end of list, the int parameter is the length of the array |
| `push_front(T)` | `List<T>&` | pushes value infront of first value |
| `insert(int, T)` | `List<T>&` | Inserts value at given index, pushes al later elements back |
| `pop([int=-1])` | `T` | Removes Element at Index, returns the element value, default Index: -1 |
| `foreach(void (*func)(List<T>::Iterator*))` | `List<T>&` | Runs Function on with each element as parameter (provided as iterator) |
| `map(T (*func)(List<T>::Iterator*))` | `List<T>&` | Runs Function on with each element as parameter (provided as iterator) and sets the element value to the fundtions return value |
| `operator [] (int)` | `T&` | gets the value-reference at given index |
| `operator << (T \| List<T>&)` | `List<T>&` | pushes values |