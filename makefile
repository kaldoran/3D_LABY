CC = gcc
CFLAGS = -W -Wall -Wextra -pedantic -lGL -lGLU -lSDL -lSDL_image -ldl -lm

#
# Repositories.
 #
OBJ_REP = objets
INC_REP = includes
SRC_REP = sources
BIN_REP = bin

#
# Sources, objects and binary file name
 #
SRC = $(foreach dir, $(SRC_REP), $(wildcard $(dir)/*.c))
OBJ = $(addsuffix .o, $(basename $(subst ${SRC_REP}, ${OBJ_REP}, ${SRC})))
BIN = main

.PHONY: clean mrproper clear dir
.SUFFIXES:
	

all: clear dir $(BIN_REP)/$(BIN)

#
# BIN creation.
 #
$(BIN_REP)/$(BIN): $(OBJ)
	@$(CC) $(OBJ) -o $@ -I $(INC_REP) $(CFLAGS) 
#
# Object files creation.
 #
$(OBJ_REP)/%.o: $(SRC_REP)/%.c
	@$(CC) -c $^ -o $@ -I $(INC_REP) $(CFLAGS) 

#
# Other usefull targets.
 #
clean:
	rm -f ./$(OBJ_REP)/*.o;
	rm -f output

mrproper:
	rm -f ./$(OBJ_REP)/*.o ./$(BIN_REP)/$(BIN);
	rm -f output

rebuild: mrproper clear all

#
# Screen cleaning.
 #
clear:
	clear

#
# Repository creation.
 #
dir:
	@mkdir -p $(OBJ_REP)
	@mkdir -p $(BIN_REP)
