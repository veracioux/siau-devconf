
test:
	@${MAKE} build 1>/dev/null 2>/dev/null
	@_build/unittests

build:
	cmake -S . -B _build
	cmake --build _build

clean:
	rm -rf _build _out
