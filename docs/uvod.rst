====
Uvod
====

.. toctree::

U kratkim crtama
================

Ovaj podmodul ima zadatak da omogući programeru da isprogramira novi pametni
uređaj. Dolazi sa programom `dev-conf` koji se koristi za generisanje 
**MBED workspace-a** u kojem će se programirati uređaj.

Konfigurator uređaja se koristi u 3 koraka:

1. Programer kreira JSON fajlove sa osnovnim podacima o uređajima
2. Iz ovih fajlova se generišu C++ fajovi koji se smještaju u MBED radni
   direktorij.
3. Programer sasvim normalno programira funkcionalnosti uređaja koje su
   deklarisane u generisanim C++ fajlovima.

U nastavku je detaljno opisan prethodni postupak. Da biste vidjeli koje opcije
podržava program `dev-conf`, pogledajte :ref:`MANPAGE <manpage>`.

.. _details:

Detaljan opis postupka
======================

Generisanje JSON predložaka
---------------------------

Pokretanjem sljedećeg programa: 

.. code-block:: shell

   dev-conf --setup .

će se u trenutnom direktoriju generisati predlošci:

.. code-block:: none

   ./
   ├── factory_device.json
   └── user_device.json

.. admonition:: Napomena
   
   Umjesto trenutnog direktorija `./`, može se navesti bilo koji drugi. Ako
   zadani direktorij ne postoji, biće automatski kreiran.

.. admonition:: Napomena

   Nije potrebno generisati JSON predloške korištenjem programa `dev-conf`.
   Moguće je ove datoteke ručno kreirati, ali prethodni način je jednostavniji.

Način popunjavanja ovih predložaka je opisan u odjeljku :ref:`JSON datoteke`.

Generisanje C++ koda
--------------------

Ako su JSON datoteke koje opisuju uređaj spremne, sljedeći korak je iz tih
datoteka generisati polazne `h` i `cpp` datoteke. Ovo se može obaviti
pomoću sljedeće komande:

.. code-block:: shell

    dev-conf --input-dir . --output-dir mbed-workspace/

.. todo:: Moramo ponovo implementirati nakon što smo reorganizovali projekat.

.. admonition:: Napomena

   Umjesto ulaznog direktorija `./` i izlaznog direktorija
   `mbed-workspace/`, moguće je zadati bilo koje druge direktorije. Ako zadani
   direktoriji ne postoje, biće kreirani.

Kao rezultat, biće kreirane sljedeće datoteke. 

MBED polazni workspace
++++++++++++++++++++++

.. code-block:: none

    mbed-workspace/
    ├── factory_device.json
    ├── user_device.json
    ├── main.cpp
    ├── device.cpp
    └── device.h

Pri tome su JSON datoteke prosto kopirane iz ulaznog direktorija u izlazni.
Nakon ovog koraka, sve što je preostalo je implementirati funkcionalnost u ovim
datotekama. Konfigurator uređaja je učinio svoj dio, ostalo je zadatak
programera.

.. todo:: eventualno generisati i ostale mbed datoteke korištenjem mbed-cli?
