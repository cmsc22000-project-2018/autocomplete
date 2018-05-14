.PHONY: all clean
.RECIPEPREFIX +=

#Variables copied from task4, standard as seen in lab
CC = gcc
CFLAGS = -O2 -Wall -Wextra -g -fPIC -c
RM = rm -f

SRCS = ./src/autocomplete.c ./src/parser.c ./src/prefix.c ./src/dictionary.c ./src/mock_trie.c
OBJS = $(SRCS:.c=.o)

#Resulting binary from makefile
BINS = autocomplete

#Flags location for header files
IFLAGS = -I./include/

all: $(BINS)

clean:
  $(RM) $(BINS)

$(BINS): $(SRCS)
  $(CC) $(IFLAGS) $^ -o $(BINS)

test:
