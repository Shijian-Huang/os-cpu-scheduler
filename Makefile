CC = gcc
CFLAGS = -Wall
SRC = src/main.c src/fcfs.c src/rr.c src/sjf.c src/srt.c src/hpfp.c src/hpfnp.c src/process.c src/utility.c
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
TARGET = scheduler

# Rule to link object files into the final binary
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile .c to .o into the build/ folder
build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o $(TARGET)
