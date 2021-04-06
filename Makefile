CPP      = g++
CC       = gcc
OBJ      = main.o XKBTools.o
LINKOBJ  = main.o XKBTools.o
CFLAGS = -Iinclude -std=c++17 -Wall -Wextra -g -c
LIBS = -lX11 -lxkbfile -lboost_filesystem -lboost_system -lpthread
BIN	 = xkbjson


.PHONY: all default build install

default: $(BIN)
all: default ;

install: $(BIN)
	mkdir -p $(DESTDIR)usr/bin && cp $(BIN) $(DESTDIR)usr/bin/$(BIN)

build: $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

./%.o: ./%.cpp
	$(CPP) $(CFLAGS) $< -o $@ $(LIBS)

.PHONY : clean
clean : 
	-rm -rf $(OBJ) $(BIN) usr/