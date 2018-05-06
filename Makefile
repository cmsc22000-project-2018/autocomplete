.PHONY: all clean
.RECIPEPREFIX +=

#Variables copied from task4, standard as seen in lab
CC = gcc
CFLAGS = -O2 -Wall -Wextra -g -fPIC -c
RM = rm -f

SRCS = /src/autocomplete.c /src/parser.c /src/prefix.c
OBJS = $(SRCS:.c=.o)

#Resulting binary from makefile
BINS = autocomplete

#Flags location for header files
IFLAGS = -I/include/

all: $(BINS)

#Standard as in lab, also seen in libgeometry
clean:
  $(RM) $(OBJS) $(BINS)

$(OBJS): $(SRCS)
  $(CC) -c $(IFLAGS) $^

$(BINS): $(OBJS)
  $(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

test:
