#include <cstddef>
#include <iostream>
#include <stdexcept>

#include "grove/flagset.h"

enum class test_flag_a : std::size_t {
    a = 0,
    b,
    c,
    high_a = 63,
    high_b,
    high_c = 127,
};

enum class test_flag_b : std::size_t {
    a = 0,
    b,
    c,
};

using test_flags_a = grove::flagset<test_flag_a, 128>;
using test_flags_b = grove::flagset<test_flag_b, 3>;

int main() {
    const auto expect = [](bool condition, const char* message) {
        if (!condition) {
            std::cerr << "flagset test failed: " << message << '\n';
            return false;
        }
        return true;
    };

    // default construction starts with no flags set.
    {
        test_flags_a flags;

        if (!expect(flags.none() && !flags.any(), "default state is empty")) {
            return 1;
        }
        if (!expect(!flags.test(test_flag_a::a), "unset flag tests false")) {
            return 2;
        }
    }

    // construction and setting accept multiple flags directly.
    {
        test_flags_a flags{test_flag_a::a, test_flag_a::b};
        flags.set({test_flag_a::c, test_flag_a::high_a});

        if (!expect(flags.test(test_flag_a::a), "constructor sets a flag")) {
            return 3;
        }
        if (!expect(flags.test_all({test_flag_a::b, test_flag_a::c}),
                    "set initializer list sets every flag")) {
            return 4;
        }
    }

    // test_all requires every requested flag; test_any requires at least one.
    {
        test_flags_a flags{test_flag_a::a};

        if (!expect(!flags.test_all({test_flag_a::a, test_flag_a::high_a}),
                    "test_all fails when a requested flag is unset")) {
            return 5;
        }
        if (!expect(flags.test_any({test_flag_a::high_a, test_flag_a::a}),
                    "test_any checks later values after an unset flag")) {
            return 6;
        }
        if (!expect(!flags.test_any({test_flag_a::b, test_flag_a::high_a}),
                    "test_any is false when no requested flags are set")) {
            return 7;
        }
        if (!expect(flags.test_all({}) && !flags.test_any({}),
                    "empty tests follow all and any semantics")) {
            return 8;
        }
    }

    // set with a boolean mirrors the requested state.
    {
        test_flags_a flags;
        flags.set(test_flag_a::a, true);

        if (!expect(flags.test(test_flag_a::a), "set(flag, true) sets the flag")) {
            return 9;
        }
        flags.set(test_flag_a::a, false);
        if (!expect(!flags.test(test_flag_a::a), "set(flag, false) resets the flag")) {
            return 10;
        }
    }

    // repeated set and reset operations are idempotent.
    {
        test_flags_a flags;
        flags.set(test_flag_a::a);
        flags.set(test_flag_a::a);
        flags.reset(test_flag_a::b);
        flags.reset(test_flag_a::b);

        if (!expect(flags.test(test_flag_a::a) && !flags.test(test_flag_a::b),
                    "repeated set and reset preserve the expected state")) {
            return 11;
        }
    }

    // empty set and reset lists leave the current state unchanged.
    {
        test_flags_a flags{test_flag_a::a};
        flags.set({});
        flags.reset({});

        if (!expect(flags.test(test_flag_a::a) && flags.any(),
                    "empty mutation lists do not change state")) {
            return 12;
        }
    }

    // high indices demonstrate support beyond a single 64-bit word.
    {
        test_flags_a flags;
        flags.set({test_flag_a::high_b, test_flag_a::high_c});

        if (!expect(flags.test_all({test_flag_a::high_b, test_flag_a::high_c}),
                    "flags above index 63 can be set and tested")) {
            return 13;
        }
    }

    // resetting multiple flags leaves unrelated flags unchanged.
    {
        test_flags_a flags{test_flag_a::a, test_flag_a::b, test_flag_a::c};
        flags.reset({test_flag_a::b, test_flag_a::c});

        if (!expect(!flags.test(test_flag_a::b) && !flags.test(test_flag_a::c),
                    "reset clears all requested flags")) {
            return 14;
        }
        if (!expect(flags.test(test_flag_a::a), "reset preserves unrelated flags")) {
            return 15;
        }
    }

    // clear removes every set flag.
    {
        test_flags_a flags{test_flag_a::a, test_flag_a::high_c};
        flags.clear();

        if (!expect(flags.none() && !flags.any(), "clear resets every bit")) {
            return 16;
        }
    }

    // different enum types have independent flagset types.
    {
        test_flags_b flags{test_flag_b::a, test_flag_b::c};

        if (!expect(flags.test_all({test_flag_b::a, test_flag_b::c}),
                    "a second enum type has its own flagset")) {
            return 17;
        }
    }

    // invalid indices throw before a multi-flag mutation changes any bits.
    {
        constexpr auto invalid_flag = static_cast<test_flag_a>(128);
        test_flags_a flags{test_flag_a::c};
        bool threw = false;

        try {
            flags.set({test_flag_a::a, invalid_flag});
        } catch (const std::out_of_range&) {
            threw = true;
        }

        if (!expect(threw, "set throws for an out-of-range flag")) {
            return 18;
        }
        if (!expect(!flags.test(test_flag_a::a) && flags.test(test_flag_a::c),
                    "failed set leaves the state unchanged")) {
            return 19;
        }
    }

    // invalid reset input also leaves the entire state unchanged.
    {
        constexpr auto invalid_flag = static_cast<test_flag_a>(128);
        test_flags_a flags{test_flag_a::a, test_flag_a::b};
        bool threw = false;

        try {
            flags.reset({test_flag_a::a, invalid_flag});
        } catch (const std::out_of_range&) {
            threw = true;
        }

        if (!expect(threw, "reset throws for an out-of-range flag")) {
            return 20;
        }
        if (!expect(flags.test_all({test_flag_a::a, test_flag_a::b}),
                    "failed reset leaves the state unchanged")) {
            return 21;
        }
    }

    // try_set reports invalid input without throwing or partially changing state.
    {
        constexpr auto invalid_flag = static_cast<test_flag_a>(128);
        test_flags_a flags{test_flag_a::c};

        if (!expect(!flags.try_set(invalid_flag), "try_set rejects an invalid flag")) {
            return 22;
        }
        if (!expect(!flags.try_set({test_flag_a::a, invalid_flag}),
                    "try_set rejects an invalid list")) {
            return 23;
        }
        if (!expect(flags.test(test_flag_a::c) && !flags.test(test_flag_a::a),
                    "failed try_set leaves the state unchanged")) {
            return 24;
        }
        if (!expect(flags.try_set({test_flag_a::a, test_flag_a::b}) &&
                    flags.test_all({test_flag_a::a, test_flag_a::b}),
                    "try_set applies a valid list")) {
            return 25;
        }
    }

    std::cout << "flagset checks passed\n";
    return 0;
}
