CC = gcc
CFLAGS = -Wall
LIBNAME = libmylib.a

INCDIRS := sysinfo/cpu sysinfo/disk sysinfo/memory sysinfo/process
SRCDIRS := sysinfo/cpu sysinfo/disk sysinfo/memory sysinfo/process

INCLUDE := $(patsubst %, -I%, $(INCDIRS))

OBJDIR = ./obj

CFILES := $(wildcard $(addsuffix /*.c, $(SRCDIRS))) test/main.c
OBJFILES := $(patsubst %.c, $(OBJDIR)/%.o, $(CFILES))

.PHONY: all clean testrun

all: testrun

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

testrun: $(OBJFILES)
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

clean:
	rm -rf $(OBJDIR)/* testrun