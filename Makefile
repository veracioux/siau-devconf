
PREFIX ?= /usr/local

SHELL=bash
CONFIG ?= Release
CMAKE = cmake -DCMAKE_BUILD_TYPE=${CONFIG} -S src/ -B _build/
CMAKE_BUILD = cmake --build _build/

.PHONY: app man docs

all: app man

app:
	mkdir -p _build/
	$(CMAKE) && $(CMAKE_BUILD)

man:
	mkdir -p _build/
	rst2man docs/man/devconf.rst _build/devconf.1.gz

install: man
	mkdir -p _build/
	$(CMAKE) -DTEMPLATE_DIR="$(REAL_INSTALL_DIR)/share/devconf" \
		&& $(CMAKE_BUILD)
	mkdir -p "$(DESTDIR)/$(PREFIX)"/{bin,share/devconf,share/man/man1}
	cp ./_build/devconf "$(DESTDIR)/$(PREFIX)"/bin/
	cp ./_build/devconf.1.gz "$(DESTDIR)/$(PREFIX)"/share/man/man1/
	cp -r ./template/* "$(DESTDIR)/$(PREFIX)"/share/devconf/

uninstall:
	rm -rf "$(DESTDIR)/$(PREFIX)"/share/devconf/ \
		"$(DESTDIR)/$(PREFIX)"/bin/devconf \
		"$(DESTDIR)/$(PREFIX)"/share/man/man1/devconf.1.gz

clean:
	rm -rf _build/ devlib/_build

clean-all:
	$(MAKE) clean
	cd docs/ && $(MAKE) clean
