#!/usr/bin/env python3

man_descriptions = {
    "devconf": "Komanda za generisanje C++ predloška za MBED",
}


if __name__ == "__main__":
    import sys

    print(man_descriptions[sys.argv[1]])
