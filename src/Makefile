EFENCE = -lefence

OBJECTS = cosTable.o evalVar.o fileAccess.o hqr.o input.o life.o lines.o\
main.o osystemSDL.o osystemSDL_GL.o pak.o polys.o renderer.o \
screen.o tatou.o threadCode.o track.o unpack.o vars.o videoMode.o
TARGETS = AITDrecode
AITD_LDFLAGS = `sdl-config --libs` -DUNIX -lGL -lGLU -lSDL_sound -g $(EFENCE) 
CXXFLAGS = -O3 `sdl-config --cflags` -DUNIX -DUSE_GL -g -Wall
CXX = g++
CC = gcc

all: $(TARGETS)

AITDrecode: $(OBJECTS)
	$(CXX) -o AITDrecode $(OBJECTS) $(AITD_LDFLAGS)

optimized: $(OBJECTS)
	$(CXX) -O -o AITDopti $(OBJECTS) $(AITD_LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGETS)

rebuild: clean all
