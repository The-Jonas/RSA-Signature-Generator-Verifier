#include "../include/rsa_core.h"
#include "../include/math_utils.h"
#include <iostream>

namespace RSA {

    void init_keys(KeyPair& keys) {
        mpz_init(keys.n); mpz_init(keys.e); mpz_init(keys.d);
        mpz_init(keys.p); mpz_init(keys.q);
    }

    void generate_keys(KeyPair& keys, int bits) {
        std::cout << "Gerando p (" << bits << " bits)..." << std::endl;
        MathUtils::generate_prime(keys.p, bits, 40);

        std::cout << "Gerando q (" << bits << " bits)..." << std::endl;
        MathUtils::generate_prime(keys.q, bits, 40);

        // n = p * q
        mpz_mul(keys.n, keys.p, keys.q);

        // Tontiente de Euler -> phi(n) = (p - 1) * (q - 1)
        mpz_t p_minus_1, q_minus_1, phi;
        mpz_init(p_minus_1); mpz_init(q_minus_1); mpz_init(phi);

        mpz_sub_ui(p_minus_1, keys.p, 1);
        mpz_sub_ui(q_minus_1, keys.q, 1);
        mpz_mul(phi, p_minus_1, q_minus_1);

        // É o padrão mais seguro e rápido pra expoente público (Primo de Fermat: 2^16 + 1)
        mpz_set_ui(keys.e, 65537);

        // Calculando 'd' (Inverso modular de e % phi)
        // Usamos euclides estendido: a*x + b*y = gcd(a,b)
        // Queremos e*d + phi*y = 1, então 'x' será o nosso 'd'
        mpz_t g, y;
        mpz_init(g); mpz_init(y);

        MathUtils::extended_gcd(g, keys.d, y, keys.e, phi);

        // d = d % phi, se ele for negativo
        if (mpz_cmp_ui(keys.d, 0) < 0) {
            mpz_add(keys.d, keys.d, phi);
        }

        mpz_clear(p_minus_1); mpz_clear(q_minus_1); mpz_clear(phi); 
        mpz_clear(g); mpz_clear(y);
        std::cout << "Par de chaves RSA gerado com sucesso!" << std::endl;
    }

    void clear_keys(KeyPair& keys) {
        mpz_clear(keys.n); mpz_clear(keys.e);
        mpz_clear(keys.d); mpz_clear(keys.q);
    }

    // Cifragem bruta (M^e mod n)
    void encrypt_raw(mpz_t result, const mpz_t message, const KeyPair& keys) {
        MathUtils::power(result, message, keys.e, keys.n);
    }

    // Decifragem bruta (C^d mod n)
    void decrypt_raw(mpz_t result, const mpz_t cipher, const KeyPair& keys) {
        MathUtils::power(result, cipher, keys.d, keys.n);
    }

}