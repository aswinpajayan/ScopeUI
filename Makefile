CC     = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0 gmodule-export-2.0 glib-2.0`
LIBS   = `pkg-config --libs gtk+-3.0 gmodule-2.0 glib-2.0`
DEBUG  = -Wall -g

OBJECTS = scopeUI.o callbacks.o

.PHONY: clean

all: ui

ui: $(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) $(LIBS) -o $@
scopeUI.o: scopeUI_main.c support.h
	$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@
callbacks.o: callbacks.c support.h
	$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o ui


run:
	./ui
