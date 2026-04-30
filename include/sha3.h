#ifndef SHA3_H
#define SHA3_H

#include <vector>
#include <cstdint>
#include <string>

namespace CryptoHash {
    // Função principal, que recebe bytes brutos (como o conteúdo de um arquivo)
    // Devolve exatamente 32 bytes (256 bits) do Hash SHA-3
    std::vector<uint8_t> sha3_256(const std::vector<uint8_t>& input);

    // Função auxiliar: Para facilitar o uso com strings de texto normais
    std::vector<uint8_t> sha3_256(const std::string& input);
}

#endif