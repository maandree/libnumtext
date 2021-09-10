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

CMD =\
	card2ord\
	numtext-strip

LIB_OBJ =\
	libnumtext_card2ord.o\
	libnumtext_num2text.o\
	libnumtext_remove_separators.o\
	$(LANG:=.o)

HDR =\
	libnumtext.h\
	common.h

TEST =\
	$(LANG:=.test)

OBJ      = $(LIB_OBJ) $(CMD_OBJ)
LOBJ     = $(LIB_LOBJ)
CMD_OBJ  = $(CMD:=.o)
LIB_LOBJ = $(LIB_OBJ:.o=.lo)
TEST_OBJ = $(TEST:=.o)


all: libnumtext.a libnumtext.$(LIBEXT) numtext
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

numtext: numtext.o $(CMD_OBJ)
	$(CC) -o $@ $@.o $(CMD_OBJ) libnumtext.a $(LDFLAGS)

libnumtext.a: $(LIB_OBJ)
	@rm -f -- $@
	$(AR) rc $@ $(LIB_OBJ)

libnumtext.$(LIBEXT): $(LIB_LOBJ)
	$(CC) $(LIBFLAGS) -o $@ $(LIB_LOBJ) $(LDFLAGS)

check: $(TEST)
	@set -e &&\
	for t in $(TEST); do\
		printf './%s\n' "$$t";\
		"./$$t" || exit 1;\
	done

install: libnumtext.a libnumtext.$(LIBEXT) numtext
	mkdir -p -- "$(DESTDIR)$(PREFIX)/lib"
	mkdir -p -- "$(DESTDIR)$(PREFIX)/include"
	mkdir -p -- "$(DESTDIR)$(PREFIX)/bin"
	mkdir -p -- "$(DESTDIR)$(PREFIX)/$(LIBEXEC)"
	cp -- libnumtext.a "$(DESTDIR)$(PREFIX)/lib/"
	cp -- libnumtext.$(LIBEXT) "$(DESTDIR)$(PREFIX)/lib/libnumtext.$(LIBMINOREXT)"
	ln -sf -- libnumtext.$(LIBMINOREXT) "$(DESTDIR)$(PREFIX)/lib/libnumtext.$(LIBMAJOREXT)"
	ln -sf -- libnumtext.$(LIBMINOREXT) "$(DESTDIR)$(PREFIX)/lib/libnumtext.$(LIBEXT)"
	cp -- libnumtext.h "$(DESTDIR)$(PREFIX)/include/"
	cp -- numtext "$(DESTDIR)$(PREFIX)/$(LIBEXEC)/"
	set -e && for cmd in $(CMD); do\
		ln -sf -- "../$(LIBEXEC)/numtext" "$(DESTDIR)$(PREFIX)/bin/$$cmd" || exit 1;\
	done

uninstall:
	-rm -f -- "$(DESTDIR)$(PREFIX)/lib/libnumtext".*
	-rm -f -- "$(DESTDIR)$(PREFIX)/include/libnumtext.h"
	-cd -- "$(DESTDIR)$(PREFIX)/bin" && rm -- $(CMD)
	-rm -f -- "$(DESTDIR)$(PREFIX)/$(LIBEXEC)/numtext"

clean:
	-rm -f -- *.o *.a *.lo *.su *.so *.so.* *.gch *.gcov *.gcno *.gcda *.test *.dylib numtext

.SUFFIXES:
.SUFFIXES: .lo .o .c .test .test.o

.PHONY: all check install uninstall clean
