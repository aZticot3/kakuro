# Compilateur et options
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -I./include

# Répertoires du projet
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = include

# Récupération de la liste des fichiers sources
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
DEPS = $(patsubst $(OBJ_DIR)/%.o,$(OBJ_DIR)/%.d,$(OBJS))

# Ajouter main.cpp aux objets
MAIN_OBJ = $(OBJ_DIR)/main.o
MAIN_DEP = $(OBJ_DIR)/main.d

# Nom de l'exécutable
TARGET = $(BIN_DIR)/kakuro

# Bibliothèque nlohmann/json (supposée installée dans le système)
# Si elle est incluse dans le projet, ajustez le chemin
JSON_LIB = -I/usr/include/nlohmann

# Options de linkage
LDFLAGS = 

# Bibliothèques supplémentaires
LIBS = 

# Règle par défaut
all: directories $(TARGET)

# Création des répertoires nécessaires
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Règle de création de l'exécutable
$(TARGET): $(OBJS) $(MAIN_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

# Règle de compilation des fichiers source
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(JSON_LIB) -MMD -MP -c $< -o $@

# Règle spéciale pour main.cpp
$(MAIN_OBJ): main.cpp
	$(CXX) $(CXXFLAGS) $(JSON_LIB) -MMD -MP -c $< -o $@

# Règle de clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Règle de clean et rebuild
rebuild: clean all

# Inclusion des fichiers de dépendances
-include $(DEPS) $(MAIN_DEP)

# Indique que 'all', 'clean', etc. sont des noms de règles et non des fichiers
.PHONY: all clean rebuild directories