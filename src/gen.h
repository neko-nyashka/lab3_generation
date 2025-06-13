#ifndef GEN_H
#define GEN_H

#include <chrono>
#include <random>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

/**
 * @brief Returns the current time in microseconds as a 32-bit integer.
 * @return A time-based seed value.
 */
uint32_t time_seed();

/**
 * @brief Computes the next value using the mid-square method.
 * @param x The previous value.
 * @return The next pseudo-random number.
 */
uint32_t mid_square_next(uint32_t x);

/**
 * @brief Generates a sequence using the mid-square method.
 * @param seed Initial seed value.
 * @param len Length of the sequence.
 * @param a Output array for the generated numbers.
 */
void mid_square_sample(uint32_t seed, int len, uint32_t* a);

/**
 * @brief Computes the next value using the Linear Congruential Generator (LCG) method.
 * @param x The previous value.
 * @return The next pseudo-random number.
 */
uint32_t lcg_next(uint32_t x);

/**
 * @brief Generates a sequence using the LCG method.
 * @param seed Initial seed value.
 * @param len Length of the sequence.
 * @param a Output array for the generated numbers.
 */
void lcg_sample(uint32_t seed, int len, uint32_t* a);

/**
 * @brief Computes the next value using the xorshift method.
 * @param x The previous value.
 * @return The next pseudo-random number.
 */
uint32_t xorshift_next(uint32_t x);

/**
 * @brief Generates a sequence using the xorshift method.
 * @param seed Initial seed value.
 * @param len Length of the sequence.
 * @param a Output array for the generated numbers.
 */
void xorshift_sample(uint32_t seed, int len, uint32_t* a);

/**
 * @brief Generates a sequence using the Mersenne Twister (mt19937) generator.
 * @param seed Initial seed value.
 * @param len Length of the sequence.
 * @param a Output array for the generated numbers.
 */
void mt_sample(uint32_t seed, int len, uint32_t* a);


/**
 * @brief Generates multiple samples using the provided generator and performs statistical analysis.
 * @param samples Output vector of sequences.
 * @param generator Pointer to a generator function.
 * @param name Name of the generator (used for logging/output).
 */
void generate_and_analyze(std::vector<std::vector<uint32_t>>& samples,
                          void (*generator)(uint32_t, int, uint32_t*),
                          const std::string& name);

/**
 * @brief Benchmarks the execution time of different random number generators and logs results.
 */
void time();


#endif 
