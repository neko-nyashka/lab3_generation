#include "stats.h"
long double mean(const uint32_t* data, int len) {
	uint64_t sum = 0;
	for (int i = 0; i < len; i++) {
        sum += data[i];
    }
	return sum / len;
}
long double stddev(const uint32_t* data, int len, long double mean) {
	long double sq_sum = 0; 
	
	for (int i = 0; i < len; i++) {
        sq_sum += (data[i] - mean) * (data[i] - mean) ;
    }
	return sqrtl(sq_sum / (len - 1));

}
long double cv(long double mean, long double std_dev) {
	return (std_dev / mean) * 100;
}
	


double chi2_teor(int v, double x_p) {
    return v + sqrt(2.0 * v) * x_p + (2.0/3.0) * (x_p * x_p) - (2.0/3.0);
}


double chi2_exp(const std::vector<uint32_t>& sample) {

	int bins = 1+ std::floor(log2(sample.size()));
	
    const uint64_t total_range = static_cast<uint64_t>(UINT32_MAX) + 1;
    const uint64_t bin_size = total_range / bins;
    
    std::vector<int> freq(bins, 0);


    for (uint32_t num : sample) {
        uint64_t value = num;
        int bin_index = value / bin_size;
        if (bin_index >= bins) bin_index = bins - 1;
        freq[bin_index]++;
    }

    double n = sample.size();
    double expected = n / bins;
    

    double chi_square = 0.0;
    for (int count : freq) {
        double diff = count - expected;
        chi_square += (diff * diff) / expected;
    }

    return chi_square;
}


int checking_chi2(std::vector<uint32_t> &sample) {
	int n = sample.size();
    int v = floor(log2(double(n)));
    double x_25 = -0.674;
    double x_75 = 0.674;

    double chi2_e = chi2_exp(sample);
    double chi2_teor_left = chi2_teor(v, x_25);
    double chi2_teor_right = chi2_teor(v, x_75);
    int passed = chi2_teor_left < chi2_e && chi2_e < chi2_teor_right;
    std::string res =  passed ? "| chi2 PASS " : "| chi2 FAIL ";
    std::cout << res;

    return passed;
}