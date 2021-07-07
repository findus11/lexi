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

all: make_dirs $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(FLAGS) $^ -o $@

$(ODIR)/%.o: $(SDIR)/%.c $(INCLUDE)
	$(CC) $(CFLAGS) $(FLAGS) -I$(IDIR) -c $< -o $@

.PHONY: clean make_dirs

clean:
	rm -f $(ODIR)/*.o *~ $(NAME)

make_dirs:
	mkdir -p $(ODIR)

