# Tests

We have written tests for:

- C++ code, using [QtTest](https://doc.qt.io/qt-5/qttest-index.html)

    These tests can be run by:
    ```sh
    make cpp
    ```
    This will suppress CMake build messages. If you wish to see the messages
    (for example if the build fails), run `make build`.

- CLI, using [BATS](https://github.com/bats-core/bats-core)

    These tests can be run by:
    ```sh
    make -C ..
    make cli
    ```
    Note: *The first line builds the `devconf` executable.*

All tests can be run at once with the following commands:
```sh
make -C ..
make
```

