.PHONY = all clean debug

NAME = day8
CC = g++
INCDIR = ../bootcode/include
CFLAGS = -Wall -I$(INCDIR)
LDFLAGS = -lm -pthread
DEPS = $(INCDIR)/*.hpp

all: CFLAGS += -O3
all: $(NAME)


debug: CFLAGS += -g -DDEBUG
debug: $(NAME)

$(NAME): $(DEPS) $(NAME).cpp
	$(CC) $(CFLAGS) -o $@ $(NAME).cpp $(LDFLAGS)


clean:
	rm -f $(NAME)