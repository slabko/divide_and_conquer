#include "product.hpp"

#include <algorithm>
#include <cassert>
#include <execution>
#include <stdexcept>

bigint bigint_from_string(std::string const & str)
{
    bigint res(str.size());

    const bool check = std::all_of(std::execution::unseq, str.cbegin(), str.cend(), [](auto n) { return n >= 48 && n <= 57; });
    if (!check)
    {
        throw std::invalid_argument("nonint character");
    }

    std::transform(std::execution::unseq, str.cbegin(), str.cend(), res.begin(), [](auto c) { return c - 48; });

    return res;
}

std::string string_from_bigint(bigint const & n)
{
    const bool check = std::all_of(std::execution::unseq, n.cbegin(), n.cend(), [](auto n) { return n >= 0 && n <= 9; });
    if (!check)
    {
        throw std::invalid_argument("non-char integer");
    }

    std::string res(n.size(), '\0');
    std::transform(std::execution::unseq, n.cbegin(), n.cend(), res.begin(), [](auto n) { return n + 48; });
    return res;
}

inline bool not_zero(uint8_t x)
{
    return x != 0;
}

std::pair<uint8_t, uint8_t> add(uint8_t a, uint8_t b, uint8_t carry)
{
    uint8_t res = a + b + carry;
    return {res % 10, res / 10};
}

std::pair<uint8_t, uint8_t> subtract(uint8_t a, uint8_t b, uint8_t carry)
{
    b += carry;
    if (a >= b)
    {
        return {a - b, 0};
    }

    return {10 + a - b, 1};
}


bigint add(bigint const & lhs, bigint const & rhs)
{
    auto as = lhs;
    auto bs = rhs;

    // `as` should be longer or the same as `bs`
    if (as.size() < bs.size())
    {
        std::swap(as, bs);
    }

    bigint res{};
    res.reserve(as.size() + 1); // the res cannot be any longer than bs.size() + 1

    uint8_t carry = 0;

    auto a = as.crbegin();
    for (auto b = bs.crbegin(); b < bs.crend(); ++a, ++b)
    {
        auto [r, new_carry] = add(*a, *b, carry);
        res.push_back(r);
        carry = new_carry;
    }

    for (; a < as.crend(); ++a)
    {
        auto [r, new_carry] = add(*a, 0, carry);
        res.push_back(r);
        carry = new_carry;
    }

    if (carry)
    {
        res.push_back(carry);
    }

    std::reverse(res.begin(), res.end());
    return res;
}

bigint subtract(bigint const & as, bigint const & bs)
{
    assert(as.size() >= bs.size());

    bigint res{};
    res.reserve(as.size());

    uint8_t carry = 0;

    auto a = as.crbegin();
    for (auto b = bs.crbegin(); b < bs.crend(); ++b, ++a)
    {
        auto [r, new_carry] = subtract(*a, *b, carry);
        res.push_back(r);
        carry = new_carry;
    }

    for (; a < as.crend(); ++a)
    {
        auto [r, new_carry] = subtract(*a, 0, carry);
        res.push_back(r);
        carry = new_carry;
    }

    assert(carry == 0);

    auto trailing_zeros_end = std::find_if(res.crbegin(), res.crend(), not_zero);
    res.erase(trailing_zeros_end.base(), res.end());
    std::reverse(res.begin(), res.end());

    return res;
}

std::pair<bigint, bigint> split(bigint const & val, size_t n)
{
    if (val.empty())
    {
        return {bigint{}, bigint{}};
    }

    if (val.size() <= n)
    {
        return {bigint{}, val};
    }

    bigint a{};
    a.reserve(val.size() - n);
    bigint b{};
    b.reserve(n);

    std::copy(val.cbegin(), val.cend() - n, std::back_inserter(a));

    auto leading_zeros_end = std::find_if(val.cend() - n, val.cend(), not_zero);
    std::copy(leading_zeros_end, val.cend(), std::back_inserter(b));

    return {a, b};
}

bigint power_ten(bigint const & val, uint8_t power)
{
    if (val.empty())
    {
        return val;
    }

    auto res = val;
    res.reserve(res.size() + power);

    for (uint8_t i = 0; i < power; ++i)
    {
        res.push_back(0);
    }

    return res;
}

bigint multiply(bigint const & lhs, bigint const & rhs)
{
    if (lhs.empty() || rhs.empty())
    {
        return {};
    }

    if (lhs.size() <= 1 && rhs.size() <= 1)
    {
        uint8_t a_number = lhs.size() == 1 ? lhs.front() : 0;
        uint8_t b_number = rhs.size() == 1 ? rhs.front() : 0;
        uint8_t res = a_number * b_number;
        if (res < 10)
        {
            return {res};
        }
        return {static_cast<uint8_t>(res / 10), static_cast<uint8_t>(res % 10)};
    }

    size_t len = std::max(lhs.size(), rhs.size());
    size_t n = len - (len / 2);
    auto [a, b] = split(lhs, n);
    auto [c, d] = split(rhs, n);

    auto ac = multiply(a, c);
    auto bd = multiply(b, d);
    auto ad_plus_bc = multiply(add(a, b), add(c, d));
    ad_plus_bc = subtract(ad_plus_bc, ac);
    ad_plus_bc = subtract(ad_plus_bc, bd);

    auto res = power_ten(ac, 2 * n);
    res = add(res, power_ten(ad_plus_bc, n));
    res = add(res, bd);

    return res;
}
