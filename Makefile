CC = gcc
WARNING = all extra pedantic
INC = include
SRC_FOLDERS = ./src

CFLAGS = -g

PKG = glib-2.0


C_VERSION = c99

INC_DIRS  = $(foreach d, $(INC), -I$d)
CFLAGS_FT = $(foreach d, $(WARNING), -W$d)

SRC 			= $(foreach d, $(SRC_FOLDERS), $d/*.c)
PKG_CONF  = $(foreach d, $(PKG), $(shell pkg-config --cflags --libs $d))
BIN = exec

all:
	mkdir -p ./bin
	$(CC) -o bin/$(BIN) $(INC_DIRS) $(CFLAGS_FT) $(SRC) -std=$(C_VERSION) $(CFLAGS) $(PKG_CONF)
