#include "../src/naive.hpp"
#include "../src/kmp.hpp"
#include "../src/instrumented.hpp"

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

// Prevent the compiler from treating search results as unused.
volatile std::size_t resultSink = 0;

// Generate a reproducible random string using a four-character alphabet.
std::string randomString(std::size_t length, std::mt19937& rng) {
    std::uniform_int_distribution<int> character(0, 3);
    std::string result(length, 'a');

    for (char& c : result) {
        c = static_cast<char>('a' + character(rng));
    }

    return result;
}

// Run one search and return elapsed time in microseconds.
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

// Return the median of a nonempty collection of timings.
double median(std::vector<double> values) {
    if (values.empty()) {
        throw std::invalid_argument("Cannot calculate median of empty data");
    }

    std::sort(values.begin(), values.end());

    const std::size_t middle = values.size() / 2;

    if (values.size() % 2 == 0) {
        return (values[middle - 1] + values[middle]) / 2.0;
    }

    return values[middle];
}

// Experiment 1: Random text, fixed pattern length.
void runRandomBenchmark() {
    std::mt19937 rng(12345);

    std::ofstream csv("benchmarks/random_results.csv");

    if (!csv) {
        throw std::runtime_error("Could not open random results CSV");
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

        // Check correctness before measuring.
        if (naiveSearch(text, pattern) != kmpSearch(text, pattern)) {
            throw std::runtime_error("Search implementations disagree");
        }

        std::vector<double> naiveTimes;
        std::vector<double> kmpTimes;

        for (int repetition = 0; repetition < repetitions; ++repetition) {
            // Alternate measurement order to reduce ordering bias.
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

        std::cout << "Random n = " << n
                  << " | naive = " << naiveMedian << " us"
                  << " | KMP = " << kmpMedian << " us\n";
    }

    std::cout << "Results saved to benchmarks/random_results.csv\n";
}

// Experiment 2: Adversarial text, fixed pattern length.
void runAdversarialBenchmark() {
    std::ofstream csv("benchmarks/adversarial_results.csv");

    if (!csv) {
        throw std::runtime_error("Could not open adversarial results CSV");
    }

    csv << "text_length,pattern_length,naive_us,kmp_us,"
           "naive_comparisons,kmp_comparisons\n";

    const std::vector<std::size_t> textLengths = {
        1000, 5000, 10000, 50000, 100000, 500000
    };

    const std::size_t patternLength = 20;
    const int repetitions = 15;

    // The text contains only 'a'.
    // The pattern contains 19 'a' characters followed by 'b'.
    const std::string adversarialPattern =
        std::string(patternLength - 1, 'a') + 'b';

    for (std::size_t n : textLengths) {
        const std::string text(n, 'a');

        // Check correctness before measuring.
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

        // Count comparisons separately from timing measurements.
        const auto naiveStats =
            naiveSearchWithStats(text, adversarialPattern);

        const auto kmpStats =
            kmpSearchWithStats(text, adversarialPattern);

        if (naiveStats.matches != kmpStats.matches) {
            throw std::runtime_error("Instrumented searches disagree");
        }

        csv << n << ','
            << patternLength << ','
            << naiveMedian << ','
            << kmpMedian << ','
            << naiveStats.comparisons << ','
            << kmpStats.comparisons << '\n';

        std::cout << "Adversarial n = " << n
                  << " | naive = " << naiveMedian << " us"
                  << " | KMP = " << kmpMedian << " us\n";

        std::cout << "Comparisons | naive = "
                  << naiveStats.comparisons
                  << " | KMP = "
                  << kmpStats.comparisons << '\n';
    }

    std::cout
        << "Results saved to benchmarks/adversarial_results.csv\n";
}

// Experiment 3: Adversarial text, increasing pattern length.
void runPatternLengthBenchmark() {
    std::ofstream csv("benchmarks/pattern_length_results.csv");

    if (!csv) {
        throw std::runtime_error("Could not open pattern-length results CSV");
    }

    csv << "text_length,pattern_length,naive_us,kmp_us,"
           "naive_comparisons,kmp_comparisons\n";

    const std::size_t textLength = 100000;
    const std::vector<std::size_t> patternLengths = {
        10, 20, 40, 80, 160, 320, 640
    };
    const int repetitions = 15;

    // The text contains only 'a' characters.
    const std::string text(textLength, 'a');

    for (std::size_t m : patternLengths) {
        // The pattern almost matches, but its final character is 'b'.
        const std::string pattern =
            std::string(m - 1, 'a') + 'b';

        // Check correctness before measuring.
        if (naiveSearch(text, pattern) != kmpSearch(text, pattern)) {
            throw std::runtime_error("Search implementations disagree");
        }

        std::vector<double> naiveTimes;
        std::vector<double> kmpTimes;

        for (int repetition = 0; repetition < repetitions; ++repetition) {
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

        // Count comparisons separately from timing measurements.
        const auto naiveStats =
            naiveSearchWithStats(text, pattern);

        const auto kmpStats =
            kmpSearchWithStats(text, pattern);

        if (naiveStats.matches != kmpStats.matches) {
            throw std::runtime_error("Instrumented searches disagree");
        }

        csv << textLength << ','
            << m << ','
            << naiveMedian << ','
            << kmpMedian << ','
            << naiveStats.comparisons << ','
            << kmpStats.comparisons << '\n';

        std::cout << "Pattern length m = " << m
                  << " | naive = " << naiveMedian << " us"
                  << " | KMP = " << kmpMedian << " us\n";

        std::cout << "Comparisons | naive = "
                  << naiveStats.comparisons
                  << " | KMP = "
                  << kmpStats.comparisons << '\n';
    }

    std::cout
        << "Results saved to benchmarks/pattern_length_results.csv\n";
}

int main() {
    try {
        runRandomBenchmark();
        runAdversarialBenchmark();
        runPatternLengthBenchmark();

        std::cout << "All benchmarks completed.\n";
    } catch (const std::exception& error) {
        std::cerr << "Benchmark error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}