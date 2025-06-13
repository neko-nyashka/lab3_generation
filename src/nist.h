#ifndef NIST_H
#define NIST_H
#include <iostream>
/**
 * @brief Performs the monobit test (frequency test) for randomness.
 * @param sample The sequence to test.
 * @return 1 if the test is passed, otherwise 0.
 */
int monobit_test(const std::vector<uint32_t>& sample);

/**
 * @brief Performs the runs test for randomness.
 * @param sample The sequence to test.
 * @return 1 if the test is passed, otherwise 0.
 */
int runs_test(const std::vector<uint32_t>& sample);

/**
 * @brief Performs the cumulative sums (Cusum) test for randomness.
 * @param sample The sequence to test.
 * @return 1 if the test is passed, otherwise 0.
 */
int cusum_test(const std::vector<uint32_t>& sample);

/**
 * @brief Performs the block frequency test.
 * @param sample The sequence to test.
 * @return 1 if the test is passed, otherwise 0.
 */
int block_frequency_test(const std::vector<uint32_t>& sample);

/**
 * @brief Performs the longest run of ones in a block test.
 * @param sample The sequence to test.
 * @return 1 if the test is passed, otherwise 0.
 */
int longest_run_test(const std::vector<uint32_t>& sample);

/**
 * @brief Computes the cumulative distribution function (CDF) of the standard normal distribution.
 * @param x Input value.
 * @return The CDF value (Phi).
 */
double Phi(double x);

#endif