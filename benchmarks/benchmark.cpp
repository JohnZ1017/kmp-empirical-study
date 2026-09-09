#include "../src/naive.hpp"
#include "../src/kmp.hpp"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

using Clock = std::chrono::steady_clock;

// Prevent the compiler from treating the search result as unused.
volatile std::size_t resultSink = 0;

// Generate a reproducible random string.
std::string randomString(std::size_t length, std::mt19937& rng) {
    std::uniform_int_distribution<int> character(0, 3);
    std::string result(length, 'a');

    for (char& c : result) {
        c = static_cast<char>('a' + character(rng));
    }

    return result;
}

// Run one search and return the elapsed time in microseconds.
template <typename SearchFunction>
double measureSearch(
    SearchFunction search,
    const std::string& text,
    const std::string& pattern
) {
    const auto start = Clock::now();

    const auto matches = search(text, pattern);

    const auto end = Clock::now();

    resultSink = resultSink + matches.size();

    return std::chrono::duration<double, std::micro>(
        end - start
    ).count();
}

// Return the median of a collection of timings.
double median(std::vector<double> values) {
    std::sort(values.begin(), values.end());

    const std::size_t middle = values.size() / 2;

    if (values.size() % 2 == 0) {
        return (values[middle - 1] + values[middle]) / 2.0;
    }

    return values[middle];
}

void runAdversarialBenchmark() {
    std::ofstream csv("benchmarks/adversarial_results.csv");

    if (!csv) {
        throw std::runtime_error("Could not open adversarial results CSV");
    }

    csv << "text_length,pattern_length,naive_us,kmp_us\n";

    const std::vector<std::size_t> textLengths = {
        1000, 5000, 10000, 50000, 100000, 500000
    };

    const std::size_t patternLength = 20;
    const int repetitions = 15;

    // The pattern almost matches at every position, but ends in 'b'.
    const std::string pattern(patternLength - 1, 'a');
    const std::string adversarialPattern = pattern + 'b';

    for (std::size_t n : textLengths) {
        const std::string text(n, 'a');

        // Verify both algorithms agree before timing.
        if (naiveSearch(text, adversarialPattern) !=
            kmpSearch(text, adversarialPattern)) {
            throw std::runtime_error("Search implementations disagree");
        }

        std::vector<double> naiveTimes;
        std::vector<double> kmpTimes;

        for (int repetition = 0; repetition < repetitions; ++repetition) {
            if (repetition % 2 == 0) {
                naiveTimes.push_back(
                    measureSearch(naiveSearch, text, adversarialPattern)
                );
                kmpTimes.push_back(
                    measureSearch(kmpSearch, text, adversarialPattern)
                );
            } else {
                kmpTimes.push_back(
                    measureSearch(kmpSearch, text, adversarialPattern)
                );
                naiveTimes.push_back(
                    measureSearch(naiveSearch, text, adversarialPattern)
                );
            }
        }

        const double naiveMedian = median(naiveTimes);
        const double kmpMedian = median(kmpTimes);

        csv << n << ','
            << patternLength << ','
            << naiveMedian << ','
            << kmpMedian << '\n';

        std::cout << "Adversarial n = " << n
                  << " | naive = " << naiveMedian << " us"
                  << " | KMP = " << kmpMedian << " us\n";
    }

    std::cout << "Results saved to benchmarks/adversarial_results.csv\n";
}

int main() {
    std::mt19937 rng(12345);

    std::ofstream csv("benchmarks/random_results.csv");

    if (!csv) {
        throw std::runtime_error("Could not open results CSV");
    }

    csv << "text_length,pattern_length,naive_us,kmp_us\n";

    const std::vector<std::size_t> textLengths = {
        1000, 5000, 10000, 50000, 100000, 500000
    };

    const std::size_t patternLength = 20;
    const int repetitions = 15;

    for (std::size_t n : textLengths) {
        const std::string text = randomString(n, rng);
        const std::string pattern = randomString(patternLength, rng);

        // Correctness check before measuring.
        if (naiveSearch(text, pattern) != kmpSearch(text, pattern)) {
            throw std::runtime_error("Search implementations disagree");
        }

        std::vector<double> naiveTimes;
        std::vector<double> kmpTimes;

        for (int repetition = 0; repetition < repetitions; ++repetition) {
            // Alternate the order to reduce systematic ordering bias.
            if (repetition % 2 == 0) {
                naiveTimes.push_back(
                    measureSearch(naiveSearch, text, pattern)
                );
                kmpTimes.push_back(
                    measureSearch(kmpSearch, text, pattern)
                );
            } else {
                kmpTimes.push_back(
                    measureSearch(kmpSearch, text, pattern)
                );
                naiveTimes.push_back(
                    measureSearch(naiveSearch, text, pattern)
                );
            }
        }

        const double naiveMedian = median(naiveTimes);
        const double kmpMedian = median(kmpTimes);

        csv << n << ','
            << patternLength << ','
            << naiveMedian << ','
            << kmpMedian << '\n';

        std::cout << "n = " << n
                  << " | naive = " << naiveMedian << " us"
                  << " | KMP = " << kmpMedian << " us\n";
    }

    std::cout << "Results saved to benchmarks/random_results.csv\n";
    runAdversarialBenchmark();
    return 0;
}