CC = gcc
CFLAGS = -Wall
LIBNAME = libmylib.a
LIBS = -lmicrohttpd

INCDIRS := ./inc
SRCDIRS := ./src

INCLUDE := $(patsubst %, -I%, $(INCDIRS))

OBJDIR = ./obj

CFILES := $(wildcard $(addsuffix /*.c, $(SRCDIRS))) run/main.c
OBJFILES := $(patsubst %.c, $(OBJDIR)/%.o, $(CFILES))

.PHONY: all clean CrossNex

all: CrossNex

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

CrossNex: $(OBJFILES)
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)

clean:
	rm -rf $(OBJDIR)/* CrossNex
