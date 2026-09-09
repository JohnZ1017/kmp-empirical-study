#include <string>
#include <vector>

std::vector<std::size_t> naiveSearch(
    const std::string& text,
    const std::string& pattern
) {
    std::vector<std::size_t> matches;

    // If the pattern is longer than the text, it cannot match.
    if (pattern.size() > text.size()) {
        return matches;
    }

    // Try every possible starting position.
    for (std::size_t i = 0; i + pattern.size() <= text.size(); ++i) {
        std::size_t j = 0;

        // Compare the pattern against the text at position i.
        while (j < pattern.size() && text[i + j] == pattern[j]) {
            ++j;
        }

        // If every character matched, record the starting position.
        if (j == pattern.size()) {
            matches.push_back(i);
        }
    }

    return matches;
}