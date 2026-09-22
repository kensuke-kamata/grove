#include <iostream>

class number {
public:
    explicit number(int value)
        : value_(value)
    {
        std::cout << "  [ctor] number value : "
                  << value_ << '\n';
    }

    ~number() {
        std::cout << "  [dtor] number value : "
                  << value_ << '\n';
    }

private:
    int value_;
};

class trace {
public:
    trace(const char* name, int id, int& object_count, int number_value)
        // NOTE: these members must be initialized before the constructor body:
        //       - const data members
        //       - reference data members
        //       - data members whose type has no default constructor
        //       - base classes that require constructor arguments
        : name_(name)
        , id_(id)                     // const member
        , object_count_(object_count) // reference member
        , number_(number_value)       // no default constructor
    {
        ++object_count_;

        std::cout << "  [ctor] trace name   : "
                  << name_
                  << " (#" << id_ << ")\n";
    }

    ~trace() {
        std::cout << "  [dtor] trace name   : "
                  << name_
                  << " (#" << id_ << ")\n";

        --object_count_;
    }

private:
    const char* name_;
    const int id_;
    int& object_count_;
    number number_;
};

void function(int& object_count) {
    std::cout << "\n [function] entering\n\n";

    trace stack_object(
        "stack_object",
        1,
        object_count,
        42);

    trace* heap_object = new trace(
        "heap_object",
        2,
        object_count,
        43);

    std::cout << "\n [function] object count : "
              << object_count << "\n\n";

    delete heap_object;                  // heap  object's destructor runs here

    std::cout << "\n [function] leaving\n\n";
}                                        // stack object's destructor runs here

int main() {
    int object_count = 0;

    std::cout << "[main] object count : " << object_count << '\n';

    function(object_count);

    std::cout << "\n";
    std::cout << "[main] left function\n";
    std::cout << "[main] object count : " << object_count << '\n';

    return 0;
}
