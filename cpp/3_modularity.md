The key to managing all the C++ components is to clearly define the interactions
among those parts. The first and most important step is to distinguish between
the interface to a part and its implementation.

```cpp
class Vector {
    public:
    Vector(int s);
    double& operator[](int  i);
    int size();

    private:
    double* elem;
    int s2;
};
```
The key point is that the funtion definitions are 'elsewhere'.

```cpp
Vector::Vector(int s): elem{new double[s]}, s2{s} {}

double& Vector::operator[](int i) {
    return elem[i];
}

int Vector::size() {
    return s2;
}
```

## Separate compilation
Typically we place the declarations that specify the interface to a module in a
file with a name indicating its intended use.

```cpp
// Vector.h

class Vector {
// blablabla
};
```

Users will include that file to access that interface:

```cpp
// user.cpp

#include "Vector.h"
```

To help the compiler ensure consistency, the `.cpp` file providing the
implementation of `Vector` will also include the `.h` file providing its
interface:

```cpp
// Vector.cpp

#include "Vector.h"

Vector::Vector(int s): elem{new double[s]}, s2{s} {}
// rest of it here
```

THe code in `user.cpp` and `Vector.cpp` shares the `Vector` interface
information provided in `Vector.h`, but the 2 files can be separately compiled.


## Namespaces
C++ offers namespaces as a mechanism for expressing that some declarations
belong together and that their names shouldn't clash with other names.

```cpp
namespace My_code {
    class complex {

    };
    int main();
};

int main() {
    return My_code::main();
}
```

By putting my code into the namespace `My_code`, I make sure that my names do
not conflict with the standard-library names in namespace `std`.

To gain access to names in the standard library namespace, we can use a
`using`-directive:

```cpp
using namespace std;
```

A `using` directive makes names from the named namespace accessible as if they
were local to the scope in which we placed the directive.

Namespaces are primarily used to organize larger program components, such as
libraries.

## Error Handling
THe `Vector::operator[]()` can detect an attempted out-of-range access and throw
an `out_of_range` exception.

```cpp
double& Vector::operator[](int i) {
    if (i < 0 || size() <= i) {
        throw out_of_range {"Vector::operator[]"};
    }
    return elem[i];
}
```

THe implementation will unwind the function call stack to get back to the
context of that caller.

```cpp
try {
    v[v.size()] = 7;
} catch (out_of_range) {}
```

The `out_of_range` type is defined in the standard library (in <stdexcept>).

A function that should never throw an exception can be declared `noexcept`.

### Invariants
40
