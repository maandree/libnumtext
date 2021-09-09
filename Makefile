.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)

OS = linux
# linux = Linux
# macos = Mac OS
include $(OS).mk

LIB_MAJOR = 1
LIB_MINOR = 0
LIB_VERSION = $(LIB_MAJOR).$(LIB_MINOR)


LANG =\
	swedish

OBJ =\
	libnumtext_card2ord.o\
	libnumtext_num2text.o\
	libnumtext_remove_separators.o\
	$(LANG:=.o)

LOBJ =\
	$(OBJ:.o=.lo)

HDR =\
	libnumtext.h\
	common.h

TEST =\
	$(LANG:=.test)

TEST_OBJ =\
	$(TEST:=.o)


all: libnumtext.a libnumtext.$(LIBEXT)
$(OBJ): $(HDR)
$(LOBJ): $(HDR)
$(TEST_OBJ): $(HDR)
$(TEST): libnumtext.a

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

.c.lo:
	$(CC) -fPIC -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

.test.o.test:
	$(CC) -o $@ $< libnumtext.a $(LDFLAGS)

libnumtext.a: $(OBJ)
	@rm -f -- $@
	$(AR) rc $@ $(OBJ)

libnumtext.$(LIBEXT): $(LOBJ)
	$(CC) $(LIBFLAGS) -o $@ $(LOBJ) $(LDFLAGS)

check: $(TEST)
	@set -e &&\
	for t in $(TEST); do\
		printf './%s\n' "$$t";\
		"./$$t" || exit 1;\
	done

install: libnumtext.a libnumtext.$(LIBEXT)
	mkdir -p -- "$(DESTDIR)$(PREFIX)/lib"
	mkdir -p -- "$(DESTDIR)$(PREFIX)/include"
	cp -- libnumtext.a "$(DESTDIR)$(PREFIX)/lib/"
	cp -- libnumtext.$(LIBEXT) "$(DESTDIR)$(PREFIX)/lib/libnumtext.$(LIBMINOREXT)"
	ln -sf -- libnumtext.$(LIBMINOREXT) "$(DESTDIR)$(PREFIX)/lib/libnumtext.$(LIBMAJOREXT)"
	ln -sf -- libnumtext.$(LIBMINOREXT) "$(DESTDIR)$(PREFIX)/lib/libnumtext.$(LIBEXT)"
	cp -- libnumtext.h "$(DESTDIR)$(PREFIX)/include/"

uninstall:
	-rm -f -- "$(DESTDIR)$(PREFIX)/lib/libnumtext".*
	-rm -f -- "$(DESTDIR)$(PREFIX)/include/libnumtext.h"

clean:
	-rm -f -- *.o *.a *.lo *.su *.so *.so.* *.gch *.gcov *.gcno *.gcda *.test *.dylib

.SUFFIXES:
.SUFFIXES: .lo .o .c .test .test.o

.PHONY: all check install uninstall clean
