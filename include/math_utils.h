#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <gmp.h>

namespace MathUtils {
    // Para calcular (base^exp) % mod, de maneira otimizada
    void power(mpz_t result, const mpz_t base, const mpz_t exp, const mpz_t mod);

    // Teste de Primalidade de Miller-Rabin
    bool miller_rabin(const mpz_t n, int interations);

    // Algoritmo de Euclides Estendido (para achar o inverso modular 'd')
    void extended_gcd(mpz_t g, mpz_t x, mpz_t y, const mpz_t a, const mpz_t b);

    // Gera um número primo com a quantidade de bits pedida
    void generate_prime(mpz_t prime, int bits, int iterations);
}

#endif