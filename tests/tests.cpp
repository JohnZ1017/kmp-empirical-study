#include "../src/naive.hpp"

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

int main() {
    testNaiveSearch();
    testLPS();
    testKMPSearch();
    return 0;
}