# Smart home device configurator

<p align="center">
  <a href="https://asciinema.org/a/437748" target="_blank">
    <img src="https://gist.github.com/veracioux/732e6f374ea0fbed2f4694a4c2a75f43/raw/17bfb84ca18acfc8c335c90c23fa4978c0a75696/devconf-demo.svg" />
  </a>
</p>


Here's how to use it:

- Fill out a few JSON files with a basic device description
- Generate template C++ code in an mbed workspace
- Implement some C++ functions
- Compile and download to the microcontroller

**Features:**

- JSON files can be used by both the smart device and the smart home system
  - this is provided by the library `devlib`
- Uses the MQTT protocol
  - you only have to implement the device features
- Uses Mbed OS 6
- Tested on Nucleo F746ZG

All of this is provided by the `devconf` program. Below you can find
instructions on how to build it.

**Note:** tested on Linux only.

## Documentation

The complete documentation can be found [here](https://siau-devconf.rtfd.io).

## Dependencies

- CMake >= 3.10
- Qt >= 5.9
  - qt5-mqtt
- make
- python3, docutils (to build the manual)

**Optional:**

- [mbed-cli](https://pypi.org/project/mbed-cli/) if you want to generate an mbed workspace automatically

## Build

Run the following commands:

    git clone --recurse-submodules 'https://github.com/veracioux/siau-devconf'
    make

This will create the program `devconf` and the man page in `_build/`
Alternatively, you can call:

    make app

and

    make man

to build them separately.

**NOTE:** `devconf` built this way must be called only from the project root
directory, otherwise it won't work correctly.

You can also compile the program directly from `src/CMakeLists.txt`.

## Installation

To install `devconf` on your system run:

    make install PREFIX=<DIR>

**NOTE:** `<DIR>` **must be an absolute path. This is essential!**

If you just run `make install` without giving the `PREFIX` variable, the program
shall be installed under `/usr/local`. In that case you will need root
permissions.
