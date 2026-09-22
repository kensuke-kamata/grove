# Scope resolution

The `::` operator selects a name from a specific scope.

```cpp
name          // Search from the current scope.
demo::name    // Select name from the demo namespace.
::name        // Select name from the global namespace.
```

It is also used for static members and for defining members outside a class or
namespace body.

```cpp
settings::value
int demo::settings::value = 30;
```

The experiment in [`tests/scope_resolution.cpp`](../tests/scope_resolution.cpp)
prints values from local, namespace, global, and static-member scopes.
