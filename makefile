CC=gcc
CFLAGS=-I.

DEPS = ProcessStruct.h
OBJ = MyFDViewer.o ProcessStruct.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

MyFDViewer: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o MyFDViewer
