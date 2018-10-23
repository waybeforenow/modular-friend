CXXFLAGS = --std=c++11 -O2 -Wall
LDFLAGS += -lasound -lm -lpthread -lstdc++

PREFIX ?= $(DESTDIR)/usr
BINDIR ?= $(PREFIX)/bin

.PHONY : all install clean

SRCS = friend-alsa-capture.cc friend-alsa-playback.cc friend-exceptions.cc \
			 friend-filter-input.cc friend-filter-main.cc friend-filter-output.cc

OBJS = $(SRCS:.cc=.o)

all: friend-filter-main

install: friend-filter-main
	install -d $(BINDIR)
	install -m 0755 friend-filter-main $(BINDIR)

clean:
	rm friend-filter-main || true
	rm $(OBJS) || true

friend-filter-main: $(OBJS)

friend-alsa-capture.o: friend-defaults.h
friend-alsa-playback.o: friend-defaults.h
