#pragma once

#include <cstddef>
#include <string>
#include <vector>

std::vector<std::size_t> naiveSearch(
    const std::string& text,
    const std::string& pattern
);