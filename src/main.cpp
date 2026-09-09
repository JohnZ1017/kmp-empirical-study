#include "naive.hpp"

#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string text = "ababcabc";
    std::string pattern = "abc";

    std::vector<std::size_t> matches = naiveSearch(text, pattern);

    std::cout << "Matches at indices: ";

    for (std::size_t index : matches) {
        std::cout << index << " ";
    }

    std::cout << '\n';

    return 0;
}