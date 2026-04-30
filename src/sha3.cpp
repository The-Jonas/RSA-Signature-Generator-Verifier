#include "../include/sha3.h"
#include <cryptopp/sha3.h>

namespace CryptoHash {

    std::vector<uint8_t> sha3_256(const std::vector<uint8_t>& input) {
        CryptoPP::SHA3_256 hash;
        std::vector<uint8_t> digest(hash.DigestSize());
        // O DigestSize é de 32 bits, daí vamos alimentar o hash com os dados e finalizar.

        hash.Update(input.data(), input.size());
        hash.Final(digest.data());

        return digest;
    }

    std::vector<uint8_t> sha3_256(const std::string& input) {
        CryptoPP::SHA3_256 hash;
        std::vector<uint8_t> digest(hash.DigestSize());

        // Usamos o reinterpret_cast pra converter o caracteres da string em dados brutos
        hash.Update(reinterpret_cast<const uint8_t*>(input.data()), input.size());
        hash.Final(digest.data());

        return digest;
    }
}
