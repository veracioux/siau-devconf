BATS = bats --formatter pretty
export PROJECT_DIR = $(shell realpath "$$PWD"/..)
export DEVCONF_TEMPLATE_DIR = ${PROJECT_DIR}/template
export PATH = $(shell echo "${PROJECT_DIR}/_build/devconf:$$PATH")

.PHONY: all cli

# All tests
all: prepare
	@${MAKE} build 1>/dev/null 2>/dev/null
	@echo "\n\033[1;35mRunning C++ tests\033[0m\n"
	@ctest --test-dir _build/
	@echo "\n\033[1;35mRunning CLI tests\033[0m\n"
	@${BATS} cli/devconf.bats

cli: prepare
	@echo "\n\033[1;35mRunning CLI tests\033[0m\n"
	@${BATS} cli/devconf.bats

build:
	mkdir -p _build
	cp -r files _build/
	cmake -S . -B _build
	cmake --build _build

clean:
	rm -rf _build _out

# Helper targets
prepare:
	@rm -rf _out/
	@mkdir _out
