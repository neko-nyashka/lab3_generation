#include "nist.h"

// Frequency (Monobit) Test
// To check the balance between zeros and ones in the entire sequence.
int monobit_test(const std::vector<uint32_t>& sample) {
    int n = (sample.size() * 32);
    int sum = 0;
    for (uint32_t num : sample) {
        for (int i = 0; i < 32; ++i) {
			int bit = (num >> i) & 1;
			sum += (bit == 1) ? 1 : -1;

        }
    }
    double s_obs = fabs(sum) / sqrt(n);
    double p_val = erfc(s_obs / M_SQRT2);
	if (p_val > 0.01) {
		std::cout<<"| Monobit PASS ";
	} else {
		std::cout<<"| Monobit FAIL ";
	}
	 return p_val > 0.01;
}


// Runs Test
// To check the alternation of zeros and ones.
int runs_test(const std::vector<uint32_t>& sample) {
    int n = (sample.size() * 32);
    int one_count = 0;
	int runs = 1;
	int pred_bit = 0, bit = 0;
    for (uint32_t num : sample) {
        for (int i = 0; i < 32; ++i) {
			if (i > 0) pred_bit = bit;
			bit = (num >> i) & 1;
			if (bit != pred_bit) runs++;
			one_count += bit;
        }
    }
	double pi = double(one_count) / n;
	double p_val;
	if (fabs(pi - 0.5) >= 2.0 / sqrt(n)) {
        p_val = 0.0;
	} else {
		double numerator = runs - 2.0 * n * pi * (1 - pi);
    	double denominator = 2.0 * sqrt(n) * pi * (1 - pi);
    	p_val = erfc(fabs(numerator) / denominator);
	}
	if (p_val > 0.01) {
		std::cout<<"| Runs PASS ";
	} else {
		std::cout<<"| Runs FAIL ";
	}
	return p_val > 0.01;
}

// Standard Normal Distribution Function
double Phi(double x) {
    return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}
// Cumulative Sums (Cusums) Test
// To check the maximum deviation of the accumulated amount.
int cusum_test(const std::vector<uint32_t>& sample) {
  int n = (sample.size() * 32);
    int sum = 0, max_sum = 0;
    for (uint32_t num : sample) {
        for (int i = 0; i < 32; ++i) {
			int bit = (num >> i) & 1;
			sum += (bit == 1) ? 1 : -1;
			if (abs(sum) > max_sum) max_sum = abs(sum);
        }
    }
    
    double z = max_sum;
    double p = 0.0;
    int k_start = int((-n/z + 1)/4);
    int k_end = int((n/z - 1)/4);
    
    for (int k = k_start; k <= k_end; k++) {
        p += Phi((4*k+1)*z/sqrt(n)) - Phi((4*k-1)*z/sqrt(n));
    }
    double p_val =  1.0 - p;
	if (p_val > 0.01) {
		std::cout<<"| CuSum PASS ";
	} else {
		std::cout<<"| Cusum FAIL ";
	}
	return p_val  > 0.01;
}

// Block Frequency Test
// Check the uniformity of small blocks.
int block_frequency_test(const std::vector<uint32_t>& sample) {
	int block_size = 128;
    int num_blocks = sample.size() * 32 / block_size;
    double chi_sq = 0.0;
    for (int i = 0; i < num_blocks; i++) {
        int ones = 0;
		for(int ind = i * 4; ind < (i + 1) * 4; ++ind) {
			for (int j = 0; j < 32; ++j) {

				ones += (sample[ind] >> j) & 1;
			}
		}
        double pi = double(ones) / block_size;
        chi_sq += (pi - 0.5) * (pi - 0.5);
    }
    chi_sq *= 4 * block_size;

    // X -> N(v, 2v):
    // v = N_blocks;  Z = (X - v) / sqrt(2v);  p = erfc(|Z|/sqrt(2)).
    double nu = double(num_blocks);
    double Z  = (chi_sq - nu) / sqrt(2.0 * nu);
    double p_val = erfc(abs(Z) / sqrt(2.0) );
	if (p_val > 0.01) {
		std::cout<<"| BlockFreq PASS ";
	} else {
		std::cout<<"| BlockFreq FAIL ";
	}
    return p_val > 0.01;
}


// Longest Run of Ones in a Block (Longest series Test)
// To check the length of the longest sequence of units.
int longest_run_test(const std::vector<uint32_t>& sample) {
    int total_bits = sample.size() * 32;
    int M = 128;
    int K = 3;

    std::vector<double> pi = { 0.1174, 0.2430, 0.2493, 0.1752, 0.1027, 0.1124 };
   
    int num_blocks = total_bits / M;
    std::vector<int> counts(K + 1, 0);
    int bit_pos = 0, run = 0, max_run = 0;

    for (uint32_t x : sample) {
        for (int b = 0; b < 32; ++b) {
            int bit = (x >> b) & 1;
            if (bit) {
                run++;
                if (run > max_run) max_run = run;
            } else {
                run = 0;
            }
            bit_pos++;
            if (bit_pos == M) {
                int idx = 0;  
				if (max_run <= 4) idx = 0;
				else if (max_run == 5) idx = 1;
				else if (max_run == 6) idx = 2;
				else if (max_run == 7) idx = 3;
				else if (max_run == 8) idx = 4;
                else idx = 5;
               
                counts[idx]++;
                bit_pos = 0;
                run = 0;
                max_run = 0;
            }
        }
    }

    double chi2 = 0.0;
    for (int i = 0; i <= K; ++i) {
        double expected = double(num_blocks) * pi[i];
        double diff = double(counts[i]) - expected;
        chi2 += (diff * diff) / expected;
    }

    double Z = (chi2 - K) / sqrt(2.0 * K);
    double p_val = erfc(fabs(Z) / sqrt(2.0));

    if (p_val > 0.01) {
        std::cout << "| LRO PASS ";
    } else {
        std::cout << "| LRO FAIL ";
    }

    return p_val > 0.01;
}