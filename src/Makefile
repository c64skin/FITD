EFENCE = -lefence

OBJECTS = fileAccess.o input.o main.o osystemSDL.o pak.o renderer.o \
screen.o tatou.o threadCode.o unpack.o vars.o videoMode.o
TARGET = AITDrecode
LDFLAGS = `sdl-config --libs` -g $(EFENCE)
CPPFLAGS = `sdl-config --cflags` -g -Wall
CXX = g++

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET)

rebuild: clean all
