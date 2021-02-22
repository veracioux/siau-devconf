========================
Razvoj programa dev-conf
========================

.. toctree::

Definirane funkcije
===================

Datoteka `main.cpp`
-------------------

.. doxygenfile:: main.cpp

Datoteka `file.h`
-----------------

Sadrži generičke operacije sa datotekama koje se koriste u projektu.

.. doxygenfile:: file.h

Datoteka `symbols.h.in`
-----------------------

Sadrži ``#define`` direktive čije vrijednosti zavise od instalacijskih
parametara. Na primjer, u ovoj datoteci je definiran simbol ``TEMPLATE_DIR``
koji sadrži putanju direktorija u kojem se nalaze predlošci JSON datoteka.
Vrijednost ovog simbola se postavlja kroz CMake korištenjem ``configure_file``.

.. include:: _build/files/symbols.h.in
   :code: c++
   
