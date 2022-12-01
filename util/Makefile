
CPP=g++

ROOT_DIR=.
SRC_DIR=$(ROOT_DIR)/src/
OBJ_DIR=$(ROOT_DIR)/obj/
BIN_DIR=$(ROOT_DIR)/bin/

SRC:=$(wildcard $(SRC_DIR)/*.cpp)
OBJ:=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

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

all: $(OBJ)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rfv \
		$(OBJ_DIR)
