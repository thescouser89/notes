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


For basic use, consider the concept _Regular_. A type is regular when it behaves
much like an int or a vector. An object of a regular type

- can be default constructed
- can be copied using a constructor or an assignment
- can be compared using `==` and `!=`.

## Function Objects
One particularly useful kind of template is the function object (sometimes
called a functor), which is used to define objects that can be like functions.

```cpp
template<typename T>
class Less_than {
    const T val; // value to compare againsto

    public:
    Less_than(const T& v): val(v) {}
    bool operator()(const T& x) const { return x < val; }
}
```

The function called `operator()` implements the 'function call'.

```cpp
Less_than<int> lti {42};
Less_than<string> lts {"Backus"};

void fct(int n, const string& s) {
    bool b1 = lti(n);
    bool b2 = lts(s);
}
```

Some function objects are widely used as arguments to algorithms. For example,
we can count the occurrences of values for which a predicate returns `true`:

```cpp
template<typename C, typename P>
int count(const C& c, P pred) {
    int cnt = 0;
    for (const auto& x: c) {
        if (pred(x)) {
            ++cnt;
        }
    }
    return cnt;
}
```

A predicate is something that we can invoke to return true or false.

There is a notation for implicitly generating function objects:

```cpp
[&](int a){ return a < x; }
```

This is called a lambda expression. It generates a function object exactly like
`Less_than<int>{x}`. The `[&]` is a capture list specifying that local names
used (such as x) will be accessed through references.

If we wanted to capture only x, we could have said so: `[&x]`. Had we wanted to
give the generated object a copy of x, we could have said so: `[=x]`. Capture
nothing is `[]`, capture all local names used by value is [=], and all local
names used by reference is `[&]`.

## Variadic Templates
A template can be defined to accept an arbitrary number of arguments of
arbitrary types. Such a template is called a variadic template.

```cpp
template<typename T, typename... Tail>
void f(T head, Tail... tail) {
    g(head);
    f(tail...);
}

f(0.2, 'c', "yuck", 0, 1, 2);
```

The key to implementing a variadic template is to note that when you pass a list
of arguments to it, you can separate the first argument from the rest. The
ellipsis, ..., is used to indicate the rest of a list.

The strength of variadic templates is that they can accept any arguments you
care to give them. The weakness is that they type checking of the interface is a
possibly elaborate template program.

## Aliases
Introduce a synonym for a type or a template:

```cpp
using size_t = unsigned int;
```

It is very common for a parameterized type to provide an alias for types related
to their template arguments. For example:

```cpp
template<typename T>
class Vector {
    public:
    using value_type = T;
}


/// another example
template<typename Value>
using String_map = Map<string, Value>;
String_map<int> m; // m is a Map<string, int>
```
