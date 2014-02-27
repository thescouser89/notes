C++ augments the built-in types and operations with a set of abstraction
mechanisms. They are designed to allow programmers to use their own types. These
types are called user-defined types. They are referred as classes and
enumerations.

## Structures
Organize the elements it needs into a data structure, a _struct_:

```cpp
struct Vector {
    int s2;
    double* elem;
}
```

A variable of type `Vector` can be defined like this:

```cpp
Vector v;
```

However, the `elem` pointer does not point to anything.

```cpp
void vector_init(Vector& v, int s) {
    v.elem = new double[s];
    v.s2 = s;
}
```

The `new` operator allocates memory from an area called the heap and 'live'
until they are destroyed using the `delete` operator.

```cpp
double read_and_sum(int s) {
    Vector v;
    vector_init(v, s);
    for (int i = 0; i != s; ++i) {
        cin >> v.elem[i];
    }
    double sum = 0;
    for (int i =0; i != s; ++i) {
        sum += v.elem[i];
    }
    return sum;
}
```

## Classes
A tighter connection between the representation and the operations is needed for
a user-defined type to have all the properties expected of a 'real type'.

A class is defined to have a set of members, which can be data, function, or
type members. The interface is defined by the public members of a class, and
private members are accessible only through that interface.

```cpp
class Vector {
    public:
    Vector(int s): elem{new double[s]}, s2{s} {} // construct a vector
    double& operator[](int i) { return elem[i]; }
    int size() { return s2; }

    private:
    double* elem;
    int s2;
}

Vector v(6); // a vector with 6 elements
```

A `Vector` object is a 'handle' containing a pointer to the elements, plus the
number of elements (s2).

A 'function' with the same name as its clas is called a _constructor_.

The constructor `Vector(int)` initializes the `Vector` members using a member
initializer list:

```cpp
:elem{new double[s]}, s2{s}
```

Access to elements is provided by a subscript function, called `operator[]`. It
returns a reference to the appropriate element.

There is no fundamental difference between a `struct` and a `class`: a struct is
simply a class with members public by default.

## Unions
A union is a struct in which all members are allocated at the same address so
that the `union` occupies only as much space as its largest member.

```cpp
enum Type {str, num};

struct Entry {
    char* name;
    Type t;
    char* s;
    int i;
}

void f(Entry* p) {
    if (p->t == str) {
        count << p->s;
    }
}

union Value {
    char* s;
    int i
}
```

The language does not keep track of which kind of value is held by a union, so
the programmer must do that.

## Enumerations
C++ supports a simple form of user-defined type for which we can enumerate the
values:

```cpp
enum class Color {red, blue, green};
enum class Traffic_light {green, yellow, red};

Color col = Color::red;
Traffic_light light = Traffic_light::red;
```

Note that enumerators are in the scope of their `enum` class, `Color::red` is
different from `Traffic_light::red`.

Enumerations are used to represent small sets of integer values.

The `class` after the `enum` specifies that an enumeration is strongly typed and
that its enumerators are scoped. `enum classes` help prevent accidental missues
of constants. We cannot mix `Traffic_light` and `Color` values.

```cpp
int i = Color::red; // error
```

By default, an `enum class` has only assignment, initialization, and
comparisons defined. However, an enumeration is a user-defined type so we can
define operators with it:

```cpp
Traffic_light& operator++(Traffic_light& t) {
    switch(t) {
        case Traffic_light::green: return t = Traffic_light::yellow;
        case Traffic_light::yellow: return t = Traffic_light::red;
        case Traffic_light::red: return t = Traffic_light::green;
    }
}

Traffic_light next = ++light;
```

If you want enumerator values to be ints, you can remove the class from enum
class to get a 'plain' enum.

The enumerators from a 'plain' enum are entered into the same scope as the name
of their `enum` and implicitly converts to their integer value.

Prefer `class enums` over 'plain' enums to minimize surprises.
