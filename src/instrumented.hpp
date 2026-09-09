#pragma once

#include <cstddef>
#include <string>
#include <vector>

struct SearchStats {
    std::vector<std::size_t> matches;
    std::size_t comparisons = 0;
};

SearchStats naiveSearchWithStats(
    const std::string& text,
    const std::string& pattern
);

SearchStats kmpSearchWithStats(
    const std::string& text,
    const std::string& pattern
);