BATS = bats --formatter pretty

test:
	@${MAKE} build 1>/dev/null 2>/dev/null
	@ctest --test-dir _build/

test-cli:


build:
	mkdir -p _build
	cp -r files _build/
	cmake -S . -B _build
	cmake --build _build

clean:
	rm -rf _build _out
