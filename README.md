# 🛡️ RSA Signature Generator & Verifier
Implementação de um sistema de geração e verificação de assinaturas digitais baseadas no algoritmo RSA, desenvolvida como requisito para a disciplina de **Segurança Computacional (CIC0201)** da Universidade de Brasília (UnB).

## Descrição do Projeto
O objetivo deste trabalho é implementar as primitivas criptográficas do RSA "do zero", sem o auxílio de bibliotecas de alto nível para a lógica de cifração. O sistema permite a geração de chaves seguras, a proteção de mensagens via OAEP e a garantia de integridade através de assinaturas digitais

## 💻 Funcionalidades Principais (Requisitos)

### Parte I: Chaves e Cifração
* **Geração de Chaves:** Produção de primos $p$ e $q$ com no mínimo 1024 bits.
* **Teste de Primalidade:** Implementação do algoritmo de **Miller-Rabin**.
* **Cifração/Decifração RSA:** Implementação da lógica assimétrica utilizando **OAEP** (Optimal Asymmetric Encryption Padding).

### Parte II: Assinatura Digital
* **Hashing:** Cálculo de integridade utilizando a função de hash **SHA-3**.
* **Assinatura:** Cifração do hash da mensagem.
* **Formatação:** Codificação do resultado final em **BASE64** para transporte e verificação.

### Parte III: Verificação
* **Parsing:** Decodificação de documentos em BASE64.
* **Decifração de Assinatura:** Recuperação do hash original via chave pública.
* **Validação:** Comparação entre o hash decifrado e o hash calculado do arquivo original.

## 🛠️ Tecnologias Utilizadas
* **Linguagem:** C++.
* **Aritmética Modular:** Utilização de biblioteca para manipulação de grandes números ($BigInt$), conforme permitido.
* **Restrição:** Não foram utilizadas bibliotecas como OpenSSL para as primitivas de cifração e geração de chaves.
