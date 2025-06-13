#include "gen.h"
#include "nist.h"
#include "stats.h"

uint32_t time_seed() {
    auto now = std::chrono::high_resolution_clock::now();
    auto micro = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    return uint32_t(micro.time_since_epoch().count());
}

// The middle squares method
uint32_t mid_square_next(uint32_t x) {
    uint64_t tmp = uint64_t(x) * x;
	uint64_t mid = tmp & 0xFFFFFFFF0000;
    return  uint32_t(mid >> 16); 
}


void mid_square_sample(uint32_t seed, int len, uint32_t* a) {
	uint32_t x = seed;
    for (int i = 0; i < len; i++) {
        uint32_t next = mid_square_next(x);
		next = next == 0 ? seed + i : next;
        a[i] = next;
        x = next; 
    }
}


// Linear congruent method r_i+1 = (k * r_i + b) % M
uint32_t lcg_next(uint32_t x) {
	uint64_t product = uint64_t(1664525) * x + 1013904223u;
    return uint32_t(product); 
}

void lcg_sample(uint32_t seed, int len, uint32_t* a) {
    uint32_t x = seed;  
    
    for (int i = 0; i < len; i++) {
        uint32_t next = lcg_next(x);
        a[i] = next;
        x = next;
        
    }
}

// Xorshift
uint32_t xorshift_next(uint32_t x) {
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        return x;
	}


void xorshift_sample(uint32_t seed, int len, uint32_t* a) {
    uint32_t x = seed;  
    
    for (int i = 0; i < len; i++) {
        uint32_t next = xorshift_next(x);
        a[i] = next;
        x = next;
        
    }
}


void mt_sample(uint32_t seed, int len, uint32_t* a) {
    std::mt19937 gen(seed);
    for (int i = 0; i < len; i++) {
        a[i] = gen();
    }
}



void generate_and_analyze(
    std::vector<std::vector<uint32_t>>& samples, void (*generator)(uint32_t, int, uint32_t*), const std::string& name) {
	int sample_count = 20;                       
    int sample_size = 1000;      
    std::cout << "\n" << name << "\n";
    samples.clear();
    samples.reserve(sample_count);
    
    for (int i = 1; i <= sample_count; i++) {
        std::vector<uint32_t> sample(sample_size);
        generator(time_seed() + i*i, sample_size, sample.data());
        
        samples.push_back(sample);
        
        long double m = mean(sample.data(), sample_size);
        long double sd = stddev(sample.data(), sample_size, m);
        long double c = cv(m, sd);
        
        std::cout << "Sample " << i 
                  << ": size = " << sample_size
                  << ", mean: " << uint64_t(m)
                  << ", stddev: " << uint64_t(sd)
                  << ", cv: " << c << "%\n";
    }
}



void time() {
    std::ofstream fout("results.csv");
	int sizes[13] = {1000, 2000, 3000, 10000, 20000, 30000, 50000, 100000, 200000, 300000, 500000, 800000, 1000000};
    if (!fout) {
        std::cerr << "Error opening file" << "\n";
        return;
    }

    fout << "size,mid_square,lcg,xorshift,mt19937\n";
    int max_size = 1000000;
    std::vector<uint32_t> buffer(max_size);
    for (int len : sizes) {
        uint32_t seed = time_seed();

        // mid_square_sample
        auto start = std::chrono::high_resolution_clock::now();
        mid_square_sample(seed, len, buffer.data());
        auto end = std::chrono::high_resolution_clock::now();
        double mid_square_time = std::chrono::duration<double, std::milli>(end - start).count();

        // lcg_sample
        start = std::chrono::high_resolution_clock::now();
        lcg_sample(seed, len, buffer.data());
        end = std::chrono::high_resolution_clock::now();
        double lcg_time = std::chrono::duration<double, std::milli>(end - start).count();

        // xorshift_sample
        start = std::chrono::high_resolution_clock::now();
        xorshift_sample(seed, len, buffer.data());
        end = std::chrono::high_resolution_clock::now();
        double xorshift_time = std::chrono::duration<double, std::milli>(end - start).count();

        // mt_sample
        start = std::chrono::high_resolution_clock::now();
        mt_sample(seed, len, buffer.data());
        end = std::chrono::high_resolution_clock::now();
        double mt_time = std::chrono::duration<double, std::milli>(end - start).count();

        fout << len << "," 
             << mid_square_time << "," 
             << lcg_time << "," 
             << xorshift_time << "," 
             << mt_time << "\n";

    }

    fout.close();
}

int main() {

    std::vector<std::vector<uint32_t>> mid_square_samples;
	std::vector<std::vector<uint32_t>> lcg_samples;
	std::vector<std::vector<uint32_t>> xorshift_samples;
	std::vector<std::vector<uint32_t>> mt_samples;

	generate_and_analyze(mid_square_samples, mid_square_sample, "The middle squares method");
    generate_and_analyze(lcg_samples, lcg_sample, "Linear congruent method");
    generate_and_analyze(xorshift_samples, xorshift_sample, "Xorshift method");
    generate_and_analyze(mt_samples, mt_sample, "MT19937 method");

	std::vector<std::vector<std::vector<uint32_t>>> all_samples = {
        mid_square_samples, 
        lcg_samples, 
        xorshift_samples, 
        mt_samples
    };
    
    std::vector<std::string> method_names = {
        "mid_square", "lcg", "xorshift", "mt19937"
    };

 	for (int m = 0; m < int(all_samples.size()); m++) {
        auto& samples = all_samples[m];
        std::string method = method_names[m];
        std::cout<< '\n' <<method<<'\n';
        for (size_t i = 0; i < samples.size(); i++) {
            auto& sample = samples[i];
			std::cout <<"|"<< std::setw(2)<< i ;
            checking_chi2(sample);
			monobit_test(sample);
			runs_test(sample);
			cusum_test(sample);
			block_frequency_test(sample);
			longest_run_test(sample);
			std::cout<<'\n';
        }
    }

	time();


    
    return 0;
}