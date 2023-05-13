#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <optional>

using bigint = std::vector<uint8_t>;

bigint bigint_from_string(std::string const& str);

std::string string_from_bigint(bigint const& n);

bigint add(bigint const& lhs, bigint const& rhs);

bigint subtract(bigint const& lhs, bigint const& rhs);

bigint multiply(bigint const& lhs, bigint const& rhs);