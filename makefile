NAME='cmind'
PREFIX='/usr'
TEMPDIR := $(shell mktemp -u --suffix .$(cmind))

cmind: src/$(NAME)

install:
	install -Dm755 src/$(NAME) $(INSTALLDIR)/bin/$(NAME)
	install -Dm644 LICENSE $(PREFIX)/share/licenses/$(NAME)/LICENSE

uninstall:
	rm -f $(INSTALLDIR)/bin/$(NAME)
	rm -f $(PREFIX)/share/licenses/$(NAME)/LICENSE

clean:
	rm -f src/$(NAME)

togit:
	git add .
	git commit -m "Updated from makefile"
	git push origin
