CC=gcc
SOURCES=main.c
OPENMP=-fopenmp
EXEC=a8

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(SOURCES) $(OPENMP) -o $@

clean: 
	rm $(EXEC)