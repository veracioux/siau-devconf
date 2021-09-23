========================
Razvoj programa devconf
========================

.. toctree::

Definirane funkcije
===================

Datoteka `main.cpp`
-------------------

.. doxygenfile:: main.cpp

Datoteka `file.h`
-----------------

Definira operacije sa datotekama koje se koriste u projektu. To su uglavnom
funkcije koje na osnovu templejt datoteke i podataka učitanih iz JSON datoteka
generišu izlazne fajlove koji će se smjestiti u MBED workspace.

.. doxygenfile:: file.h

Datoteka `generator.h`
----------------------

Sadrži funkcije za generisanje C++ koda na osnovu podataka učitanih iz JSON
datoteka.

.. doxygenfile:: generator.h
