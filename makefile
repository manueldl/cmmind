NAME='cmind'
PREFIX='/usr'
TEMPDIR := $(shell mktemp -u --suffix .$(cmind))

cmind: src/cmind

install:
	install -Dm755 src/$(NAME) $(PREFIX)/bin/$(NAME)
	install -Dm644 LICENSE $(PREFIX)/share/licenses/$(NAME)/LICENSE

uninstall:
	rm -f $(INSTALLDIR)/bin/$(NAME)
	rm -f $(PREFIX)/share/licenses/$(NAME)/LICENSE

clean:
	rm -f src/$(NAME)

togit: clean
	git add .
	git commit -m "Updated from makefile"
	git push origin

pacman:
	mkdir $(TEMPDIR)
	cp packages/pacman/PKGBUILD $(TEMPDIR)/
	cd $(TEMPDIR); makepkg -dr
	cp $(TEMPDIR)/$(NAME)-*.pkg.tar.xz packages/pacman/
	rm -rf $(TEMPDIR)
	@echo Package done!
	@echo Package is in `pwd`/packages/pacman/
