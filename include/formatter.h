#ifndef FORMATTER_H
#define FORMATTER_H

#include <string>
#include <vector>
#include <cstdint>
#include <gmp.h>
#include "rsa_core.h"

namespace Formatter {

    // Converter um vetor de bytes brutos para uma string em BASE64
    std::string encode_base64(const std::vector<uint8_t>& data);

    // Converte uma string em BASE64 de volta para bytes brutos
    std::vector<uint8_t> decode_base64(const std::string& base64_str);

    // Salva a assinatura e as chaves públicas em um arquivo formatado
    void save_signature_file(const std::string& filename, const mpz_t signature, const RSA::KeyPair& keys);
    
}

#endif