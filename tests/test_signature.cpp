#include <iostream>
#include <iomanip>
#include <string>
#include "../include/math_utils.h"
#include "../include/rsa_core.h"
#include "../include/padding_oaep.h"
#include "../include/sha3.h"
#include "../include/formatter.h"

// Função auxiliar para imprimir vetores de bytes em hexadecimal
void print_hex(const std::string& label, const std::vector<uint8_t>& data) {
    std::cout << label << " (" << data.size() << "bytes):" << std::endl;
    for (size_t i = 0; i < data.size(); i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
        if ((i + 1) % 32 == 0) std::cout << std::endl; // Isso é uma quebra de linha a cada 32 bytes
    }
    std::cout << std::dec << "\n" << std::endl;
}

int main() {
    std::cout << "===========================================" << std::endl;
    std::cout << " TESTE DE INTEGRAÇÃO: GERADOR DE ASSINATURA" << std::endl;
    std::cout << "===========================================\n" << std::endl;

    // 1. O Documento Original
    std::string documento = "Este eh um contrato confidencial da Universidade de Brasilia.";
    std::cout << "[1] DOCUMENTO ORIGINAL:\n" << documento << "\n\n";

    // 2. Cálculo do Hash SHA-3 (Parte II - 1)
    std::cout << "Calculando Hash SHA-3..." << std::endl;
    std::vector<uint8_t> hash_documento = CryptoHash::sha3_256(documento);
    print_hex("[2] HASH DO DOCUMENTO (SHA-3)", hash_documento);

    // 3. Geração das Chaves RSA (Parte I - a)
    std::cout << "===========================================" << std::endl;
    RSA::KeyPair minhas_chaves;
    RSA::init_keys(minhas_chaves);
    // Usando 1024 bits para a chave. O tamanho do bloco (block_size) será 128 bytes. (128 * 8)
    RSA::generate_keys(minhas_chaves, 1024); 
    std::cout << "\n===========================================\n" << std::endl;

    // 4. Aplicação do Preenchimento OAEP (Parte I - b)
    std::cout << "Blindando o Hash com a Rede de Feistel (OAEP)..." << std::endl;
    std::vector<uint8_t> bloco_blindado = OAEP::encode(hash_documento, 128);
    print_hex("[3] BLOCO BLINDADO (OAEP)", bloco_blindado);

    // 5. Assinatura Matemática (Parte II - 2)
    // ATENÇÃO: Assinar significa cifrar com a CHAVE PRIVADA (d). 
    // Na nossa matemática bruta, a operação (C^d mod n) está na função decrypt_raw.
    mpz_t mensagem_matematica, assinatura_final;
    mpz_init(mensagem_matematica);
    mpz_init(assinatura_final);

    // Converte os 128 bytes para um número gigante
    OAEP::bytes_to_mpz(mensagem_matematica, bloco_blindado);

    std::cout << "Assinando matematicamente com a Chave Privada (RSA)..." << std::endl;
    RSA::decrypt_raw(assinatura_final, mensagem_matematica, minhas_chaves);

    // Mostra o resultado final como um número gigante em base 10
    std::cout << "\n[4] ASSINATURA DIGITAL GERADA (Número Gigante):" << std::endl;
    mpz_out_str(stdout, 10, assinatura_final);
    std::cout << "\n\n";

    std::cout << "Formatando e salvando em arquivo..." << std::endl;
    Formatter::save_signature_file("contrato.sig", assinatura_final, minhas_chaves);

    // Limpeza de memória
    mpz_clear(mensagem_matematica);
    mpz_clear(assinatura_final);
    RSA::clear_keys(minhas_chaves);

    std::cout << "===========================================" << std::endl;
    std::cout << " TESTE CONCLUÍDO COM SUCESSO!" << std::endl;
    std::cout << "===========================================" << std::endl;

    return 0;
}