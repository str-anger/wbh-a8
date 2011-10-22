CC=gcc
SOURCES=main.c
EXEC=a8

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(SOURCES) -o $@

clean: 
	rm $(EXEC)