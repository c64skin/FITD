EFENCE = -lefence

OBJECTS = cosTable.o evalVar.o fileAccess.o hqr.o input.o life.o lines.o\
main.o osystemSDL.o pak.o polys.o renderer.o \
screen.o tatou.o threadCode.o track.o unpack.o vars.o videoMode.o
TARGETS = AITDrecode
AITD_LDFLAGS = `sdl-config --libs` -DUNIX -lSDL_sound -g $(EFENCE)
CXXFLAGS = `sdl-config --cflags` -DUNIX -g -Wall
CXX = g++
CC = gcc

all: $(TARGETS)

AITDrecode: $(OBJECTS)
	$(CXX) -o AITDrecode $(OBJECTS) $(AITD_LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGETS)

rebuild: clean all
