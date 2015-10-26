all: lumeus

lumeus: main.c
	@$(CC) main.c -o lumeus

clean:
	@rm -f lumeus

install: all
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@cp -f lumeus $(DESTDIR)$(PREFIX)/bin
	@chown root $(DESTDIR)$(PREFIX)/bin/lumeus
	@chmod a+xs $(DESTDIR)$(PREFIX)/bin/lumeus

uninstall:
	@rm -f $(DESTDIR)$(PREFIX)/bin/lumeus

.PHONY: all clean uninstall
