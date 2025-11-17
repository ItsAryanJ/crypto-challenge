CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c \
      src/menu.c \
      src/challenge.c \
      src/utils.c \
      src/leaderboard.c \
      src/caesar.c \
      src/vigenere.c \
      src/playfair.c \
      src/hill.c \
      src/transposition.c

OBJ = $(SRC:.c=.o)

TARGET = crypto_challenge

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	del /Q src\*.o
	del /Q $(TARGET).exe 2>nul

