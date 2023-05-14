#include <cstdlib>
#include <fmt/core.h>

#include "src/product.hpp"

int main() {
    auto a = bigint_from_string("3141592653589793238462643383279502884197169399375105820974944592");
    auto b = bigint_from_string("2718281828459045235360287471352662497757247093699959574966967627");

    auto res = multiply(a , b);

    fmt::print("{}\n", string_from_bigint(res));

    return EXIT_SUCCESS;
}