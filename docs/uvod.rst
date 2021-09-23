====
Uvod
====

.. toctree::

U kratkim crtama
================

Ovaj podmodul ima zadatak da omogući programeru da isprogramira novi pametni
uređaj. Dolazi sa programom `devconf` koji se koristi za generisanje 
**MBED workspace-a** u kojem će se programirati uređaj.

Konfigurator uređaja se koristi u 3 koraka:

1. Programer kreira JSON fajlove sa osnovnim podacima o uređajima
2. Iz ovih fajlova se generišu C++ fajovi koji se smještaju u MBED radni
   direktorij.
3. Programer sasvim normalno implementira funkcionalnosti uređaja koje su
   deklarisane u generisanim C++ fajlovima.

U nastavku je detaljno opisan prethodni postupak. Da biste vidjeli koje opcije
podržava program `devconf`, pogledajte :ref:`MANPAGE <manpage>`.

.. _details:

Detaljan opis postupka
======================

Generisanje JSON predložaka
---------------------------

Pokretanjem sljedećeg programa:

.. code-block:: shell

   devconf --template .

će se u trenutnom direktoriju generisati predlošci:

.. code-block:: none

   ./
   ├── factory_device.json
   └── user_device.json

.. admonition:: Napomena

   Umjesto trenutnog direktorija `./`, može se navesti bilo koji drugi. Ako
   zadani direktorij ne postoji, biće automatski kreiran.

.. admonition:: Napomena

   Nije potrebno generisati JSON predloške korištenjem programa `devconf`.
   Moguće je ove datoteke ručno kreirati, ali prethodni način je jednostavniji.

Način popunjavanja ovih predložaka je opisan u odjeljku :ref:`JSON datoteke`.

Generisanje C++ koda
--------------------

Ako su JSON datoteke koje opisuju uređaj spremne, sljedeći korak je iz tih
datoteka generisati polazne `h` i `cpp` datoteke. Ovo se može obaviti
pomoću sljedeće komande:

.. code-block:: shell

    devconf --input-dir . --output-dir mbed-workspace/

.. admonition:: Uputa
   :class: tip

   Moguće je koristiti dodatnu opciju `-m/--mbed`. Ova opcija će inicijalizirati
   mbed workspace sa potrebnim bibliotekama u izlaznom direktoriju.

.. admonition:: Napomena

   Umjesto ulaznog direktorija `./` i izlaznog direktorija
   `mbed-workspace/`, moguće je zadati bilo koje druge direktorije. Ako zadani
   direktoriji ne postoje, biće kreirani.

MBED polazni workspace
++++++++++++++++++++++

Kao rezultat prethodne komande biće kreirane sljedeće datoteke:

.. code-block:: none

    mbed-workspace/
    ├── factory_device.json
    ├── user_device.json
    ├── main.cpp
    ├── iot_device.cpp
    ├── iot_device.h
    └── autogen/

Pri tome su JSON datoteke prosto kopirane iz ulaznog direktorija u izlazni.
Nakon ovog koraka, sve što je preostalo je implementirati funkcionalnost u ovim
datotekama. Konfigurator uređaja je učinio svoj dio, ostalo je zadatak
programera.

Naime, potrebno je implementirati funkcije u datoteci
`iot_device.cpp`. Zavisno od konkretne primjene, možda će biti potrebno
izmijeniti i datoteke `main.cpp`, te `iot_device.h`. Ako nije korištena
`-m/--mbed` opcija, onda je potrebno ručno dodati biblioteke `mbed-os` i
`mbed-mqtt` u mbed workspace.

.. admonition:: Napomena

   Direktorij `autogen/` sadrži pomoćne fajlove i nije predviđeno da se
   direktno modifikuje, osim u izuzetnim situacijama.

