#pragma once

#include <cstdint>
#include <iostream>

// tag for integer constructor (scales argument)
struct IntegerTag {};

// tag for prescaled argument
struct PrescaledTag {};

template<int B>
struct Fix {
    Fix() = default;

    explicit Fix(const int32_t other, PrescaledTag) {
        val = other;
    }

    explicit Fix(const int32_t other, IntegerTag t={}) {
        val = other<<B;
    }

    explicit Fix(const float other) {
        val = other * (1<<B);
    }

    // get as float (simpler to write than a static_cast)
    float toFloat() const {
        return val / static_cast<float>(1<<B);
    }

    // return integer part without rounding
    int32_t toInt() const {
        return val >> B;
    }

    static consteval int32_t fractionalBits() {
        return B;
    }

    int32_t val;
};

template<int B>
inline Fix<B> operator+(const Fix<B> lhs, const Fix<B> rhs) {
    return Fix<B>(lhs.val + rhs.val);
}

template<int B>
inline Fix<B> operator-(const Fix<B> lhs, const Fix<B> rhs) {
    return Fix<B>(lhs.val - rhs.val);
}

template<int B>
inline Fix<B> operator*(const Fix<B> lhs, const Fix<B> rhs) {
    return Fix<B>((lhs.val * rhs.val) >> B);
}

template<int B>
inline Fix<B> operator/(const Fix<B> lhs, const Fix<B> rhs) {
    return Fix<B>{ (lhs.val << B) / rhs.val };
}

template<int B>
inline bool operator<(const Fix<B> lhs, const Fix<B> rhs) {
    return lhs.val < rhs.val;
}

template<int B>
inline bool operator>(const Fix<B> lhs, const Fix<B> rhs) {
    return lhs.val > rhs.val;
}

// better as member?
template<int B>
inline Fix<B>& operator+=(Fix<B>& rhs, const Fix<B> lhs) {
    rhs.val += lhs.val;
    return rhs;
}


template<int B>
inline Fix<B> ceil(const Fix<B> fix) {
    constexpr int32_t mask = (1 << B) - 1;
    return Fix<B>{(fix.val + mask) & ~mask};
}


template<int B>
inline Fix<B> floor(const Fix<B> fix) {
    constexpr int32_t mask = (1 << B) - 1;
    return Fix<B>{fix.val & mask};
}

template<int B>
std::ostream& operator<<(std::ostream& os, const Fix<B> fix) {
    os << fix.toFloat();
    return os;
}

// Fixed point type with 10 fractional bits, ~1/1000
// resolution. If integer part remains within 10 bits
// during all arithmetic operations then division
// and multiplication doesn't loose precision.
// Referred in the code as 10.10 (with 20.10
// intermediate values during / and *)
using Fix10 = Fix<10>;
