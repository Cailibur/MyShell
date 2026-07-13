CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = MyShell

SRCS = SH.c prompt.c builtin.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

SH.o: SH.c prompt.h
	$(CC) $(CFLAGS) -c SH.c

prompt.o: prompt.c prompt.h
	$(CC) $(CFLAGS) -c prompt.c

builtin.o: builtin.c builtin.h
	$(CC) $(CFLAGS) -c builtin.c

clean:
	rm -f $(OBJS) $(TARGET)