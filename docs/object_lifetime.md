# Object lifetime

[tests/object_lifetime.cpp](../tests/object_lifetime.cpp) demonstrates constructor
and destructor timing for stack and heap objects.

- A stack object's destructor runs when its scope ends.
- A heap object's destructor runs when `delete` is called.
- Member objects are constructed before the constructor body and destroyed after
  the destructor body, in reverse order.
- `const` members, reference members, members without a default constructor, and
  base classes requiring arguments are initialized in the member initializer list.
