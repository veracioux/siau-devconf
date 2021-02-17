======================
Reprezentacija uređaja
======================

.. toctree:: :maxdepth: 3

Uređaji imaju dva seta parametara:

1. :ref:`tvorničke <factory_device_json>` -- ``factory_device.json``
2. :ref:`korisničke <user_device_json>` -- ``user_device.json``

*Tvornički parametri* određuju hardverske parametre koji se ne mogu mijenjati i
koje uređaji posjeduju kada izađu iz fabrike. *Korisnički parametri* su oni koji
se mijenjaju prilikom ugradnje uređaja u pametnu kuću i oni predstavljaju *ličnu
kartu* svakog pojedinačnog uređaja.

Tvornički parametri se nalaze u centralnoj bazi podataka. Ova baza podataka
sadrži opise svih uređaja koji su prepoznati. S druge strane, korisnički
parametri se pohranjuju samo u lokalnoj bazi podataka na nivou kuće.

.. todo:: Dogovoriti sa timom [Logika, GUI] kako ovo organizovati.

JSON datoteke
-------------

.. _factory_device_json:

Datoteka ``factory_device.json``
++++++++++++++++++++++++++++++++

.. literalinclude:: _build/files/factory_device.json
   :language: json

.. admonition:: Uputa
   :class: tip

   Tekst ``TODO`` programeru služi kao indikator da odgovarajući atribut nije
   još konfigurisan.

Atributi uređaja
################

Atribut ``name``
****************

   Naziv modela uređaja.

.. todo:: Izbaciti ovaj atribut?

Atributi ``vendorId``, ``deviceId``
***********************************

   Ovi atributi predstavljaju identifikaciju uređaja i jedinstveni su za svaki
   uređaj istog modela, od istog proizvođača.

Niz ``functions``
#################

   Niz funkcija uređaja. Funkcija je bilo koja radnja koju uređaj može
   obavljati. Funkcija može da prima neki parametar, koji predstavlja nešto što
   korisnik zadaje pritiskom dugmeta, izborom iz menija ili sl. Korisnik ovu
   komandu u GUI vidi pod imenom koje se zadaje atributom ``friendly``. U klasi
   ``Device`` će se generisati metoda sa nazivom ``name`` i koju programer treba
   da implementira za konkretni uređaj. Generisana metoda je po svom karakteru
   *setter*.

   Alternativno, funkcija može biti specificirana kao niz podfunkcija. Ovo je
   korisno ako dvije funkcije konceptualno pripadaju istoj skupini (na primjer
   funkcije ``upali`` i ``ugasi`` za neku pametnu lampu utiču na ON/OFF stanje
   lampe).

.. _functions_valueType:

Atribut ``valueType``
*********************
   Moguće vrijednosti:
      .. centered:: ``float``, ``int``, ``bool``, ``void``, ``<ENUM-VALUE>``
   Default: ``void``
      ..

   Određuje tip podatka koji prima ova funkcija. Ovaj podatak predstavlja
   payload odgovarajućeg topic-a. Može biti ``float``, ``int``, ``bool``,
   ``void`` ili naziv korisnički-definiranog enumeriranog tipa. Konkretne
   vrijednosti ovog enum-a se zadaju putem ``valueRange`` koji je opisan u
   nastavku. U headeru ``device.h`` će se generisati definicija ovog enum tipa,
   zajedno sa funkcijama koje vrše konverziju ovog tipa u string i obratno.

   Ako se ne navede atribut ``valueType``, onda će se uzeti da je on ``void``, što
   znači da ova funkcija ne zahtijeva nikakav ulazni podatak.

Atribut valueRange
******************
   U slučaju da je ``valueType`` neki brojni tip, ovaj atribut treba biti niz
   od dva elementa koji sadrži donju i gornju granicu vrijednosti. U slučaju
   da je ``valueType`` korisnički definirani enum tip, ovaj niz sadrži sve moguće
   vrijednosti ovog enum tipa u obliku stringova.

.. _functions_unit:

Atribut ``unit``
****************
   Mjerna jedinica ulaznog parametra ove funkcije koja se treba prikazati u GUI.
   Ako se ne zada ovaj atribut, uzima se da je prazan string. Ako je
   ``valueType`` jednak ``void``, onda ovaj atribut nema nikakvu ulogu.

Niz ``values`` 
###############

    Sadrži vrijednosti koje se mogu zatražiti od uređaja. Ovo su najčešće neka
    mjerenja, ili trenutno stanje uređaja.
    
Atribut ``name``
****************
   Format: Ispravan naziv C++ funkcije.
      ..

   Jedinstveni identifikator koji ujedno predstavlja i ime metode u C++ klase
   ``Device``. Ova metoda je po svom karakteru *getter*.

Atribut ``friendly``
********************

   Naziv pod kojim će korisnik vidjeti ovu vrijednost na GUI prikazu.

Atribut ``valueType``
*********************
   Moguće vrijednosti:
      .. centered:: ``float``, ``int``, ``bool``, ``void``, ``<ENUM-VALUE>``
   Default: ``void``
      ..

   Ima istu ulogu kao :ref:`istoimeni atribut <functions_valueType>` u sklopu
   funkcija.

Atribut ``unit``
****************
   Ima istu ulogu kao :ref:`istoimeni atribut <functions_unit>` u sklopu
   funkcija.

.. todo:: Atribut if...

.. +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. _user_device_json:

Datoteka ``user_device.json``
+++++++++++++++++++++++++++++

Lična karta svakog konkretnog uređaja.

.. literalinclude:: _build/files/user_device.json
   :language: json           

.. admonition:: Uputa
   :class: tip

   Tekst ``TODO`` programeru služi kao indikator da odgovarajući atribut nije
   još konfigurisan.

.. +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

C++ datoteke
------------

.. admonition:: Napomena

   Ovaj odjeljak je od interesa samo za programere uređaja.

Datoteka ``device.h.in``
++++++++++++++++++++++++

.. literalinclude:: _build/files/device.h.in
   :language: c++

Datoteka ``main.cpp.in``
++++++++++++++++++++++++

.. literalinclude:: _build/files/main.cpp.in
   :language: c++

.. todo:: Ovaj dio je nepotpun. Potrebno je odrediti šta će se generisati u ovoj
   datoteci, da li će se uopšte vršiti konfigurisanje ove datoteke i sl.
   Najprije je potrebno napraviti osnovnu implementaciju uređaja...
