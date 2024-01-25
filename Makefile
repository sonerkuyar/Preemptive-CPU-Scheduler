CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# List of source files
SRCS = src/main.c src/process.c src/ready_queue.c src/arrival_queue.c

# List of object files
OBJS = $(patsubst %.c,%.o,$(SRCS))

# Executable name
EXECUTABLE = scheduler

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLE)
