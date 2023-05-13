#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <catch2/catch_test_macros.hpp>

#include "merge_sort.hpp"
#include "product.hpp"

int first_digit(int x)
{
    if (x == 0)
        return 0;

    int digits = floor(log10(abs(x))) + 1;
    return x / std::pow(10, digits - 1);
}

TEST_CASE("String to bigint")
{
    REQUIRE(bigint_from_string("1234567890") == bigint{1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
    REQUIRE(bigint_from_string("").empty());

    REQUIRE_THROWS_AS(bigint_from_string("k"), std::invalid_argument);
}

TEST_CASE("Bigint to string")
{
    REQUIRE(string_from_bigint({1, 2, 3, 4, 5, 6, 7, 8, 9, 0}) == "1234567890");
    REQUIRE(string_from_bigint({}).empty());

    REQUIRE_THROWS_AS(string_from_bigint({10}), std::invalid_argument);
}

bigint add(std::string const & a, std::string const & b)
{
    return add(bigint_from_string(a), bigint_from_string(b));
}

bigint subtract(std::string const & a, std::string const & b)
{
    return subtract(bigint_from_string(a), bigint_from_string(b));
}

bigint multiply(std::string const & a, std::string const & b)
{
    return multiply(bigint_from_string(a), bigint_from_string(b));
}

TEST_CASE("Test add")
{
    REQUIRE(add("16", "28") == bigint{4, 4});
    REQUIRE(add("999", "1") == bigint{1, 0, 0, 0});
}

TEST_CASE("Test subtract")
{
    REQUIRE(subtract("1", "1").empty());
    REQUIRE(subtract("2", "1") == bigint{1});
    REQUIRE(subtract("10", "1") == bigint{9});
    REQUIRE(subtract("100", "1") == bigint{9, 9});
    REQUIRE(subtract("99", "90") == bigint{9});
}

TEST_CASE("Test multiply")
{
    REQUIRE(multiply("1", "1") == bigint{1});
    REQUIRE(multiply("7", "4") == bigint{2, 8});
    REQUIRE(multiply("", "9").empty());

    REQUIRE(multiply("128", "4") == bigint{5, 1, 2});
    REQUIRE(multiply("732", "459") == bigint{3, 3, 5, 9, 8, 8});

    REQUIRE(multiply("12345", "67890") == bigint_from_string("838102050"));
}

TEST_CASE("Test assignment")
{
    auto a = bigint_from_string("3141592653589793238462643383279502884197169399375105820974944592");
    auto b = bigint_from_string("2718281828459045235360287471352662497757247093699959574966967627");

    auto expected = bigint_from_string("8539734222673567065463550869546574495034888535765114961879601127"
                                       "067743044893204848617875072216249073013374895871952806582723184");

    REQUIRE(multiply(a, b) == expected);
}

TEST_CASE("Plain sorting")
{
    auto simple_comp = [](int a, int b) { return a < b; };

    std::random_device random_device{};
    std::mt19937 mt_19937{random_device()};
    std::uniform_int_distribution<int> generator{0, 50};

    SECTION("manual examples")
    {
        REQUIRE(merge_sort(std::vector<int>{2, 1}, simple_comp) == std::vector<int>{1, 2});
        REQUIRE(merge_sort(std::vector<int>{3, 2, 1}, simple_comp) == std::vector<int>{1, 2, 3});
        REQUIRE(merge_sort(std::vector<int>{4, 2, 1, 3}, simple_comp) == std::vector<int>{1, 2, 3, 4});
        REQUIRE(merge_sort(std::vector<int>{4, 2, 1, 3, 5}, simple_comp) == std::vector<int>{1, 2, 3, 4, 5});

        REQUIRE(merge_sort<int>({5, 4, 3, 2, 1}, simple_comp) == std::vector<int>{1, 2, 3, 4, 5});

        REQUIRE(merge_sort(std::vector<int>{}, simple_comp).empty());
    }


    SECTION("Random examples")
    {
        for (int i = 0; i < 1000; ++i)
        {
            auto size_xs = generator(mt_19937);

            std::vector<int> xs(size_xs);

            std::generate(xs.begin(), xs.end(), [&]() { return generator(mt_19937); });

            std::vector<int> expected{};
            expected.reserve(size_xs);

            std::copy(xs.cbegin(), xs.cend(), std::back_inserter(expected));
            std::sort(expected.begin(), expected.end());

            REQUIRE(merge_sort(xs, simple_comp) == expected);
        }
    }
}

TEST_CASE("The sort is stable")
{
    auto fist_digit_comp = [](int a, int b) { return first_digit(a) < first_digit(b); };

    REQUIRE(merge_sort<int>({3, 3, 2, 2, 1, 1, 1}, fist_digit_comp) == std::vector<int>{1, 1, 1, 2, 2, 3, 3});
    REQUIRE(merge_sort<int>({32, 31, 21, 22, 15, 11, 1}, fist_digit_comp) == std::vector<int>{15, 11, 1, 21, 22, 32, 31});
}
