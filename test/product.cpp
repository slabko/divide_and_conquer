#include <catch2/catch_test_macros.hpp>

#include "../src/merge_sort.hpp"
#include "../src/product.hpp"

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
