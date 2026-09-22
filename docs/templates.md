# Template definitions

The compiler needs to see a template's definition when it instantiates a
specialization. A declaration alone is not enough.

```cpp
// math.h
template <typename T>
T add(T a, T b) { return a + b; }
```

```cpp
// main.cpp
#include "math.h"

int result = add(2, 3); // compiler generates add<int>
```

If the definition is placed in `math.cpp`, `main.cpp` sees only the declaration:

```cpp
// math.h
template <typename T>
T add(T a, T b);

// math.cpp
#include "math.h"

template <typename T>
T add(T a, T b) { return a + b; }

// main.cpp
#include "math.h"

int result = add(2, 3); // error: add<int> was not generated here
```

`math.cpp` does not generate `add<int>` because it never uses that specialization.
The linker then cannot find the function requested by `main.cpp`.

Each `.cpp` file is compiled separately. Ordinary functions can be compiled in
one `.cpp` and linked from another; templates are usually defined in headers so
the using `.cpp` can instantiate the required type.

---

If only specific types are supported, `math.cpp` can explicitly generate one:

```cpp
// math.h
template <typename T>
T add(T a, T b);

// math.cpp
#include "math.h"

template <typename T>
T add(T a, T b) { return a + b; }

template int add<int>(int, int); // Generate add<int> in this translation unit

// main.cpp
#include "math.h"

int result = add(2, 3);
```

Then `main.cpp` can call `add(2, 3)` through the declaration in `math.h`.
Other types, such as `double`, are not generated unless they are explicitly listed.
