#include "kmp.hpp"

std::vector<std::size_t> buildLPS(
    const std::string& pattern
) {
    std::vector<std::size_t> lps(pattern.size(), 0);

    std::size_t length = 0;
    std::size_t i = 1;

    while (i < pattern.size()) {
        if (pattern[i] == pattern[length]) {
            ++length;
            lps[i] = length;
            ++i;
        } else if (length != 0) {
            length = lps[length - 1];
        } else {
            lps[i] = 0;
            ++i;
        }
    }

    return lps;
}

std::vector<std::size_t> kmpSearch(
    const std::string& text,
    const std::string& pattern
) {
    std::vector<std::size_t> matches;

    // Match the same empty-pattern convention as naiveSearch.
    if (pattern.empty()) {
        for (std::size_t i = 0; i <= text.size(); ++i) {
            matches.push_back(i);
        }
        return matches;
    }

    const std::vector<std::size_t> lps = buildLPS(pattern);

    std::size_t i = 0; // Current position in the text.
    std::size_t j = 0; // Number of pattern characters currently matched.

    while (i < text.size()) {
        if (text[i] == pattern[j]) {
            ++i;
            ++j;

            // The entire pattern has matched.
            if (j == pattern.size()) {
                matches.push_back(i - j);

                // Continue searching, including overlapping matches.
                j = lps[j - 1];
            }
        } else if (j != 0) {
            // Keep the longest prefix that could still match.
            j = lps[j - 1];
        } else {
            // No matched prefix remains, so advance in the text.
            ++i;
        }
    }

    return matches;
}