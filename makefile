# Compilador e Flags
CXX = g++
CXXFLAGS = -Wall -std=c++11 -O2
LIBS = -lgmp -lcryptopp

# Diretórios
SRC_DIR = src
TEST_DIR = tests

# Arquivos fonte base (tudo que está no src)
SRCS = $(SRC_DIR)/math_utils.cpp \
       $(SRC_DIR)/rsa_core.cpp \
       $(SRC_DIR)/padding_oaep.cpp \
       $(SRC_DIR)/sha3.cpp \
       $(SRC_DIR)/formatter.cpp

# Alvos principais
all: math signature verifier

# Compila o Math
math:
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_math.cpp $(SRCS) -o run_math_test $(LIBS)

# Compila o Assinador
signature:
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_signature.cpp $(SRCS) -o run_signature $(LIBS)

# Compila o Verificador
verifier:
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_verifier.cpp $(SRCS) -o run_verifier $(LIBS)

# Limpa os executáveis e arquivos gerados
clean:
	rm -f run_math_test run_signature run_verifier contrato.sig


