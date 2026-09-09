#include "../src/naive.hpp"

#include <random>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

void testNaiveSearch() {
    assert((naiveSearch("ababcabc", "abc") ==
            std::vector<std::size_t>{2, 5}));

    assert((naiveSearch("aaaaa", "aa") ==
            std::vector<std::size_t>{0, 1, 2, 3}));

    assert((naiveSearch("abcdef", "xyz") ==
            std::vector<std::size_t>{}));

    assert((naiveSearch("abc", "abcd") ==
            std::vector<std::size_t>{}));

    assert((naiveSearch("abc", "abc") ==
            std::vector<std::size_t>{0}));

    std::cout << "All naive search tests passed!\n";
}

#include "../src/kmp.hpp"

void testLPS() {
    assert((buildLPS("ABABC") ==
            std::vector<std::size_t>{0, 0, 1, 2, 0}));

    assert((buildLPS("AAAA") ==
            std::vector<std::size_t>{0, 1, 2, 3}));

    assert((buildLPS("ABCDE") ==
            std::vector<std::size_t>{0, 0, 0, 0, 0}));

    assert((buildLPS("AABAACAABAA") ==
            std::vector<std::size_t>{0, 1, 0, 1, 2, 0, 1, 2, 3, 4, 5}));

    assert((buildLPS("") ==
            std::vector<std::size_t>{}));

    std::cout << "All LPS tests passed!\n";
}

void testKMPSearch() {
    assert((kmpSearch("ababcabc", "abc") ==
            std::vector<std::size_t>{2, 5}));

    assert((kmpSearch("aaaaa", "aa") ==
            std::vector<std::size_t>{0, 1, 2, 3}));

    assert((kmpSearch("abcdef", "xyz") ==
            std::vector<std::size_t>{}));

    assert((kmpSearch("abc", "abcd") ==
            std::vector<std::size_t>{}));

    assert((kmpSearch("abc", "abc") ==
            std::vector<std::size_t>{0}));

    assert((kmpSearch("ABABABABC", "ABABC") ==
            std::vector<std::size_t>{4}));

    assert((kmpSearch("", "") ==
            std::vector<std::size_t>{0}));

    assert((kmpSearch("abc", "") ==
            std::vector<std::size_t>{0, 1, 2, 3}));

    std::cout << "All KMP search tests passed!\n";
}

void testRandomizedSearches() {
    std::mt19937 rng(12345); // Fixed seed makes tests reproducible.

    std::uniform_int_distribution<int> textLength(0, 100);
    std::uniform_int_distribution<int> patternLength(0, 20);
    std::uniform_int_distribution<int> character(0, 3);

    for (int test = 0; test < 1000; ++test) {
        std::string text;
        std::string pattern;

        int n = textLength(rng);
        int m = patternLength(rng);

        for (int i = 0; i < n; ++i) {
            text += static_cast<char>('a' + character(rng));
        }

        for (int i = 0; i < m; ++i) {
            pattern += static_cast<char>('a' + character(rng));
        }

        const auto naive = naiveSearch(text, pattern);
        const auto kmp = kmpSearch(text, pattern);

        if (naive != kmp) {
            throw std::runtime_error(
                "KMP and naive search disagree on test " +
                std::to_string(test)
            );
        }
    }

    std::cout << "All randomized search tests passed!\n";
}

int main() {
    testNaiveSearch();
    testLPS();
    testKMPSearch();
    testRandomizedSearches();
    return 0;
}