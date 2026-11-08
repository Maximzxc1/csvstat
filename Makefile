CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
SRC = main.c reader.c number.c column.c output.c
HDR = reader.h number.h column.h output.h

csvstat: $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o csvstat $(SRC)