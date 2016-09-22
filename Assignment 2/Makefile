CC = g++
LIBS = -lGL -lglut -lGLU -lm 

SRC = draw.cpp
BIN = draw

all	:	$(BIN)

$(BIN)	:	Objects.hpp Objects.cpp $(SRC) 
	$(CC) $(SRC) Objects.cpp -o $(BIN) $(LIBS)

clean	:
	rm -f $(BIN) *~ *.aux *.log
