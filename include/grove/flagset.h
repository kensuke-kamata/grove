#pragma once

#include <bitset>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>

namespace grove {

template <class flag, std::size_t size>
class flagset {
    static_assert(std::is_enum_v<flag>, "flagset requires an enum type");

public:
    flagset() = default;

    flagset(std::initializer_list<flag> flags) {
        set(flags);
    }

    void set(flag f) {
        set(f, true);
    }

    void set(flag f, bool enabled) {
        validate(f);
        bits_.set(to_index(f), enabled);
    }

    void set(std::initializer_list<flag> flags) {
        validate(flags);
        for (flag f : flags) {
            bits_.set(to_index(f));
        }
    }

    [[nodiscard]]
    bool try_set(flag f) noexcept {
        if (!is_valid(f)) {
            return false;
        }
        bits_.set(to_index(f));
        return true;
    }

    [[nodiscard]]
    bool try_set(std::initializer_list<flag> flags) noexcept {
        for (flag f : flags) {
            if (!is_valid(f)) {
                return false;
            }
        }
        for (flag f : flags) {
            bits_.set(to_index(f));
        }
        return true;
    }

    void reset(flag f) {
        validate(f);
        bits_.reset(to_index(f));
    }

    void reset(std::initializer_list<flag> flags) {
        validate(flags);
        for (flag f : flags) {
            bits_.reset(to_index(f));
        }
    }

    void clear() {
        bits_.reset();
    }

    [[nodiscard]]
    bool test(flag f) const {
        validate(f);
        return bits_.test(to_index(f));
    }

    [[nodiscard]]
    bool test_all(std::initializer_list<flag> flags) const {
        validate(flags);
        for (flag f : flags) {
            if (!bits_.test(to_index(f))) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]]
    bool test_any(std::initializer_list<flag> flags) const {
        validate(flags);
        for (flag f : flags) {
            if (bits_.test(to_index(f))) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]]
    bool any() const {
        return bits_.any();
    }

    [[nodiscard]]
    bool none() const {
        return bits_.none();
    }

private:
    static constexpr std::size_t to_index(flag f) noexcept {
        return static_cast<std::size_t>(f);
    }

    static constexpr bool is_valid(flag f) noexcept {
        return to_index(f) < size;
    }

    static void validate(flag f) {
        if (!is_valid(f)) {
            throw std::out_of_range("flag index is outside flagset size");
        }
    }

    static void validate(std::initializer_list<flag> flags) {
        for (flag f : flags) {
            validate(f);
        }
    }

    std::bitset<size> bits_{};
};

} // namespace grove
