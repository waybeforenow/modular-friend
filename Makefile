CXXFLAGS = --std=c++11 -O2 -Wall $(DEBUGFLAGS)
LDFLAGS = -lasound -lm -lpthread -lstdc++
DEBUGFLAGS =

.PHONY : all

SRCS = friend-alsa-capture.cc friend-alsa-playback.cc friend-exceptions.cc \
			 friend-filter-input.cc friend-filter-main.cc friend-filter-output.cc

OBJS = $(SRCS:.cc=.o)

all: friend-filter-main

friend-filter-main: $(OBJS)

friend-alsa-capture.o: friend-defaults.h
friend-alsa-playback.o: friend-defaults.h
