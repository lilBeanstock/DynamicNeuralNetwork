SRC = main.c
HDR_DIR = ./src
HDR = $(HDR_DIR)/*.h
TARGET = main

all: $(TARGET)

clean:
	rm -rf main

$(TARGET): $(SRC) $(HDR)
	gcc -Wall -Wextra -O2 -o $@ $(SRC) -I $(HDR_DIR)