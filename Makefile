.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)

LANG =\
	swedish

OBJ =\
	libnumtext_num2text.o\
	libnumtext_remove_separators.o\
	$(LANG:=.o)

HDR =\
	libnumtext.h\
	common.h

TEST =\
	$(LANG:=.test)

TEST_OBJ =\
	$(TEST:=.o)


all: libnumtext.a
$(OBJ): $(HDR)
$(OBJ:.o=.lo): $(HDR)
$(TEST_OBJ): libnumtext.h
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

check: $(TEST)
	@set -e &&\
	for t in $(TEST); do\
		printf './%s\n' "$$t";\
		"./$$t" || exit 1;\
	done

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
.SUFFIXES: .lo .o .c .test .test.o

.PHONY: all check install uninstall clean
