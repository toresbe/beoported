CC=cc
CXX=g++
RM=rm -f
CPPFLAGS=-g -fPIC -std=gnu++11 -Wall -Wextra -Ilibpc2 -I. -DBOOST_LOG_DYN_LINK $(shell pkg-config --cflags libusb-1.0 libnotify) 
LDFLAGS=-g
LDLIBS=$(shell pkg-config --libs libusb-1.0 libnotify) -lrabbitmq -lboost_iostreams -lboost_thread -lboost_system -lpthread -lboost_log_setup -lboost_log

PROG=pc2d
SRCS=$(wildcard *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

all:    $(PROG)

libpc2/libpc2.a:
	$(MAKE) -C libpc2/

pc2d: libpc2/libpc2.a $(OBJS) 
	$(CXX) $(LDFLAGS) -o $(PROG) $(OBJS) $< $(LDLIBS)

install-deps:
	apt install libboost-log-dev libboost-iostreams-dev libboost-thread-dev libboost-system-dev librabbitmq-dev libusb-1.0-0-dev libnotify-dev

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) pc2

install: pc2d
	install -m 644 etc/pc2.service /etc/systemd/system/
	install -m 644 services/pc2_hue.service /etc/systemd/system/
	install -m 644 services/pc2_projector.service /etc/systemd/system/
	install -m 755 pc2d /usr/local/bin
	install -m 755 services/pc2_hue /usr/local/bin
	install -m 755 services/pc2_projector /usr/local/bin
	install -m 644 etc/50-pc2.rules /etc/udev/rules.d
