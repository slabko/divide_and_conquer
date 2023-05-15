#pragma once

#include <vector>
#include <cmath>

template <typename InputIt, typename OutputIt, typename CompFunc>
inline static int merge(OutputIt dst, InputIt first, InputIt middle, InputIt last, CompFunc comp)
{
    size_t inversions = 0;

    auto split = middle;
    for (; first != split; ++first)
    {
        for (; middle != last && comp(*middle, *first); ++middle)
        {
            inversions += std::distance(first, split);
            *dst++ = *middle;
        }
        *dst++ = *first;
    }

    std::copy(middle, last, dst);

    return inversions;
}

template <typename T, typename CompFunc>
std::vector<T> merge_sort(std::vector<T> const & data, CompFunc comp, size_t& inversions)
{
    inversions = 0;
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

        for (auto first = src->begin(); first <= src->end(); first += step)
        {
            auto middle = std::min(first + split, src->end());
            auto last = std::min(first + step, src->end());
            inversions += merge(std::back_inserter(*dst), first, middle, last, comp);
        }
    }

    return *dst;
}

template <typename T>
std::vector<T> merge_sort(std::vector<T> const & data) {
    size_t inversions{0};
    auto simple_compare = [](T a, T b){ return a < b; };
    return merge_sort(data, simple_compare, inversions);
}

template <typename T>
std::vector<T> merge_sort(std::vector<T> const & data, size_t& inversions) {
    auto simple_compare = [](T a, T b){ return a < b; };
    return merge_sort(data, simple_compare, inversions);
}

template <typename T, typename CompFunc>
std::vector<T> merge_sort(std::vector<T> const & data, CompFunc comp) {
    size_t inversions{0};
    return merge_sort(data, comp, inversions);
}

