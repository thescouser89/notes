We can generalize our vector-of-doubles type to a vector-of-anything type by
making it a `template` and replacing the specific type `double` with a
parameter.

```cpp
template<typename T>
class Vector {
    private:
    T* elem;

    public:
    T& operator[](int i);
    Vector(int s);
    // ...
}
```

The `template<typename T>` prefix makes `T` a parameter of the declaration if
prefixes.

```cpp
template<typename T>
Vector<T>::Vector(int s) {}
```

To support the range-for loop for our Vector, we must define suitable begin()
and end() functions.

```cpp
template<typename T>
T* begin(Vector<T>& x) {
    return x.size() ? &x[0]: nullptr;
}

template<typename T>
T* end(Vector<T>& x) {
    return begin(x) + x.size(); // pointer to one past last element
}
```

Templates are a compile-time mechanism, so their use incurs no run-time overhead
compared to hand-crafted code.

In addition to type arguments, a template can take value arguments.

```cpp
template<typename T, int N>
struct Buffer {
    using value_type = T;
    constexpr int size() { return N; }
    T[N];
};
```

THe alias (value_type) and the constexpr function are provided to allow users
read-only access to the template arguments.

E.g, Buffer allows us to create arbitrarily sized buffers with no overheads from
the use of free store.

```cpp
Buffer<char, 1024> glob;
void fct() {
    Buffer<int, 10> buf; // local buffer of integers on the stack
}
```

## Function Templates
Templates are also used for parameterization of both types and algorithms in the
standard library.

```cpp
template<typename Container, typename Value>
Value sum(const Container& c, Value v) {
    for (auto x: c) {
        v += x;
    }
    return v;
}
```
The Value template argument and the function argument v are there to allow the
caller to specify the type and initial value of the accumulator.

The types of the template arguments for `sum<T, V>` are deduced from the
function arguments.

The most common use of templates is to support generic programming.
73
