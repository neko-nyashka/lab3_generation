#ifndef STATS_H
#define STATS_H
#include <cmath>
#include <iostream>
/**
 * @brief Computes the mean of the given data.
 * @param data Pointer to the array of values.
 * @param len Number of elements in the array.
 * @return The mean value.
 */
long double mean(const uint32_t* data, int len);

/**
 * @brief Computes the standard deviation of the given data.
 * @param data Pointer to the array of values.
 * @param len Number of elements in the array.
 * @param mean The mean of the data.
 * @return The standard deviation.
 */
long double stddev(const uint32_t* data, int len, long double mean);

/**
 * @brief Computes the coefficient of variation.
 * @param mean The mean of the data.
 * @param std_dev The standard deviation.
 * @return The coefficient of variation (as a percentage).
 */
long double cv(long double mean, long double std_dev);


/**
 * @brief Checks the chi-squared hypothesis test for the sample.
 * @param sample Vector of integer sample data.
 * @return 0 if accepted, 1 if rejected.
 */
int checking_chi2(std::vector<uint32_t> &sample);

/**
 * @brief Returns theoretical chi-squared value.
 * @param v Degrees of freedom.
 * @param x_p Significance level (quantile).
 * @return Theoretical chi-squared value.
 */
double chi2_teor(int v, double x_p);

/**
 * @brief Calculates experimental chi-squared value.
 * @param sample Vector of integer sample data.
 * @return Experimental chi-squared value.
 */
double chi2_exp(const std::vector<uint32_t>& sample);

#endif