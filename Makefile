SHELL := /bin/bash

# app name
name = app
name_release = $(name).release
name_debug = $(name).debug
src_path = ./src

# compiler config
CC = gcc
CFLAGS = -std=iso9899:1990 -ansi -Wall -Wextra -Werror -Wpedantic -I./includes
LDFLAGS = -I./includes

# sources and entry point
src = $(shell find $(src_path) -name *.c ! -name *_test.c ! -name main.c)
src_main = $(shell find $(src_path) -name main.c)
obj = $(src:.c=.o)
obj_main = $(src_main:.c=.o)

# test sources and entry point
src_test = $(shell find $(src_path) -name *_test.c)
obj_test = $(src_test:.c=.o)

# files to clean up
trash = $(obj) $(obj_main) $(obj_test) $(name_profraw) $(name_profdata) $(name_coverage_html) $(name_coverage_txt)

# default build
all: release

# release target
release: CFLAGS += -O3
release: $(name_release)
$(name_release): $(obj_main) $(obj)
	$(CC) $(LDFLAGS) -o $(name_release) $^

# debug target
debug: CFLAGS += -g
debug: LDFLAGS += -g
debug: $(name_debug)
$(name_debug): $(obj_main) $(obj)
	$(CC) $(LDFLAGS) -o $(name_debug) $^

# make object files from source code
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

# remove old build files
clean:
	rm -rf $(trash)

.ONESHELL:
