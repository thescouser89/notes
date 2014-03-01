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

```dpp
Vector v1 = {1, 2, 3, 4, 5};

Vector::Vector(std::initializer_list<double> lst)
    : elem{ new double[lst.size()]}, sz{static_cast<int>(lst.size())} {
        copy(lst.begin(), lst.end(), elem); // copy from lst into elem
}

```
`static_cast` is used to convert the size of the initializer list into an int.
This is done because there is a tiny chance that the number of elements in the
list is larger than the largest integer.

## Abstract Types
An abstract type is a type that completely insulates a user from implementation
details. To do that, we decouple the interface from the representation.

Since we don't know anything abou thte representation of an abstract type, we
must allocate objects on the free store, and access them through references or
pointers.

```cpp
class Container {
    public:
    virtual double& operator[](int) = 0;
    virtual int size() const = 0;
    virtual ~Container() {}
};
```
The word `virtual` means may be redefined later in a class derived from this
one. A class derived from Container provides an implementation for the Container
interface.

The `= 0` syntax says the function is pure _virtual_; i.e some class derived
from Container must define the function.

A class with a pure virtual function is called an abstract class.

```cpp
void use(Container& c) {
    const int sz = c.size();

    for (int i = 0; i != sz; ++i) {
        cout << c[i] << '\n';
    }
}
```
```cpp
class Vector_container: public Container {
    Vector v;

    public:
    Vector_container(int s): v(s) {}
    ~Vector_container() {}

    double& operator[](int i) { return v[i]; }
    int size() const { return v.size(); }
};
```

The `:public` can be read as 'is derived from' or 'is a subtype of'. Class
Vector_container is said to be derived from class Container, and class Container
is said to be a base of class Vector_container.

To achieve the virtual properties, a Container object must contain information
to allow it to select the right function to call at runtime.

The usual implementation technique is for the compiler to convert the name of a
virtual function into an index into a table of pointers to functions.
That table is usually called the virtual function table (`vtbl`).

The functions in the `vtbl` allow the object to be used correctly even when the
size of the object and the layout of its data are unknown to the caller.

The virtual call mechanism can be made almost as efficient as the normal
function call mechanism.

## Class Hierarchies
The container example is a very simple example of a class hierarchy. We use
class hierarchies to represent concepts that have hierarchical relationships.

Shape -> Circle
 |
 |
Triangle

```cpp
class Shape {
    public:
    virtual Point center() const = 0;
    virtual void move(Point to) = 0;

    virtual void draw() const = 0;
    virtual void rotate(int angle) = 0;

    virtual ~Shape() {}
};

class Circle: public Shape {
    public:
    Circle(Point p, int rr);
    Point center() const { return x; }
    void move(Point to) { x = to; }
    void draw() const;
    void rotate(int) {}

    private:
    Point x;
    int r;
};

class Smiley: public Circle {
    public:
    Smiley(Point p, int r): Circle {p, r}, mouth{nullptr} {}

    ~Smiley() {
        delete mouth;
        for (auto p: eyes) delete p;
    }
    void move(Point to);
    void draw() const;
    void rotate(int);

    void add_eye(Shape* s) { eyes.push_back(s); }
    void set_mouth(Shape* s);
    virtual void wink(int i);

    private:
    vector<Shape*> eyes;
    Shape* mouth;
};
```

### Explicit Overriding
A programmer can explicitly state that a function is meant to override.

```cpp
boid move(Point to) override;
```

We can ask 'is this Shape a kind of Smiley?' using the `dynamic_cast` operator:

```cpp
if (Smiley* p = dynamic_cast<Smiley*>(ps)) {}
```

If the object pointed by the argument of `dynamic_cast` is not of the expected
type, `dynamic_cast` returns `nullptr`.

### Avoiding Resource Leaks
Functions returning a pointer to an object allocated on the free store are
dangerous. One solution is to return a standard-library `unique_ptr` rather than
a 'naked pointer' and store `unique_ptrs` in the container:

```cpp
unique_ptr<Shape> read_shape(istream &is) {
    //
    return unique_ptr<Shape> {new Circle {p, r}};
}
```
Now the object is owned by `unique_ptr` which will `delete` the object when it
is no longer needed, i.e when its `unique_ptr` goes out of scope.

## Copy and Move
By default, objects are copied.

```cpp
void test(complex z1) {
    complex z2 {z1}; // copy initialization
    complex z3;
    z3 = z2; // copy assignment
}
```

z1, z2, and z3 have the same value because both the assignments and the
initialization copied both members.

For simple concrete types, memberwise copy is often exactly the right semantics
for copy. For some sophisticated concrete types, like Vector, memberwise copy is
not the right semantics, and for abstract types it almost never is.

### Copying containers
```cpp
void bad_copy(Vector v1) {
    Vector v2 = v1;
    v1[0] = 2;
    v2[1] = 3;
}
```

v1 and v2 points to the same underlying part.

Copying of an object of a class is defined by two members: a copy constructor
and a copy assignment.

```cpp
class Vector {
    //
    Vector(const Vector& a); // copy constructor
    Vector& operator=(const Vector& a); // copy assignment
}
```

A suitable definition of a copy constructor for Vector allocates the space for
the required number of elements and then copies the elements into it, so that
after a copy each Vector has its own copy of the elements.

The name `this` is predefined in a member function and points to the object for
which the member function is called.

### Moving Containers
We can control copying by defining a copy constructor and a copy assignment, but
copying can be costly for large containers. We avoid the cost of copying when we
pass objects to a function by using references, but we can't return a reference
to a local object as the result. (the local object could be destroyed by the
time the caller got a chance to look at it)

```cpp
class Vector {
    Vector& operator=(Vector&& a); // move assignment
}

Vector:: Vector(Vector&& a): elem {a.elem}, sz{z.sz} {
    a.elem = nullptr;
    a.sz = 0;
}
```

After a move, a moved-from object should be in a state that allows a destructor
to be run.

```cpp
y = std::move(x); // explicit move
return y; // implicit move
```

### Essential Operations
[not really covered]
