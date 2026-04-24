#include "../include/padding_oaep.h"
#include <iostream>

namespace OAEP {
    
    // Helper: Função para fazer o XOR entre dois vetores de bytes
    std::vector<uint8_t> xor_bytes(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
        size_t len = std::min(a.size(), b.size());
        std::vector<uint8_t> result(len);
        for (size_t i = 0; i < len; ++i) {
            result[i] = a[i] ^ b[i];            // O operador bitwise XOR do C++ é o (^)

        }
        return result;
    }

    // Para o MGF1, primeiro temos que implementar o SHA-3

}