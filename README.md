# 🛡️ RSA Signature Generator & Verifier

Implementação de um sistema de geração e verificação de assinaturas digitais baseadas no algoritmo RSA, desenvolvida como requisito para a disciplina de **Segurança Computacional** da Universidade de Brasília (UnB).

## 📖 Descrição do Projeto

O objetivo deste trabalho é implementar as primitivas criptográficas do RSA "do zero", sem o auxílio de bibliotecas de alto nível para a lógica de cifração. O sistema permite a geração de chaves seguras, a proteção de mensagens via OAEP (Optimal Asymmetric Encryption Padding) e a garantia de integridade através de assinaturas digitais matemáticas.

## 💻 Funcionalidades Principais (Requisitos)

### Parte I: Chaves e Cifração
* **Geração de Chaves:** Produção de primos $p$ e $q$ com no mínimo 1024 bits.
* **Teste de Primalidade:** Implementação do algoritmo de probabilidade de **Miller-Rabin**.
* **Cifração/Decifração RSA:** Implementação da lógica assimétrica utilizando preenchimento **OAEP** para segurança semântica.

### Parte II: Assinatura Digital
* **Hashing:** Cálculo de integridade utilizando a função de hash **SHA-3 (Keccak)**.
* **Assinatura:** Cifração matemática do hash da mensagem com a chave privada ($d$).
* **Formatação:** Codificação do resultado final em **BASE64** (Padrão PEM) para transporte seguro.

### Parte III: Verificação
* **Parsing:** Extração estruturada de chaves e assinaturas de arquivos BASE64.
* **Decifração de Assinatura:** Recuperação do hash original via chave pública ($E, N$) e reversão da Rede de Feistel (OAEP).
* **Validação:** Comparação criptográfica entre o hash decifrado e o hash recém-calculado do arquivo original.

## 🛠️ Tecnologias Utilizadas
* **Linguagem:** C++ (Padrão C++11).
* **Aritmética Modular:** Biblioteca `GMP` (GNU Multiple Precision Arithmetic Library) para lidar com a matemática de números gigantes (BigInt).
* **Hash e Parsing:** Biblioteca `Crypto++` (Apenas para SHA-3 e codificação nativa BASE64).
* **Restrição:** As primitivas de geração de chave (RSA) e padding (OAEP) foram implementadas manualmente, sem uso de bibliotecas de alto nível como OpenSSL para o núcleo criptográfico.

---

## ⚙️ Pré-requisitos e Instalação

Para compilar e rodar este projeto em ambientes Linux (ou WSL no Windows), você precisará dos compiladores padrão e das bibliotecas GMP e Crypto++. 

Execute o comando abaixo para instalar as dependências no Ubuntu/Debian:
```bash
sudo apt update
sudo apt install build-essential libgmp-dev libcrypto++-dev
```
## 🚀 Como Compilar e Executar
### 1. Compilação
Na raiz do projeto basta executar no terminal:
```bash
make
```
Isso irá gerar automaticamente três executáveis: run_math_test(Gerador de primos gigantes), run_signature (O Assinador) e run_verifier (O Verificador).

Observação: Desses 3 executáveis, apenas o "run_math_test" é dispensável para o resultado final, sendo assim, usado apenas para confirmação de que realmente os números gerados pelo Miller-Rabin implementado são primos.

### 2. Gerando a Assinatura
Para ler o documento de teste, calcular o hash, gerar as chaves e criar o arquivo blindado .sig:
```bash
./run_signature
```
O programa fará toda a matemática e gerará um arquivo de saída chamado contrato.sig na mesma pasta, contendo as Chaves Públicas e a Assinatura Cifrada em formato BASE64.

### 3. Verificando a Autenticidade
Para atestar se a assinatura é válida ou se o arquivo sofreu adulteração:
```bash
./run_verifier
```
O programa lerá o contrato.sig, desfará a criptografia RSA, reverterá a Rede de Feistel, calculará o hash do texto atual e o confrontará com a assinatura original, retornando [VERDE] ASSINATURA VALIDA se tudo estiver íntegro, ou [VERMELHO] INVALIDA em caso de fraude.

### 4. Limpeza
Para limpar os executáveis e os arquivos de assinatura gerados, voltando o diretório ao seu estado original:
```bash
make clean
```
