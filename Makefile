NAME := lexi

IDIR := include
SDIR := src
ODIR := obj

SOURCES := $(wildcard $(SDIR)/*.c)
INCLUDE := $(wildcard $(IDIR)/$(NAME)/*.h)
OBJECTS := $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SOURCES))

CC=clang
FLAGS=-fsanitize=undefined -g
CFLAGS=-Weverything -Wno-padded -std=c99 -O1
LDFLAGS=

all: make_dirs $(NAME).out

$(NAME).out: $(OBJECTS)
	$(CC) $(LDFLAGS) $(FLAGS) $^ -o $@

$(ODIR)/%.o: $(SDIR)/%.c $(INCLUDE)
	$(CC) $(CFLAGS) $(FLAGS) -I$(IDIR) -c $< -o $@

.PHONY: clean make_dirs

clean:
	rm -f $(ODIR)/*.o *~ $(NAME).out

make_dirs:
	mkdir -p $(ODIR)

