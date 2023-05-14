#include <random>
#include <catch2/catch_test_macros.hpp>

#include "../src/merge_sort.hpp"

int first_digit(int x)
{
    if (x == 0)
        return 0;

    int digits = floor(log10(abs(x))) + 1;
    return x / std::pow(10, digits - 1);
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
