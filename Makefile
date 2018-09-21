CXXFLAGS = --std=c++11 -O2 -Wall
LDFLAGS = -lasound -lFLAC++

.PHONY : all

SRCS = friend-alsa-capture.cc friend-alsa-playback.cc friend-flac-decoder.cc \
			 friend-flac-encoder.cc friend-input.cc friend-output.cc
OBJS = $(SRCS:.c=.o)

all: friend-dummy-main

friend-dummy-main: $(OBJS)
