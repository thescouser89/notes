These are the language facilities supporting the programming styles known as
_object-oriented_ programming and _generic_ programming.

A class is a user-defined type provided to represent a concept in the code of a
program.

## Concrete Types
Basic idea of concrete classes is that they behave just like built-in types. The
defining characteristic of a concrete type is that its representation is part of
its definition.

```cpp
class complex {
    double re, im;

    public:
    complex(double r, double i): re{r}, im{i} {}
    complex(double r): re{r}, im{0} {}
    complex(): re{0}, im{0} {} // default complex

    double real() const { return re; }
    double imag() const { return im; }
    void real(double d) { re = d; }
    void imag(double d) { im = d; }

    complex& operator+=(complex z) { re += x.re, im += x.im; return *this; }
    complex& operator-=(complex z) { re -= x.re, im -= x.im; return *this; }

    complex& operator*=(complex) // defined out-of-class somewhere
    complex& operator/=(complex) // defined out-of-class somewhere
}
```

To make `complex` efficient, this implies that simple operations must be
inlined. That is, simple operations (such as `+=` and `imag()`) must be
implemented without function calls in the generated machine code.

It is possible to explicitly require inlining by preceeding a function
declaration with the keyword `inline`.

A constructor that can be invoked without an argument is called a _default_
constructor. By defining a default constructor you eliminate the possibility of
uninitialized variables of that type.

The `const` specifiers on the functions returning the real and imag parts
indicate that these functions do not modify the object for which they are
called.

Many usefule operations do not require direct access to the representation of
`complex`, so they are defined separately from the class definition:

```cpp
complex operator+(complex a, complex b) { return a += b; }
complex operator-(complex a, complex b) { return a -= b; }
complex operator-(complex a) { return {-a.real(), -a.imag()}; } // unary minus
complex operator*(complex a, complex b) { return a *= b; }
complex operator/(complex a, complex b) { return a /= b; }

bool operator==(complex a, complex b) {
    return a.real() == b.real() && a.imag() == b.imag();
}

bool operator!=(complex a, complex b) {
    return !(a == b);
}
```

The argument passed by value is copied, so one can modify an argument without
affecting the caller's copy, and use the result as the return value.

```cpp
complex a {2.3};
complex b {1 / a};

if ( a != b ) {}
```

The compiler converts operators involving `complex` numbers into appropriate
function calls.

For example, `c != b` means `operator!=(c, b)`

User-defined operators ("overloaded operators") should be used cautiously and
conventionally.

### A container
A container is an object holding a colleciton of elements.

We need a mechanism to ensure that the memory allocated by the constructor is
deallocated; that mechanism is a _destructor_:

```cpp
class Vector {
    // lots of functions omitted...
    Vector(int s): elem {new double[s]}, sz{s} {}
    ~Vector() { delete[] elem; }
}
```

The name of a destructor is the complement operator `~` followed by the name of
the class. Vector's constructor allocates some memory on the heap using the
`new` operator. The destructor cleans up by freeing that memory using the
`delete` operator.

```cpp
void fct(int n) {
    Vector v(n);
} // v is destroyed here
```

The constructor allocates the elements and initializes the `Vector` members
appropriately. This handle-to-data model is very commonly used to manage data
that can vary in size during the lifetime of an object. The technique of
acquiring resources in a constructor and releasing them in a destructor, known
as _Resource Acquisition is Initialization_, or _RAII_, allows us to eliminate
'naked new operations', i.e, avoid allocations in general code and keep them
buried inside the implementation of well-behaved abstractions.

Avoiding naked new and naked delete makes code far less error-prone and far
easier to keep free of resource leaks.

### Initializing containers
Two ways:

- Initializer-list constructor: Initialize with a list of elements
- `push_back()`: ad a new element at hte back of the sequence

```cpp
class Vector {
    public:
    Vector(std::initializer_list<double>); // initialize with a list of doubles
    void push_back(double);
}
```
The way to provide Vector with a move constructor, so that returning a
potentially huge amount of data from read() is cheap, is explained later.

The `std::initializer_list` used to define the initializer-list constructro is a
standard-library type known to the compiler: when we use a {}-list, such as {1,
2}, the compiler will create an object of type `initializer_list` to give to the
program.
