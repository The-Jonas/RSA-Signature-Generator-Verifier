#include "../include/formatter.h"
#include "../include/padding_oaep.h"
#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <fstream>                          
#include <iostream>

namespace Formatter {

    std::string encode_base64(const std::vector<uint8_t>& data) {
        std::string encoded;

        // O StringSource liga os dados brutos ao filtro Base64Encoder
        CryptoPP::StringSource ss(data.data(), data.size(), true,
            new CryptoPP::Base64Encoder(
                new CryptoPP::StringSink(encoded),
                false
            )                                                          // 'false' evita que insira quebras de linha automática.
        );
        return encoded;
    }

    std::vector<uint8_t> decode_base64(const std::string& base64_str) {
        std::string decoded_str;

        CryptoPP::StringSource ss(base64_str, true,
            new CryptoPP::Base64Decoder(
                new CryptoPP::StringSink(decoded_str)
            )
        );
        return std::vector<uint8_t>(decoded_str.begin(), decoded_str.end());
    }
    
    void save_signature_file(const std::string& filename, const mpz_t signature, const RSA::KeyPair& keys) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Erro ao criar o arquivo de assinatura: " << filename << std::endl;
            return;
        }

        // Precisamos converter o mpz_t para bytes antes do BASE64, a chave tem 1024 bits (128 bytes) e o 'e' é menor (65537) - 4 bytes
        std::vector<uint8_t> sig_bytes = OAEP::mpz_to_bytes(signature, 128);
        std::vector<uint8_t> n_bytes = OAEP::mpz_to_bytes(keys.n, 128);
        std::vector<uint8_t> e_bytes = OAEP::mpz_to_bytes(keys.e, 4);

        // Colocar "Tags" para o Verificador achar mais fácil depois
        file << "-----BEGIN RSA PUBLIC KEY N-----\n";
        file << encode_base64(n_bytes) << "\n";
        file << "-----END RSA PUBLIC KEY N-----\n\n";

        file << "-----BEGIN RSA PUBLIC KEY E-----\n";
        file << encode_base64(e_bytes) << "\n";
        file << "-----END RSA PUBLIC KEY E-----\n\n";

        file << "-----BEGIN RSA SIGNATURE-----\n";
        file << encode_base64(sig_bytes) << "\n";
        file << "-----END RSA SIGNATURE-----\n";

        file.close();
        std::cout << "Arquivo de assinatura salvo com sucesso em: " << filename << std::endl;
    }
}
