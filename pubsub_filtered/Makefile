LIBEVENT ?= /usr/local
TARGET ?= /usr/local
LIBSIMPLEHTTP ?= ../simplehttp
LIBSIMPLEHTTP_INC ?= $(LIBSIMPLEHTTP)/..
LIBSIMPLEHTTP_LIB ?= $(LIBSIMPLEHTTP)

CFLAGS = -I. -I$(LIBSIMPLEHTTP_INC) -I$(LIBEVENT)/include -g -O2
LIBS = -L. -L$(LIBSIMPLEHTTP_LIB) -L$(LIBEVENT)/lib -levent -lsimplehttp -ljson -lpcre -lm

pubsub_filtered: pubsub_filtered.c md5.c
	$(CC) $(CFLAGS) -o $@ md5.c $< $(LIBS)

install:
	/usr/bin/install -d $(TARGET)/bin/
	/usr/bin/install pubsub_filtered $(TARGET)/bin/

clean:
	rm -rf *.o pubsub_filtered *.dSYM
