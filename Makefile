.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)

OBJ =\
	libnumtext_num2text.o\
	libnumtext_remove_separators.o\
	swedish.o

HDR =\
	libnumtext.h\
	common.h

all: libnumtext.a
$(OBJ): $(HDR)
$(OBJ:.o=.lo): $(HDR)

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

.c.lo:
	$(CC) -fPIC -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

libnumtext.a: $(OBJ)
	@rm -f -- $@
	$(AR) rc $@ $(OBJ)

install: libnumtext.a
	mkdir -p -- "$(DESTDIR)$(PREFIX)/lib"
	mkdir -p -- "$(DESTDIR)$(PREFIX)/include"
	cp -- libnumtext.a "$(DESTDIR)$(PREFIX)/lib/"
	cp -- libnumtext.h "$(DESTDIR)$(PREFIX)/include/"

uninstall:
	-rm -f -- "$(DESTDIR)$(PREFIX)/lib/libnumtext.a"
	-rm -f -- "$(DESTDIR)$(PREFIX)/include/libnumtext.h"

clean:
	-rm -f -- *.o *.a *.lo *.su *.so *.so.* *.gch *.gcov *.gcno *.gcda

.SUFFIXES:
.SUFFIXES: .lo .o .c

.PHONY: all install uninstall clean
