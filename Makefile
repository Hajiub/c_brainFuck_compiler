CC = gcc
CFLAGS = -Wall -Wextra
TARGET = main.c
EXEC = out

all:
	$(CC) $(CFLAGS) $(TARGET) -o $(EXEC)

clean:
	rm -rf $(EXEC)


run:
	./$(EXEC)

