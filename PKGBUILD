# Maintainer: Manuel Domínguez López <mdomlop at gmail dot com>

_pkgver_year=2018
_pkgver_month=07
_pkgver_day=26

pkgname=cmind
pkgver=1.0b
pkgrel=1
pkgdesc='Simple console temperature monitor for Linux.'
arch=('i686' 'x86_64')
url='https://github.com/mdomlop/cmind'
source=()
license=('GPL3')

build() {
    cd $startdir
    make
    }

package() {
    cd $startdir
    make install DESTDIR=$pkgdir
}
