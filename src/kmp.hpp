#pragma once

#include <cstddef>
#include <string>
#include <vector>

std::vector<std::size_t> buildLPS(
    const std::string& pattern
);

std::vector<std::size_t> kmpSearch(
    const std::string& text,
    const std::string& pattern
);