
INSTALL_DIR ?= /usr/local

# This is the real directory where the program will be installed.  It should
# only differ from INSTALL_DIR if the program is being packaged for distribution
# (e.g. using a package manager). If the user doesn't set this variable through
# the command-line, it will be set equal to INSTALL_DIR.
REAL_INSTALL_DIR ?= ""

CONFIG ?= Release
CMAKE = cmake -DCMAKE_BUILD_TYPE=${CONFIG} -S dev-conf/ -B _build/
CMAKE_BUILD = cmake --build _build/

ifeq ($(REAL_INSTALL_DIR), "")
	REAL_INSTALL_DIR = $(INSTALL_DIR)
endif

.PHONY: app man docs

all: app man

app:
	mkdir -p _build/
	$(CMAKE) && $(CMAKE_BUILD)

man:
	mkdir -p _build/
	rst2man docs/man.rst _build/dev-conf.1.gz

docs:
	cd docs/ && make html

install: man
	mkdir -p _build/
	$(CMAKE) -DTEMPLATE_DIR="$(REAL_INSTALL_DIR)/share/dev-conf" \
		&& $(CMAKE_BUILD)
	mkdir -p "$(INSTALL_DIR)"/{bin,share/dev-conf,share/man/man1}
	cp ./_build/dev-conf "$(INSTALL_DIR)"/bin/
	cp ./_build/dev-conf.1.gz "$(INSTALL_DIR)"/share/man/man1/
	cp -r ./template/* "$(INSTALL_DIR)"/share/dev-conf/

uninstall:
	rm -rf "$(INSTALL_DIR)"/share/dev-conf/ \
		"$(INSTALL_DIR)"/bin/dev-conf \
		"$(INSTALL_DIR)"/share/man/man1/dev-conf.1.gz

clean:
	rm -rf _build/ dev-conf/_build devlib/_build
