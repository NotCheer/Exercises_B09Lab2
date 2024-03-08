CC=gcc
CFLAGS=-g -I.

DEPS = ProcessStruct.h TableDisplay.h
OBJ = MyFDViewer.o ProcessStruct.o TableDisplay.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

MyFDViewer: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o MyFDViewer
