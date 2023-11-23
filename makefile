CC = gcc
CFLAGS = -Wall
LIBNAME = libmylib.a

INCDIRS := sysinfo/cpu sysinfo/disk sysinfo/memory sysinfo/process
SRCDIRS := sysinfo/cpu sysinfo/disk sysinfo/memory sysinfo/process

INCLUDE := $(patsubst %, -I%, $(INCDIRS))

OBJDIR = ./obj

# 获取每个子目录下的源文件列表
CFILES := $(wildcard $(addsuffix /*.c, $(SRCDIRS))) test/main.c
OBJFILES := $(patsubst %.c, $(OBJDIR)/%.o, $(CFILES))

.PHONY: all clean testrun

all: testrun

# 编译每个源文件成目标文件
$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

testrun: $(OBJFILES)
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

clean:
	rm -rf $(OBJDIR)/* testrun