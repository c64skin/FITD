EFENCE = -lefence

OBJECTS = cosTable.o fileAccess.o input.o main.o osystemSDL.o pak.o renderer.o \
screen.o tatou.o threadCode.o unpack.o vars.o videoMode.o
TARGETS = AITDrecode
AITD_LDFLAGS = `sdl-config --libs` -g $(EFENCE)
CXXFLAGS = `sdl-config --cflags` -g -Wall
CXX = g++
CC = gcc

all: $(TARGETS)

AITDrecode: $(OBJECTS)
	$(CXX) -o AITDrecode $(OBJECTS) $(AITD_LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGETS)

rebuild: clean all
