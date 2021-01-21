pkgname=siau-dev-conf
pkgver=0.0.1
pkgrel=1
pkgdesc='Konfigurator uređaja za SIAU'
arch=('x86_64')
url='https://github.com/harisgusic/siau-project'
license=('TBD')
# TODO figure out what to do with dxflib dependency
depends=('jsoncpp')
makedepends=('git' 'cmake' 'python' 'python-docutils')

source=("$pkgname::git+https://github.com/harisgusic/siau-project")
sha256sums=('SKIP')

package() {
	cd "$srcdir/$pkgname"
	make install INSTALL_DIR="$pkgdir"/usr REAL_INSTALL_DIR="/usr"
}
