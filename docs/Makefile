# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = .
BUILDDIR      = _build

# Put it first so that "make" without argument is like "make help".
help:
	@${SPHINXBUILD} -M help "${SOURCEDIR}" "${BUILDDIR}" ${SPHINXOPTS} ${O}

.PHONY: help doxygen man mv-files

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  ${O} is meant as a shortcut for ${SPHINXOPTS}.
html: doxygen prepare-man mv-files devlib
	@${SPHINXBUILD} -M $@ "${SOURCEDIR}" "${BUILDDIR}" ${SPHINXOPTS} ${O}

doxygen:
	@mkdir -p _build/doxygen
	@doxygen

man:
	@${SPHINXBUILD} -M man "${SOURCEDIR}/man" "${BUILDDIR}" ${SPHINXOPTS} ${O}

# Tweak manpage sources for inclusion in the HTML version of the docs.
# A NAME section containing the description is added to each file from man/.
# The reason why the NAME section is excluded from the source files in the first
# place is because `sphinx-build -M man` automatically creates that section.
prepare-man: man
	@echo Converting manpages back to rst...
	@mkdir -p _intermediate/man
	@## Group (in same subshell)
	@DEST=_intermediate/man/dev-conf.rst; \
	echo ":tocdepth: 1" > $$DEST; echo >> $$DEST; \
	echo ".. _manpage:" >> $$DEST; echo >> $$DEST; \
	echo ======== >> $$DEST; echo dev-conf >> $$DEST; echo ======== >> $$DEST; \
	echo NAME >> $$DEST; echo ==== >> $$DEST; echo >> $$DEST; echo >> $$DEST; \
	echo "    dev-conf - Komanda za generisanje C++ predloška za MBED" >> $$DEST; \
	echo >> $$DEST; \
	cat man/dev-conf.rst >> $$DEST
	@## End-group

devlib:
	mkdir -p _intermediate/devlib
	cd ../devlib/docs/; \
		make doxygen;
	cp ../devlib/docs/*.rst _intermediate/devlib/
	cp -r ../devlib/docs/_build/doxygen _intermediate/devlib/

mv-files:
	mkdir -p _build/files
	cp ../template/* ../dev-conf/symbols.h.in ../devices/SmartLight.json \
		_build/files/

clean:
	rm -rf _build/ _intermediate/
