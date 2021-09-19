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
	@${SPHINXBUILD} -M html "${SOURCEDIR}" "${BUILDDIR}"

doxygen:
	@mkdir -p _build/doxygen
	@doxygen

man:
	@${SPHINXBUILD} -M man "${SOURCEDIR}/man" "${BUILDDIR}" ${SPHINXOPTS} ${O}

# Tweak manpage sources for inclusion in the HTML version of the docs.
# A NAME section containing the description is added to each manpage source file.
# The reason why the NAME section is excluded from the source files in the first
# place is because `sphinx-build -M man` automatically creates that section.
prepare-man: man
	@echo Converting manpages back to rst...
	@mkdir -p _intermediate/man
	@for file in man/*.rst; do \
		[ "$$file" = "man/index.rst" ] && continue; \
		man/prepare-man.sh "$$file" _intermediate/man; \
	done


devlib:
	mkdir -p _intermediate/devlib/_build
	cd ../devlib/docs/; \
		make doxygen;
	cp ../devlib/docs/*.rst _intermediate/devlib/
	cp -r ../devlib/docs/_build/doxygen _intermediate/devlib/_build/
	mkdir -p _build/html/devlib/doxygen
	cp -r ../devlib/docs/_build/doxygen/html/* _build/html/devlib/doxygen/

mv-files:
	mkdir -p _build/files
	cp -r ../template/* ../devices/SmartLight.json \
		_build/files/

clean:
	rm -rf _build/ _intermediate/
