DAY=day5

CPP=g++

ROOT_DIR=.
UTIL_SRC_DIR=$(UTIL_DIR)/src/
UTIL_OBJ_DIR=$(UTIL_DIR)/obj/

UTIL_DIR=../$(ROOT_DIR)/util/
SRC_DIR=$(ROOT_DIR)/src/
OBJ_DIR=$(ROOT_DIR)/obj/
BIN_DIR=$(ROOT_DIR)/bin/

UTIL_SRC:=\
	FileTokenizer.cpp

UTIL_OBJ:=$(patsubst %, $(UTIL_OBJ_DIR)/%,$(UTIL_SRC:.cpp=.o))

# UTIL_OBJ:=$(UTIL_SRC:$(UTIL_SRC_DIR)/%.cpp=$(UTIL_OBJ_DIR)/%.o)

SRC:=$(wildcard $(SRC_DIR)/*.cpp)
OBJ:=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXE:=$(BIN_DIR)/$(DAY)

PPFLAGS= -I../util/src/
CPPFLAGS= --std=gnu++17 \
-Wall \
-Werror \
-Wextra \
-Wpedantic \
-Wshadow \
-Wno-unnused-variable \
-Wcast-align \
-O3 \
-fno-exceptions

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR) $(UTIL_OBJ)
	$(CPP) $(UTIL_OBJ) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(PPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

$(UTIL_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(PPFLAGS) -c $< -o $@

$(UTIL_OBJ_DIR):
	mkdir -p $@


clean:
	@$(RM) -rfv \
		$(OBJ_DIR)\
		$(BIN_DIR)
