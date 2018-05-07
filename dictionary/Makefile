.PHONY: all clean
.RECIPEPREFIX +=

#Variables copied from task4, standard as seen in lab
CC = gcc
CFLAGS = -O2 -Wall -Wextra -g -fPIC -c
RM = rm -f

SRCS = dictionary.c mock_trie.c
OBJS = $(SRCS:.c=.o)

#Resulting binary from makefile
BINS = autocomplete_dict

#Flags location for header files
IFLAGS = -I./include/

all: $(BINS)

clean:
  $(RM) $(BINS)

$(BINS): $(SRCS)
  $(CC) $(IFLAGS) $^ -o $(BINS)

test:
