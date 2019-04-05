CC     = gcc
CFLAGS = `pkg-config --cflags gtk+-2.0 gmodule-2.0`
LIBS   = `pkg-config --libs   gtk+-2.0 gmodule-2.0`
DEBUG  = -Wall -g

OBJECTS = scopeUI.o callbacks.o

.PHONY: clean

all: start_ui

start_ui: $(OBJECTS)
    $(CC) $(DEBUG) $(LIBS) $(OBJECTS) -o $@

scopeUI.o: scopeUI_main.c support.h
    $(CC) $(DEBUG) $(CFLAGS) -c $< -o $@

callbacks.o: callbacks.c support.h
    $(CC) $(DEBUG) $(CFLAGS) -c $< -o $@

clean:
    rm -f *.o start_ui


run:
	./start_ui
