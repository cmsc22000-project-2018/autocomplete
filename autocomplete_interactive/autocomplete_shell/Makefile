# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jrameau <jrameau@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/12/13 11:43:23 by jrameau           #+#    #+#              #
#    Updated: 2017/05/21 14:58:05 by jrameau          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project file
NAME = autocomplete

# Project builds and dirs
SRCDIR = ./src/
SRCNAMES = $(shell ls $(SRCDIR) | grep -E ".+\.c")
SRC = $(addprefix $(SRCDIR), $(SRCNAMES))
INC = ./inc/
BUILDDIR = ./build/
BUILDOBJS = $(addprefix $(BUILDDIR), $(SRCNAMES:.c=.o))

# Libft builds and dirs
LIBDIR = ./libft/
LIBFT = ./libft/libft.a
LIBINC = ./libft/includes/

# Optimization and Compiler flags and commands
CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -L./api -Wl,-rpath,./api
LDLIBS = -lredisApi

# -Werror

# Debugging flags
DEBUG = -g

API = api

# Main rule
all: redis $(BUILDDIR) $(LIBFT) $(NAME)

redis:
	make -C api

# Object dir rule
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Objects rule
$(BUILDDIR)%.o:$(SRCDIR)%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(LIBINC) -I$(INC) -o $@ -c $<

# Project file rule
$(NAME): $(BUILDOBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(BUILDOBJS) $(LIBFT) -lncurses $(LDLIBS)

# Libft rule
$(LIBFT):
	make -C $(LIBDIR)

# Cleaning up the build files
clean:
	rm -rf $(BUILDDIR)
	make -C $(LIBDIR) clean
	make -C api clean

# Getting rid of the project file
fclean: clean
	rm -rf $(NAME)
	make -C $(LIBDIR) fclean
	make -C api clean

# Do both of the above
re: fclean all

# Just in case those files exist in the root dir
.PHONY: all fclean clean re
