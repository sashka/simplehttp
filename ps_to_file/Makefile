LIBEVENT ?= /usr/local
TARGET ?= /usr/local
LIBSIMPLEHTTP ?= ../simplehttp
LIBSIMPLEHTTP_INC ?= $(LIBSIMPLEHTTP)/..
LIBSIMPLEHTTP_LIB ?= $(LIBSIMPLEHTTP)
LIBPUBSUBCLIENT ?= ../pubsubclient

CFLAGS = -I. -I$(LIBSIMPLEHTTP_INC) -I$(LIBEVENT)/include -g
LIBS = -L. -L$(LIBSIMPLEHTTP_LIB) -L$(LIBPUBSUBCLIENT) -L$(LIBEVENT)/lib -levent -lpubsubclient -lsimplehttp -lm

all: ps_to_file

ps_to_file: ps_to_file.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

install:
	/usr/bin/install -D ps_to_file $(TARGET)/bin/ps_to_file

clean:
	rm -f *.o *.a ps_to_file
