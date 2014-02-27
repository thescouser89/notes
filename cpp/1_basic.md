## Introduction
C++ source code is processed by a compiler to produce object files. Then these
object files are later combined together by a linker to produce an executable
program.

Every C++ program must have exactly one global function named `main()`.

```cpp
#include <iostream>
int main() {
    std::cout << "Hello, World!\n";
}
```

The `std::` specifies that the name `cout` is to be found in the
standard-library namespace.

To make names from `std` visible without `std::`, add this:

```cpp
using namespace std;
```

C++ supports method overloading.

## Types, Variables and Arithmetic
A declaration is a statement that introduces a name into the program.

C++ offers a variety of notations for expressing initialization, such as the `=`
used above, and a universal form based on curly-brace-delimited initializer
lists:

```cpp
double d = 2.3;
double d2 {2.3};

complex<double> z = 1;
complex<double> z2 {d, d2};

/** the = is optional with {...} */
/** really, the = is redundant */
complex<double> x3 = {1, 2};

vector<int> v {1, 2, 3, 4, 5, 6};
```

The general {}-list form saves you from conversions that lose information.
```cpp
int i2 {7.2}; /** this will fail */
```

A constant cannot be left uninitialized and a variable should only be left
uninitialized in extremely rare circumstances.

Don't introduce a name until you have a suitable value for it.

### Using auto
When defining a variable, you don't need t state its type explicitly when it can
be deduced from the initializer:

```cpp
auto b = true; // bool
auto i = 123; // int
```

With `auto`, we use the `=` because there is no potentially troublesome type
conversion involved.

## Scope and Lifetime
A declaration introduces its name into a scope:

Local scope
:: A name declared in a function or lambda is called a local name

Class scope
:: A name is called a member/class name if it is defined in a class, outside any
function, lambda, or `enum class`. Its scope extends from the opening { of its
enclosing declaration to the end of that declaration.

Namespace scope
:: If a name is defined in a namespace, outside any function, lambda, class, or
`enum class`.

A name not declared inside any other construct is called a global name and is
said to be in the global namespace.

An object created by `new` lives until destroyed by `delete`.

# Constants
C++ supports two notions of immutability:

`const`
:: promise not to change this value. Data can be passed to functions without
   fear of it being modified.

`constexpr`
:: meaning roughly "to be evaluated at compile time".

```cpp
const int dmv = 17;
int var = 17;
constexpr double max1 = 1.4 * square(dmv); // ok if square(17) is a const expression
constexpr double max2 = 1.4 * square(var); // not ok since var is not constant
const double max3 = 1.4 * square(var);     // ok, may be evaluated at runtime
```

For a function to be usable in a constant expression, it must be defined with
`constexpr`.

```cpp
constexpr double square(double x) {
    return x * x;
}
```
To be `constexpr`, a function must be rather simple.

## Pointers, arrays and references
```cpp
#include <iostream>

int main(void) {
    int v[] {0, 1, 2, 3, 4, 5, 6, 7, 8};
    /* for each loop */
    for (auto i: v) {
        std::cout << i << "\n";
    }
    return 0;
}

void increment() {
    int v[] {0, 1, 2, 3, 4, 5, 6, 7, 8};
    for (auto& x: v) ++x;
    // v will become {1, 2, 3, 4, 5...}
}
```

In a declaration, the unary suffix `&` means 'reference to'. A reference is
similar to a pointer, except that you don't need to use a prefix `*` to access
the value referred to by the reference. References are particularly useful for
specifying function arguments. By using a reference, we ensure that for a call,
we do not copy the structure.

When we don't want to modify an argument, but still don't want the cost of
copying, we use a `const` reference.

e.g
```cpp
double sum(const vector<double>&)
```

When used in declarations, operators (such as &, \*, and []) are called
declarator operators.

When we don't have an object to point, we give the pointer the value `nullptr`.

```cpp
// check if a pointer points to something or notations
if (p == nullptr) return 0;
```

In older code, 0, or NULL is typically used instead of `nullptr`. However, the
`nullptr` eliminates potential confusion between integers and pointers.

```cpp
char answer = 0;
cin >> answer;

if (answer == 'y') {
    return true;
}
```
