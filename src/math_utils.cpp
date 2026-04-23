#include "../include/math_utils.h"
#include <gmp.h>
#include <iostream>

// Usar o teste de Miller-Rabin para gerar números primos gigantes

namespace MathUtils {

    // Exponenciação Modular: (base^exp) % mod
    void power(mpz_t result, const mpz_t base, const mpz_t exp, const mpz_t mod) {
        // Função de aritmetica modular do GMP que já faz isso de forma otimizada
        mpz_powm(result, base, exp, mod);
    }

    // Algoritmo de Euclides Estendido
    // Encontra g, x, y tais que ax + by = g = gcd(a, b)
    void extended_gcd(mpz_t g, mpz_t x, mpz_t y, const mpz_t a, const mpz_t b) {
        mpz_gcdext(g, x, y, a, b);
    }

    // Teste de primalidade de Mille-Rabin
    bool miller_rabin(const mpz_t n, int iterations) {
        // Casos base
        if (mpz_cmp_ui(n, 2) < 0) return false;
        if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) return true;
        if (mpz_tstbit(n, 0) == 0) return false;

        // Achar r e d tais que n - 1 = 2^r * d
        mpz_t d, n_minus_1;
        mpz_inits(d, n_minus_1, NULL);
        mpz_sub_ui(n_minus_1, n, 1);
        mpz_set (d, n_minus_1);

        unsigned long int r = 0;
        while (mpz_tstbit(d, 0) == 0) { // Enquanto d for par
            mpz_fdiv_q_2exp(d, d, 1);   // d = d / 2
            r++; 
        }

        // Estado para números aleátorios
        gmp_randstate_t state;
        gmp_randinit_default(state);
        gmp_randseed_ui(state, time(NULL));

        bool is_prime = true;
        mpz_t a, x;
        mpz_inits(a, x, NULL);

        // Rodar as iterações para teste
        for (int i = 0; i < iterations; i++) {
            // Gera uma base 'a' aleátoria no intervalo [2, n - 2]
            mpz_t limit;
            mpz_init(limit);
            mpz_sub_ui(limit, n, 4);
            mpz_urandomm(a, state, limit);
            mpz_add_ui(a, a, 2);
            mpz_clear(limit);

            // Calcula x = a^d % n
            power(x, a, d, n);

            if (mpz_cmp_ui(x,1) == 0 || mpz_cmp(x, n_minus_1) == 0) {
                continue;
            }

            bool composite = true;
            for (unsigned long int j = 0; j < r - 1; j++) {
                mpz_powm_ui(x, x, 2, n);
                if (mpz_cmp(x, n_minus_1) == 0) {
                    composite = false;
                    break;
                }
            }

            if (composite) {
                is_prime = false;
                break;
            }
        }

        // Limpeza
        mpz_clears(d, n_minus_1, a, x, NULL);
        gmp_randclear(state);
        return is_prime;
    }

    void generate_prime(mpz_t prime, int bits, int iterations) {
        gmp_randstate_t state;
        gmp_randinit_default(state);
        gmp_randseed_ui(state, time(NULL) + clock()); 

        // Gera um número aleatório com o número exato de 'bits'
        mpz_urandomb(prime, state, bits);

        // Forçamos o bit mais significativo a ser 1. Isso garante que o número 
        // terá EXATAMENTE 1024 bits, e não menos (como um 1023 bits disfarçado).
        mpz_setbit(prime, bits - 1);
        
        // Forçamos o último bit a ser 1, garantindo que o número é ÍMPAR.
        mpz_setbit(prime, 0);

        // Fica testando e somando 2 até o Miller-Rabin aprovar
        while (!miller_rabin(prime, iterations)) {
            mpz_add_ui(prime, prime, 2);
        }

        gmp_randclear(state);
    }
} 