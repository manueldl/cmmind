![retrosmart-preview](https://raw.githubusercontent.com/mdomlop/cmind/master/preview.png "cmind running")

CMind
=====

Guess the secret code. (Mastermind®-type console game)

[Mastermind](https://en.wikipedia.org/wiki/Mastermind_(board_game)) is a logic
game. This is an C implementation of such game, realized as a
personal practice while I was trying to learn some of C
programming language.

Install
-------
You can install directly to your system. Default target directory is `/usr/local/bin/`. Change PREFIX variable to `/usr` if you want to.

### Classic mode:

	make && make install

or

	make && make install PREFIX=/usr


### Packaging:

#### pacman

Maybe you can install from [AUR](https://aur.archlinux.org/packages/cmind).
Or build the package it by hand:

	make pkg_arch

#### dpkg

To build a `.deb`:

	make pkg_debian

#### Windows executable

It is possible to build a MS Windows _exe_:

	make exe

#### Termux

To build a [Termux](https://termux.dev) package:

	make pkg_termux

Note that is necessary to build **under** Termux environmet, otherwise the
binary don't work.
