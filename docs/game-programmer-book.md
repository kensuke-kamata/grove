# Notes from the Game Programmer Book

## Derived data and consistency

> ただ、計算で求まる情報を保存するやり方には、矛盾が発生しうるという欠点がある。検索して得られるプレイヤーの位置と、変数で保存されたプレイヤーの位置が違ってしまう可能性がゼロではないということだ。心配事が増えるくらいなら毎回探した方がいい、という考え方はあってもいい。速度が問題にならない限りは、生成できる情報は必要なときに生成するようにした方が無難だろう。

### Takeaway

When information can be derived from an authoritative source, storing a second copy creates a consistency risk. Prefer deriving it when needed unless the performance cost is significant.

### Keywords

- derived data
- source of truth
- consistency
- premature optimization

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
