# Notes from the Game Programmer Book

## Derived data and consistency

> ただ、計算で求まる情報を保存するやり方には、矛盾が発生しうるという欠点がある。検索して得られるプレイヤーの位置と、変数で保存されたプレイヤーの位置が違ってしまう可能性がゼロではないということだ。心配事が増えるくらいなら毎回探した方がいい、という考え方はあってもいい。速度が問題にならない限りは、生成できる情報は必要なときに生成するようにした方が無難だろう。

When information can be derived from an authoritative source, storing a second copy creates a consistency risk. Prefer deriving it when needed unless the performance cost is significant.

## Make parameter and return intent visible

Use references for required objects. `const` shows that an argument is read-only;
a mutable reference allows the function to modify it. Use a pointer when the
object may be absent:

```cpp
void draw(const Shape& shape);       // required input; read-only
void calculate(Result& output);      // required output; modified
void update(State* optionalState);   // may modify; nullptr is allowed
void save(const Data* optionalData); // read-only; nullptr is allowed
```

Pointers here are non-owning: they do not transfer ownership or extend the
object's lifetime. Do not keep using a pointer or reference after its object has
been destroyed.

For a class-type member accessor, return a pointer (to const for read-only
access) to make use explicit and avoid accidental copies:

```cpp
class World {
public:
    const Entity* player() const { return &player_; }

private:
    Entity player_;
};

const Entity* player = world.player(); // pointer; no Entity copy
```

The pointer is non-owning and valid only while the member is alive. Return
`nullptr` only if the member may be absent. A `const Entity&` return also avoids
a copy unless the caller stores it by value, such as with `const auto entity`.

## Forward declarations and include dependencies

A declaration introduces a name; a definition provides the complete type. For
example, `class B;` is a forward declaration: it tells the compiler that `B`
exists, but not its size or members. `class B { /* ... */ };` is the class
definition.

If `A` contains a `B` object by value, the compiler needs the complete
definition of `B` to determine the size and layout of `A`, so `A.h` must include
`B.h`:

```cpp
// A.h
#include "B.h"

class A {
    B b_; // B is stored inside A
};
```

If `A` only refers to a separate `B` object through a pointer, the pointer's
size is known without knowing `B`'s definition. Forward-declare `B` in `A.h`,
then include `B.h` in `A.cpp` wherever `B`'s members are used:

```cpp
// A.h
class B;

class A {
public:
    void useB();

private:
    B* b_; // non-owning pointer; its lifetime is managed elsewhere
};

// A.cpp
#include "A.h"
#include "B.h"

void A::useB() {
    b_->act(); // B's definition is needed here to use its members
}
```

Function declarations can also mention an incomplete class type, including as
a by-value parameter or return type:

```cpp
class B;

class A {
public:
    B makeB();
    void consumeB(B value);
    void observeB(const B& value);
};
```

These are declarations only: they describe function signatures and do not
create or pass any `B` objects, so they do not need `B`'s size. The definitions
of functions taking or returning `B` by value, and call sites that pass or
receive `B` by value, do need the complete type. In contrast, a function may be
declared and called with a pointer or reference to incomplete `B`; its
definition can omit `B.h` unless it accesses `B`'s members or otherwise
requires the complete type.

Prefer this form when the relationship is genuinely a reference to an
independently managed or optional `B`; it keeps `B.h` out of every file that
includes `A.h`. Keep a value member when `B` is an always-present part of `A`.
A raw pointer does not own `B`, so its lifetime must outlast its use. If `A`
owns a `B` via `std::unique_ptr<B>`, `A` can still forward-declare `B`. With
the default deleter, destroying the `unique_ptr` performs `delete` on `B`, so
the complete `B` type is required there. Declare `A`'s destructor in `A.h` and
define it in `A.cpp` after including `B.h`, so the destructor is compiled where
`B` is complete:

```cpp
// A.h
#include <memory>

class B;

class A {
public:
    A();
    ~A();

private:
    std::unique_ptr<B> b_;
};

// A.cpp
#include "A.h"
#include "B.h"

A::A() : b_(std::make_unique<B>()) {}
A::~A() = default;
```

The same completeness requirement applies to other operations that may delete
the owned object, such as `reset()` or move assignment. The constructor is also
defined in `A.cpp` here because `std::make_unique<B>()` creates a `B` and needs
its definition.

Textual includes make the compiler preprocess and parse included headers and
their transitive includes for each translation unit, so unnecessary includes
can add substantial repeated work. Whether headers account for most of a
project's compile time depends on the project; measure it rather than assuming
it. Clang documents this include cost in [its modules overview](https://clang.llvm.org/docs/Modules.html)
and supports compile-time profiling with [`-ftime-trace`](https://clang.llvm.org/docs/UsersManual.html#time-trace).
