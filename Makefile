PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man
CFLAGS += -O3 -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual \
          -Wstrict-prototypes -Wmissing-prototypes

all: lumeus

lumeus: lumeus.c
	@$(CC) ${CFLAGS} lumeus.c -o lumeus

clean:
	@rm -f lumeus

install: all
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@cp -f lumeus $(DESTDIR)$(PREFIX)/bin
	@chown root $(DESTDIR)$(PREFIX)/bin/lumeus
	@chmod a+xs $(DESTDIR)$(PREFIX)/bin/lumeus
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@cp -f lumeus.1 ${DESTDIR}${MANPREFIX}/man1/lumeus.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/lumeus.1

uninstall:
	@rm -f $(DESTDIR)$(PREFIX)/bin/lumeus ${DESTDIR}${MANPREFIX}/man1/lumeus.1

.PHONY: all clean uninstall
