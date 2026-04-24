#ifndef PADDING_OAEP_H
#define PADDING_OAEP_H

#include <vector>
#include <cstdint>
#include <gmp.h>

namespace OAEP {

    // MGF1 é o motor do OAEP
    // Ele vai usar a função Hash para "esticar" uma string pequena de bytes até o tamanho que quisermos
    std::vector<uint8_t> mgf1(const std::vector<uint8_t>& seed, size_t mask_len);

    // Aplica o preenchimento OAEP na mensagem antes de enviar pro RSA
    // block_size para RSA de 1024 bits será 128 bytes (1024 / 8)
    std::vector<uint8_t> encode(const std::vector<uint8_t>& message, size_t block_size);

    // Remove o preenchimento OAEP após decifrar com o RSA
    std::vector<uint8_t> decode(const std::vector<uint8_t>& encoded_block, size_t block_size);

    // Funções auxiliares para converter entre os bytes do OAEP e o mpz_t do RSA
    void bytes_to_mpz(mpz_t result, const std::vector<uint8_t>& bytes);
    std::vector<uint8_t> mpz_to_bytes(const mpz_t n, size_t expected_size);

};

#endif