#include <iostream>
#include <cstdint>
#include <chrono>
#include <mpir.h>
#include <cmath>

bool is_prime(uint64_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// lucas-lehmer test
bool is_mersenne_prime(uint64_t p) {
    if (p == 2) return true;
    if (!is_prime(p)) return false;

    mpz_t s, M;
    mpz_inits(s, M, NULL);

    // M = 2^p - 1 using mpz_setbit for efficiency
    mpz_setbit(M, p);  // M = 1 << p
    mpz_sub_ui(M, M, 1);

    // s = 4
    mpz_set_ui(s, 4);

    for (uint64_t i = 0; i < p - 2; ++i) {
        // s = (s * s - 2) % M
        mpz_powm_ui(s, s, 2, M);  // s = s^2 mod M
        mpz_sub_ui(s, s, 2);
        if (mpz_cmp_ui(s, 0) < 0)
            mpz_add(s, s, M);
    }

    bool result = (mpz_cmp_ui(s, 0) == 0);

    mpz_clears(s, M, NULL);
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <exponent>" << std::endl;
        return 1;
    }

    uint64_t n = std::stoull(argv[1]);

    std::cout << "Testing Mersenne number: 2^" << n << " - 1" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    bool is_prime_result = is_mersenne_prime(n);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    if (is_prime_result) {
        std::cout << "2^" << n << " - 1 is a Mersenne prime!" << std::endl;
    } else {
        std::cout << "2^" << n << " - 1 is not a Mersenne prime." << std::endl;
    }

    std::cout << "Time taken: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}