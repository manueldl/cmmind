NAME='cmind'
PREFIX='/usr'
TEMPDIR := $(shell mktemp -u --suffix .$(cmind))

cmind:
	cc -march=native -mtune=native -O2 src/cmind.c -o src/cmind

install:
	install -Dm 755 src/$(NAME) $(PREFIX)/bin/$(NAME)
	install -Dm 644 LICENSE $(PREFIX)/share/licenses/$(NAME)/LICENSE
	install -Dm 644 README.md $(PREFIX)/share/doc/$(NAME)/README

uninstall:
	rm -f $(PREFIX)/bin/$(NAME)
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
