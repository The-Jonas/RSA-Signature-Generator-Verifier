#ifndef RSA_CORE_H
#define RSA_CORE_H

#include <gmp.h>

namespace RSA {

    // Estrutura para guardar as Keypairs
    struct KeyPair {
        mpz_t n;                            // Módulo n = p * q
        mpz_t e;                            // Exponente público
        mpz_t d;                            // Exponente privado
        mpz_t p;                            // Número primo p
        mpz_t q;                            // Número primo q
    };

    // Funções de gerenciamento do ciclo de vida das chaves.
    void init_keys(KeyPair& keys);
    void generate_keys(KeyPair& keys, int bits);
    void clear_keys(KeyPair& keys);

    // Para Textbook RSA (Temporário)
    void encrypt_raw(mpz_t result, const mpz_t message, const KeyPair& keys);
    void decrypt_raw(mpz_t result, const mpz_t cipher, const KeyPair& keys);
}

#endif