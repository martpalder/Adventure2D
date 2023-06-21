CC=gcc
CXX=g++
CFLAGS=-g -O0 #-Wall
INC=-I/usr/local/include
LIB=-L/usr/local/lib
LDFLAGS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
GAME=src/game.c
EDITOR=src/editor.c
SRC=src/engine/*.c
TARGET_GAME=./bin/game
TARGET_EDITOR=./bin/editor

game:
	${CC} ${CFLAGS} ${INC} ${GAME} ${SRC} -o ${TARGET_GAME} ${LIB} ${LDFLAGS} ${PLATOFORM}

editor:
	${CC} ${CFLAGS} ${INC} ${EDITOR} ${SRC} -o ${TARGET_EDITOR} ${LIB} ${LDFLAGS} ${PLATOFORM}

all:
	${CXX} ${CFLAGS} ${INC} ${GAME} ${SRC} -o ${TARGET_GAME} ${LIB} ${LDFLAGS} ${PLATOFORM}
	${CC} ${CFLAGS} ${INC} ${EDITOR} ${SRC} -o ${TARGET_EDITOR} ${LIB} ${LDFLAGS} ${PLATOFORM}

run:
	cd bin
	./game

clean:
	rm ${TARGET_GAME}
	rm ${TARGET_EDITOR}
