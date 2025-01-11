CC = g++
CFLAGS = -Iinclude -Wall -g
LDFLAGS = -lSDL2 -lGL -lGLEW

SRC = src/main.cpp src/sdl.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = opengl-app

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

run: all
	./$(EXEC)