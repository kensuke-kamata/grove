# Fixed-size flag sets

`grove::flagset<flag, size>` stores a fixed number of flags in a
`std::bitset<size>`. Use it when the available flags are known at compile time
and many objects need compact state.

## Enum values are bit positions

Each enum value is an index into the bitset, not a bit mask.

```cpp
#include <cstddef>

enum class actor_flag : std::size_t {
    grounded = 0,
    stunned = 1,
    invisible = 2,
};

using actor_flags = grove::flagset<actor_flag, 3>;
```

Value `0` refers to bit 0, and value `1` refers to bit 1. The enum does not
need a `count` enumerator: specify the capacity once in the `flagset` type. If
an enum uses index 127, it needs at least 128 bits.

```cpp
enum class player_flag : std::size_t {
    god_mode = 0,
    show_hitbox = 1,
    show_navigation = 127,
};

using player_flags = grove::flagset<player_flag, 128>;
```

The enum's underlying type does not allocate storage for each enumerator.
Runtime flag state is stored in the `std::bitset` member. A 128-bit bitset
represents 128 flags in 16 bytes of logical storage. The actual value of
`sizeof(std::bitset<128>)` is implementation-dependent and may include padding.
On common platforms, this is much smaller than `bool[128]`.

`std::bitset<N>` has a compile-time fixed capacity. Its size cannot be chosen
with a runtime constructor argument. This implementation uses
`flagset<enum, size>` and does not allocate a dynamic bit vector.

## Invalid values and failure behavior

The enum's numeric value is used as a bit position. Valid positions are in
`[0, size)`. Passing an out-of-range value to `set`, `reset`, or a test function
throws `std::out_of_range`.

Operations that accept multiple values validate every index before changing or
checking the bitset. If a list contains an out-of-range value, `set` and
`reset` throw before changing any bits. `test_all` and `test_any` also validate
the full list before short-circuiting their result.

Use `try_set` to set flags without throwing. It returns `false` for an
out-of-range value and leaves the state unchanged. Its initializer-list
overload also validates every value before making changes.

```cpp
if (!flags.try_set({player_flag::god_mode, player_flag::show_hitbox})) {
    // a value was outside the flagset capacity
}
```

Range checking does not determine whether a value is a declared enumerator. If
an enum has gaps, an undeclared value inside `[0, size)` is accepted. C++20 has
no standard feature for enumerating declared enum values. To allow only named
values, provide an explicit list of valid values or a validation function for
each enum. Validate external values, such as save data, at the input boundary
before converting them to the enum.

## Why use an initializer list instead of `|`?

In this design, enum values are bit positions, so combining two values with
`|` does not have the usual bit-mask meaning. A custom `operator|` can produce
a mask, but a generic enum operator is affected by C++ operator lookup and may
fail when enums are declared in other namespaces. An initializer list works
regardless of the enum's namespace:

```cpp
flags.set({player_flag::god_mode, player_flag::show_hitbox});
```

## Related C++ concepts

- `static_assert` checks a condition at compile time. It is not a function and
  does not run when the program executes.
- In a template parameter list, `class` and `typename` both introduce a type
  parameter. `typename` also has another use for identifying dependent type
  names.
- `requires` is a C++20 feature for specifying when a template can be used.
- `friend` gives a non-member function access to private members. A friend
  operator defined inside a class is called a hidden friend; it is generally
  found by argument-dependent lookup when an operand has that class type.

## Run the flagset test

Run these commands from the repository root:

```sh
cmake --build build --target flagset
ctest --test-dir build -R '^flagset$' --output-on-failure
```
