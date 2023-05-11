#include "product.hpp"

#include <algorithm>
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

std::pair<uint8_t, uint8_t> add(uint8_t a, uint8_t b, uint8_t carry)
{
    uint8_t res = a + b + carry;
    return {res % 10, res / 10};
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
    std::copy(val.cend() - n, val.cend(), std::back_inserter(b));

    return {a, b};
}

bigint power_ten(bigint const& val, uint8_t power)
{
    if (val.empty()) {
        return val;
    }

    auto res = val;
    res.reserve(res.size() + power);

    for (uint8_t i = 0; i < power; ++i) {
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

    auto first_term = power_ten(multiply(a, c), 2 * n);
    auto second_term = power_ten(add(multiply(a, d), multiply(b, c)), n);
    auto third_term = multiply(b, d);

    auto res = add(first_term, second_term);
    res = add(res, third_term);
    return res;
}

bigint add(bigint const & lhs, bigint const & rhs)
{
    auto as = lhs;
    auto bs = rhs;

    // `bs` should be longer or the same as `as`
    if (as.size() > bs.size())
    {
        std::swap(as, bs);
    }

    bigint res{};
    res.reserve(bs.size() + 1); // the res cannot be any longer than bs.size() + 1

    uint8_t carry = 0;

    auto b = bs.crbegin();
    for (auto a = as.crbegin(); a < as.crend(); ++a, ++b)
    {
        auto [r, new_carry] = add(*a, *b, carry);
        res.push_back(r);
        carry = new_carry;
    }

    for (; b < bs.crend(); ++b)
    {
        auto [r, new_carry] = add(0, *b, carry);
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
