CXXFLAGS = --std=c++11 -O2 -Wall $(DEBUGFLAGS)
LDFLAGS = -lasound
DEBUGFLAGS = 

.PHONY : all

SRCS = friend-alsa-capture.cc friend-alsa-playback.cc friend-exceptions.cc \
			 friend-filter-input.cc friend-filter-main.cc friend-filter-output.cc \
			 friend-flac-decoder.cc friend-flac-encoder.cc

OBJS = $(SRCS:.cc=.o)

all: friend-ladder-main

friend-ladder-main: $(OBJS)

friend-alsa-capture.o: friend-defaults.h
friend-alsa-playback.o: friend-defaults.h
