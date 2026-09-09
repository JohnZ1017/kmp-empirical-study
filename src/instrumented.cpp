#include "instrumented.hpp"
#include "kmp.hpp"

SearchStats naiveSearchWithStats(
    const std::string& text,
    const std::string& pattern
) {
    SearchStats stats;

    if (pattern.size() > text.size()) {
        return stats;
    }

    for (std::size_t i = 0; i + pattern.size() <= text.size(); ++i) {
        std::size_t j = 0;

        while (j < pattern.size()) {
            ++stats.comparisons;

            if (text[i + j] != pattern[j]) {
                break;
            }

            ++j;
        }

        if (j == pattern.size()) {
            stats.matches.push_back(i);
        }
    }

    return stats;
}

SearchStats kmpSearchWithStats(
    const std::string& text,
    const std::string& pattern
) {
    SearchStats stats;

    // Use the same empty-pattern convention as the original search.
    if (pattern.empty()) {
        for (std::size_t i = 0; i <= text.size(); ++i) {
            stats.matches.push_back(i);
        }
        return stats;
    }

    const auto lps = buildLPS(pattern);

    std::size_t i = 0;
    std::size_t j = 0;

    while (i < text.size()) {
        ++stats.comparisons;

        if (text[i] == pattern[j]) {
            ++i;
            ++j;

            if (j == pattern.size()) {
                stats.matches.push_back(i - j);
                j = lps[j - 1];
            }
        } else if (j != 0) {
            j = lps[j - 1];
        } else {
            ++i;
        }
    }

    return stats;
}