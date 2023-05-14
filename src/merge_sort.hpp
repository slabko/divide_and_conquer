#pragma once

#include <vector>
#include <cmath>

template <typename InputIt, typename OutputIt, typename CompFunc>
inline static void merge(OutputIt dst, InputIt left, InputIt right, InputIt end, CompFunc comp)
{
    auto split = right;
    for (; left != split; ++left)
    {
        for (; right != end && comp(*right, *left); ++right)
        {
            *dst++ = *right;
        }
        *dst++ = *left;
    }

    std::copy(right, end, dst);
}

template <typename T, typename CompFunc>
std::vector<T> merge_sort(std::vector<T> const & data, CompFunc comp)
{
    if (data.empty())
        return {};

    std::vector<int> buf1{};
    buf1.insert(buf1.begin(), data.cbegin(), data.cend());

    std::vector<int> buf2{};
    buf2.reserve(data.size());

    // They will be swapped right away
    auto * dst = &buf1;
    auto * src = &buf2;

    size_t depth = std::ceil(std::log2(data.size()));
    for (int i = 0; i < depth; ++i)
    {
        std::swap(dst, src);

        dst->clear();

        int split = std::floor(std::pow(2, i));
        int step = split * 2;

        for (auto it = src->begin(); it <= src->end(); it += step)
        {
            merge(std::back_inserter(*dst), it, std::min(it + split, src->end()), std::min(it + step, src->end()), comp);
        }
    }

    return *dst;
}