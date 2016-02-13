AR?=ar
AS?=as
RANLIB?=ranlib
CC?=gcc
LD?=gcc
CFLAGS?=-Wall -Wextra -O3 -I./libbele
LDFLAGS?=

LIB=libsnoop.a

OFILES=\
	read.o\
	write.o\

HFILES=\
	snoop.h\

all: dep $(LIB)
$(LIB): $(OFILES) $(HFILES)
	$(AR) r $(LIB) $(OFILES)
	$(RANLIB) $(LIB)

%.o: %.c
	$(CC) -c $(CFLAGS) $*.c

clean:
	rm -f *.o

nuke: cleandep clean
	rm -f $(LIB)

libbele:
	git clone -q https://github.com/0intro/libbele

dep: libbele
	cd libbele; $(MAKE)

cleandep:
	rm -rf libbele
