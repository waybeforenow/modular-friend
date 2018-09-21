CXXFLAGS = --std=c++11 -O2 -Wall $(DEBUGFLAGS)
LDFLAGS = -lasound -lFLAC++
DEBUGFLAGS = 

.PHONY : all

SRCS = friend-alsa-capture.cc friend-alsa-playback.cc friend-flac-decoder.cc \
			 friend-flac-encoder.cc friend-input.cc friend-output.cc \
			 friend-exceptions.cc
OBJS = $(SRCS:.cc=.o)

all: friend-dummy-main

friend-dummy-main: $(OBJS)

friend-alsa-capture.o: friend-defaults.h
friend-alsa-playback.o: friend-defaults.h
