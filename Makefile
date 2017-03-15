CC=g++
CFLAGS=-Wall -std=c++11
LDFLAGS=
LIBS=-lsfml-system -lsfml-window -lsfml-graphics -lsfml-network -lsfml-audio -lpthread
INC=/home/owner/Documents/Development/git/SMFL/include
TARGET=sfml-streaming
OUT=main

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(OUT) $(TARGET).cpp -I $(INC) $(LIBS)

clean:
	$(RM) $(OUT)






