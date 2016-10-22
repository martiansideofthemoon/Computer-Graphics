CC = g++
LIBS = -lGL -lglut -lGLU -lm 

SRC = draw.cpp
BIN = draw

all	:	$(BIN)

$(BIN)	:	Assembly.hpp Assembly.cpp Objects.hpp Objects.cpp $(SRC) 
	$(CC) $(SRC) Assembly.cpp Objects.cpp -o $(BIN) $(LIBS)

clean	:
	rm -f $(BIN) *~ *.aux *.log
