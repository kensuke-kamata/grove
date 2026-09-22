#include <iostream>

int value = 10;

namespace demo {

int value = 20;

class settings {
public:
    static int value;
};

int settings::value = 30;

void print_values()
{
    int value = 40;

    ::std::cout << "local value     : " << value           << '\n';
    ::std::cout << "static value    : " << settings::value << '\n';
    ::std::cout << "namespace value : " << demo::value     << '\n';
    ::std::cout << "global value    : " << ::value         << '\n';
}

} // namespace demo

int main()
{
    demo::print_values();
    return 0;
}
