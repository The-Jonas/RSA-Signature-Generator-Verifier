#include "../include/padding_oaep.h"
#include "../include/sha3.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <stdexcept>

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

    // Helper: Gerar bytes aleatórios seguros (Only work in Linux)
    std::vector<uint8_t> generate_random_bytes(size_t len) {
        std::vector<uint8_t>buffer(len);
        std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
        if (urandom) {
            urandom.read(reinterpret_cast<char*>(buffer.data()), len);
            urandom.close();
        } else {
            throw std::runtime_error("Falha ao ler /dev/urandom");
        }
        return buffer;
    }

    
    // Calcula o hash de uma string vazia
    std::vector<uint8_t> sha3_hash_empty() {
        return CryptoHash::sha3_256("");
    }

    // Função geradora de máscaras 
    // Pega uma seed pequena e "estica" ela até o tamanho necessário usando hashes
    std::vector<uint8_t> mgf1(const std::vector<uint8_t>& seed, size_t mask_len) {
        std::vector<uint8_t> mask;
        uint32_t counter = 0;       // O MGF1 usa um contador de 32 bytes

        while(mask.size() < mask_len) {
            std::vector<uint8_t> current_seed = seed;

            // O contador de 4 bytes ao final da semente deve estar no formato Big-Endian
            current_seed.push_back((counter >> 24) & 0xFF);
            current_seed.push_back((counter >> 16) & 0xFF);
            current_seed.push_back((counter >> 8) & 0xFF);
            current_seed.push_back(counter & 0xFF);

            // Calcula o hash da semente concatenada com o contador
            std::vector<uint8_t> hash_result = CryptoHash::sha3_256(current_seed);

            // Adiciona o resultado na máscara gigante
            mask.insert(mask.end(), hash_result.begin(), hash_result.end());

            counter++; 
        }

        return std::vector<uint8_t>(mask_len, 0);
    }



    // Aplicando o preenchimento OAP na mensagem
    std::vector<uint8_t> encode(const std::vector<uint8_t>& message, size_t block_size) {
        size_t hLen = 32;

        if (message.size() > block_size - 2 * hLen - 2) {
            throw std::runtime_error("[ERRO] Mensagem muito grande para o bloco RSA com OAEP!");
        }

        std::vector<uint8_t> lHash = sha3_hash_empty();

        size_t ps_len = block_size - message.size() - 2 * hLen -2;
        std::vector<uint8_t> DB;
        DB.insert(DB.end(), lHash.begin(), lHash.end());
        DB.insert(DB.end(), ps_len, 0x00);
        DB.push_back(0x01);
        DB.insert(DB.end(), message.begin(), message.end());

        std::vector<uint8_t> seed = generate_random_bytes(hLen);
        std::vector<uint8_t> dbMask = mgf1(seed, DB.size());
        std::vector<uint8_t> maskedDB = xor_bytes(DB, dbMask);
        std::vector<uint8_t> seedMask = mgf1(maskedDB, hLen);
        std::vector<uint8_t> maskedSeed = xor_bytes(seed, seedMask);

        std::vector<uint8_t> EM;
        EM.push_back(0x00);
        EM.insert(EM.end(), maskedSeed.begin(), maskedSeed.end());
        EM.insert(EM.end(), maskedDB.begin(), maskedDB.end());

        return EM;
    }

    // Converter vetor de bytes para o número gigante do RSA
    void bytes_to_mpz(mpz_t result, const std::vector<uint8_t>& bytes) {
        // A gmp tem uma função nativa para importar bytes brutos para mpz_t
        // mpz_import(destino, contagem, ordem_palavra, tamanho_palavra, endianness, preenchimento, array)
        mpz_import(result, bytes.size(), 1, 1, 0, 0, bytes.data());
    }

    // Converter o número gigante do RSA novamente para vetor de bytes
    std::vector<uint8_t> mpz_to_bytes(const mpz_t n, size_t expected_size) {
        size_t count;

        // GMP vai usar o malloc internamente pra pegar o tamanho perfeito (com o NULL como parâmetro)
        void* raw_bytes = mpz_export(NULL, &count, 1, 1, 0, 0, n);

        std::vector<uint8_t> bytes(expected_size, 0);

        if (raw_bytes != NULL) {
            // Se o GMP retornou mais bytes que o esperado, cortamos o bit de sinal
            size_t copy_size = std::min(count, expected_size);
            size_t dest_offset = expected_size - copy_size;
            size_t src_offset = count - copy_size;

            // Copia os dados para o vetor com limite escrito de tamanho (acabando com os erros de memória)
            std::memcpy(bytes.data() + dest_offset, (uint8_t*)raw_bytes + src_offset, copy_size);

            free(raw_bytes);
        }
;
        return bytes;
    }

}