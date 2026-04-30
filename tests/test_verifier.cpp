#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "../include/rsa_core.h"
#include "../include/formatter.h"
#include "../include/padding_oaep.h"
#include "../include/sha3.h"

// Função auxiliar (Parsing): Extrai um bloco específico de BASE64 do arquivo
std::string extract_block(const std::string& filename, const std::string& header, const std::string& footer) {
    std::ifstream file(filename);
    std::string line, block = "";
    bool inside = false;
    while (std::getline(file, line)) {
        // Limpa o caractere fantasma do Windows antes de ler o Base64
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        if (line == footer) break;
        if (inside) block += line;
        if (line == header) inside = true;
    }
    return block;
}

// Função para imprimir em hexadecimal (Para fins visuais)
void print_hex(const std::string& label, const std::vector<uint8_t>& data) {
    std::cout << label << ":" << std::endl;
    for (size_t i = 0; i < data.size(); i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    std::cout << std::dec << "\n\n";
}

int main() {
    std::cout << "===========================================" << std::endl;
    std::cout << " TESTE FINAL: VERIFICADOR DE ASSINATURAS" << std::endl;
    std::cout << "===========================================\n" << std::endl;

    // O documento de teste
    std::string documento = "Este eh um contrato confidencial da Universidade de Brasilia.";
    std::cout << "Lendo documento...\n";

    // PARSING (Parte III - 1): Lendo as chaves públicas e a assinatura do .sig
    std::cout << "Lendo arquivo de assinatura (contrato.sig)...\n";
    std::string n_b64 = extract_block("contrato.sig", "-----BEGIN RSA PUBLIC KEY N-----", "-----END RSA PUBLIC KEY N-----");
    std::string e_b64 = extract_block("contrato.sig", "-----BEGIN RSA PUBLIC KEY E-----", "-----END RSA PUBLIC KEY E-----");
    std::string sig_b64 = extract_block("contrato.sig", "-----BEGIN RSA SIGNATURE-----", "-----END RSA SIGNATURE-----");

    if (n_b64.empty() || e_b64.empty() || sig_b64.empty()) {
        std::cerr << "[ERRO] Arquivo de assinatura corrompido ou não encontrado!" << std::endl;
        return 1;
    }

    // Convertendo BASE64 de volta para bytes puros
    std::vector<uint8_t> n_bytes = Formatter::decode_base64(n_b64);
    std::vector<uint8_t> e_bytes = Formatter::decode_base64(e_b64);
    std::vector<uint8_t> sig_bytes = Formatter::decode_base64(sig_b64);

    // Carregando as chaves públicas para o formato matemático da GMP
    RSA::KeyPair chave_publica;
    RSA::init_keys(chave_publica);
    OAEP::bytes_to_mpz(chave_publica.n, n_bytes);
    OAEP::bytes_to_mpz(chave_publica.e, e_bytes);

    mpz_t assinatura_mpz, bloco_blindado_mpz;
    mpz_init(assinatura_mpz);
    mpz_init(bloco_blindado_mpz);
    OAEP::bytes_to_mpz(assinatura_mpz, sig_bytes);

    // DECIFRAGEM (Parte III - 2): Usando a chave pública (E, N) para reverter a assinatura
    std::cout << "Decifrando assinatura com a Chave Pública (RSA)...\n";
    RSA::encrypt_raw(bloco_blindado_mpz, assinatura_mpz, chave_publica);

    // O bloco volta a ter 128 bytes
    std::vector<uint8_t> bloco_blindado = OAEP::mpz_to_bytes(bloco_blindado_mpz, 128);
    
    // "Dando ré" na Rede de Feistal do OAEP
    std::cout << "Desfazendo blindagem OAEP...\n";
    std::vector<uint8_t> hash_assinado;
    try {
        hash_assinado = OAEP::decode(bloco_blindado, 128);
    } catch (const std::exception& e) {
        std::cout << "\n[!!!] ALERTA DE SEGURANCA: " << e.what() << "\n";
        std::cout << ">>> RESULTADO: ASSINATURA INVALIDA (Adulterada ou corrompida)\n";
        return 1;
    }

    print_hex("-> Hash revelado dentro da assinatura", hash_assinado);

    // VERIFICAÇÃO (Parte III - 3): Cálculo e comparação do hash do arquivo

    std::cout << "Calculando o Hash do documento atual...\n";
    std::vector<uint8_t> hash_calculado = CryptoHash::sha3_256(documento);
    
    print_hex("-> Hash real do documento", hash_calculado);

    // Veredito final
    std::cout << "===========================================" << std::endl;
    if (hash_assinado == hash_calculado) {
        std::cout << "  [VERDE] ASSINATURA VALIDA E AUTENTICA!" << std::endl;
        std::cout << "  O documento eh legitimo e nao foi alterado." << std::endl;
    } else {
        std::cout << "  [VERMELHO] ASSINATURA INVALIDA!" << std::endl;
        std::cout << "  O documento foi adulterado apos a assinatura." << std::endl;
    }
    std::cout << "===========================================\n" << std::endl;

    // Limpeza de memória
    mpz_clear(assinatura_mpz); 
    mpz_clear(bloco_blindado_mpz);
    RSA::clear_keys(chave_publica);

    return 0;
}