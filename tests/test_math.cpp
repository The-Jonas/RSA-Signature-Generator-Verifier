#include <iostream>
#include <gmp.h>
#include "../include/math_utils.h"

using namespace std;

int main() {
    cout << "=== Iniciando Testes do MathUtils (Miller-Rabin) ===" << endl;

    mpz_t num_primo, num_composto;
    mpz_inits(num_primo, num_composto, NULL);

    // Teste 1: Número Primo Conhecido (65537)
    // A função mpz_set_str permite carregar números gigantes a partir de strings (base 10)
    mpz_set_str(num_primo, "65537", 10);
    cout << "Testando 65537 (Esperado: Primo): ";
    if (MathUtils::miller_rabin(num_primo, 40)) {
        cout << "[PASSOU]" << endl;
    } else {
        cout << "[FALHOU]" << endl;
    }

    // Teste 2: Número Composto Conhecido (65535)
    mpz_set_str(num_composto, "65535", 10);
    cout << "Testando 65535 (Esperado: Composto): ";
    if (!MathUtils::miller_rabin(num_composto, 40)) {
        cout << "[PASSOU]" << endl;
    } else {
        cout << "[FALHOU]" << endl;
    }

    // Teste 3: Gerando um Primo Gigante de 1024 bits
    mpz_t meu_primo_p;
    mpz_init(meu_primo_p);
    
    cout << "\nGerando um primo de 1024 bits ..." << endl;
    
    // Passamos a variável, o tamanho (1024) e a segurança do Miller-Rabin (40)
    MathUtils::generate_prime(meu_primo_p, 1024, 40);
    
    cout << "[SUCESSO] Primo gerado!" << endl;
    
    cout << "Valor do Primo (Base 10):" << endl;
    mpz_out_str(stdout, 10, meu_primo_p);
    cout << "\n\nTamanho em bits: " << mpz_sizeinbase(meu_primo_p, 2) << " bits" << endl;

    // Limpeza de memória
    mpz_clears(num_primo, num_composto, NULL);
    
    cout << "=== Fim dos Testes ===" << endl;
    return 0;
}