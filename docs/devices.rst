======================
Reprezentacija uređaja
======================

.. toctree:: :maxdepth: 3

Uređaji imaju dva seta parametara:

   1. :ref:`tvorničke <factory_device_json>` -- `factory_device.json`
   2. :ref:`korisničke <user_device_json>` -- `user_device.json`

*Tvornički parametri* određuju hardverske parametre koji se ne mogu mijenjati i
koje uređaji posjeduju kada izađu iz fabrike. *Korisnički parametri* su oni koji
se mijenjaju prilikom ugradnje uređaja u pametnu kuću i oni predstavljaju *ličnu
kartu* svakog pojedinačnog uređaja.

Tvornički parametri se nalaze u centralnoj bazi podataka. Ova baza podataka
sadrži opise svih uređaja koji su prepoznati. S druge strane, korisnički
parametri se pohranjuju samo u lokalnoj bazi podataka na nivou kuće.

Pošto mikrokontroleri obično posjeduju vrlo ograničene resurse, nezgodno je
vršiti procesiranje JSON datoteka na samom mikrokontroleru. Pogodnije je
iz JSON datoteka generisati C++ datoteke koje se popunjavaju samo osnovnim
deklaracijama koje su specifične za uređaj, dok programer kasnije implementira
funkcionalnost.

Generišu se sljedeće datoteke:

   1. :ref:`main.cpp.in <main_cpp_in>` :menuselection:`-->` `main.cpp`
   2. :ref:`iot_device.h.in <iot_device_h_in>` :menuselection:`-->` `iot_device.h`
   3. Prazna datoteka `iot_device.cpp`

JSON datoteke
-------------

.. _factory_device_json:

Datoteka `factory_device.json`
++++++++++++++++++++++++++++++++

.. literalinclude:: _build/files/factory_device.json
   :language: json

.. admonition:: Uputa
   :class: tip

   Tekst ``TODO`` programeru služi kao indikator da odgovarajući atribut nije
   još uvijek konfigurisan.

Atributi uređaja
################

Atribut ``name``
****************

   Naziv modela uređaja.

Atributi ``vendorId``, ``model``
***********************************

   Ovi atributi predstavljaju identifikaciju uređaja i jedinstveni su za svaki
   uređaj istog modela, od istog proizvođača.

.. _data:

Niz ``data``
############

    Sadrži podatke koje se mogu zatražiti od uređaja. Ovo su najčešće neka
    mjerenja, ili trenutno stanje uređaja.

Atribut ``name``
****************
   Format: Ispravan naziv C++ funkcije koji ne završava sa ``_response``.
      ..

   Jedinstveni identifikator koji ujedno predstavlja i ime metode u C++ klase
   ``Device``. Ova metoda je po svom karakteru *getter*.

Atribut ``friendly``
********************

   Naziv pod kojim će korisnik vidjeti ovaj podatak na GUI prikazu.

.. _data_valueType:

Atribut ``valueType``
*********************
   Moguće vrijednosti:
      .. centered:: ``float``, ``int``, ``bool``, ``<ENUM-TYPE>``
   Default: ``float``
      ..

   Određuje tip podatka koji se dobija sa uređaja. Ovaj podatak je payload
   odgovarajućeg topic-a. Može biti ``float``, ``int``, ``bool``, ili naziv
   korisnički-definiranog enumeriranog tipa. Konkretne vrijednosti ovog enum-a
   se zadaju putem atributa ``valueRange`` koji je opisan u nastavku. U headeru
   `iot_device.h` će se generisati definicija ovog enum tipa, zajedno sa
   funkcijama koje vrše konverziju ovog tipa u string i obratno.

   Ako se ne navede atribut ``valueType``, onda će se uzeti da je on ``void``, što
   znači da ova funkcija ne zahtijeva nikakav ulazni podatak.

Atribut ``valueRange``
**********************
   .. admonition:: Napomena

      Ako atribut ``valueType`` nije korisnički definirani enum tip, onda se
      ovaj atribut **ignoriše**. *Opcionalno*, GUI implementacija može
      iskoristiti ovaj atribut da bi se odredio poželjan broj decimala za prikaz
      vrijednosti ovog podatka.

   Određuje skup vrijednosti koje može uzeti enum čiji je naziv sadržan u
   atributu ``valueType``.

.. _data_unit:

Atribut ``unit``
****************
   Mjerna jedinica ulaznog parametra ove funkcije koja se treba prikazati u GUI.
   Ako se ne zada ovaj atribut, uzima se da je prazan string. Ako je
   ``valueType`` jednak ``void``, onda ovaj atribut nema nikakvu ulogu.

.. +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. _functions:

Niz ``functions``
#################

   Niz funkcija uređaja. Funkcija je bilo koja radnja koju uređaj može
   obavljati. Funkcija može da prima neki parametar, koji predstavlja nešto što
   korisnik zadaje pritiskom dugmeta, izborom iz menija ili sl. Korisnik ovu
   komandu u GUI vidi pod imenom koje se zadaje atributom ``friendly``. U klasi
   ``Device`` će se generisati metoda sa nazivom ``name`` koju programer treba
   da implementira za konkretni uređaj. Generisana metoda je po svom karakteru
   *setter*.

   Alternativno, funkcija može biti specificirana kao niz podfunkcija. Ovo je
   korisno ako dvije funkcije konceptualno pripadaju istoj skupini (na primjer
   funkcije ``upali`` i ``ugasi`` za neku pametnu lampu utiču na ON/OFF stanje
   lampe).

Atribut ``valueType``
*********************
   Moguće vrijednosti:
      .. centered:: ``float``, ``int``, ``bool``, ``void``, ``<ENUM-VALUE>``
   Default: ``void``
      ..

   Ima sličnu ulogu kao :ref:`istoimeni atribut <data_valueType>` u sklopu
   podatka uređaja . Jedna razlika je da ovaj atribut može imati dodatnu
   vrijednost ``void``. Ovaj parametar predstavlja ulaznu vrijednost koja se
   šalje uređaju da bi izvršio funkciju. To je ujedno i parametar odgovarajuće
   C++ funkcije. Tip ``void`` znači da uređaj ne zahtijeva nikakav
   parametar za izvršenje funkcije.

Atribut ``valueRange``
**********************
   U slučaju da je ``valueType`` neki brojni tip, ovaj atribut treba biti niz
   od dva elementa koji sadrži donju i gornju granicu vrijednosti. U slučaju
   da je ``valueType`` korisnički definirani enum tip, ovaj niz sadrži sve moguće
   vrijednosti ovog enum tipa u obliku stringova.

Atribut ``unit``
****************
   Ima istu ulogu kao :ref:`istoimeni atribut <data_unit>` u sklopu
   funkcija.

Atribut ``if``
**************
   Određuje kada je funkcija omogućena za upotrebu. Uvijek je formata
   ``<DEVICE_DATA> = <VALUE>`` pri čemu su ``<DEVICE_DATA>`` i ``<VALUE>``
   zamijenjeni konkretnim vrijednostima. Dakle funkcija je omogućena kada je
   podatak ``<DEVICE_DATA>`` jednak ``<VALUE>``.

   .. admonition:: Napomena

      Unutar atributa ``if`` svi razmaci se ignorišu.

----

|startexample|

.. literalinclude:: _build/files/SmartLight.json
   :language: json
   :lines: 10-14
   :dedent: 8

Ovo je primjer primitivne funkcije koja ne sadrži nikakve podfunkcije. Naziv
funkcije je ``setBrightness`` i to je ujedno naziv odgovarajuće C++ funkcije i
MQTT teme. Korisnik treba da vidi ovu funkciju sa ljepšim imenom **"Set
Brightness"**. Tip vrijednosti koju prima funkcija je ``float`` i to je ujedno
tip parametra kojeg prima odgovarajuća C++ funkcija. Opseg vrijednosti koje može
imati parametar je od 0.0 do 100.0. Mjerna jedinica parametra je `%`.

.. admonition:: Napomena

   U slučaju tipa ``float``, atribut ``valueRange`` se koristi samo da bi se na GUI
   ograničio opseg vrijednosti koje korisnik može unijeti. Ovaj atribut se ne
   koristi prilikom implementacije uređaja na mikrokontroleru.

|endcollapse|

|startexample|

.. literalinclude:: _build/files/SmartLight.json
   :language: json
   :lines: 6-9
   :dedent: 8

U ovom slučaju funkcija je sastavljena od dvije podfunkcije koje se pojedinačno
tumače kao u prethodnom primjeru.

Osnovna zamisao ovakvog zadavanja funkcije je da se na GUI dvije ili više radnji
može prikazati korištenjem istog widget-a. U ovom slučaju bi se mogao koristiti
widget ``Button`` čiji je tekst jednak sadržaju atributa ``friendly`` one
podfunkcije koja je omogućena. Korisnik može pritisnuti ovo dugme da bi
aktivirao onu funkciju čiji je naziv prikazan u tekstu dugmeta.

Stanje omogućenosti funkcije određuje atribut ``if``. Funkcija ``turnOn`` je
omogućena ako je podatak ``getState`` jednak ``Off``, a u suprotnom je omogućena
funkcija ``turnOff``.

|endcollapse|

----

.. +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. _user_device_json:

Datoteka `user_device.json`
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

.. _iot_device_h_in:

Datoteka `device.h.in`
++++++++++++++++++++++

.. literalinclude:: _build/files/iot_device.h.in
   :language: c++

Ova datoteka je predložak za datoteku `iot_device.h` koja se treba generisati u
**MBED workspace**\-u. Gdje god se u datoteci nađu alfanumerički znakovi ispred
kojih se nalazi znak ``$`` treba se ubaciti vrijednost odgovarajućeg atributa
uređaja. Također, svaka linija omeđena znakovima ``/*** ***/`` će biti
zamijenjena implementacijama odgovarajućih metoda.  Sve što je potrebno za
generisanje ove datoteke se nalazi u odgovarajućoj datoteci
:ref:`factory_device.json<factory_device_json>`. Datoteka
:ref:`user_device.json<factory_device_json>` se ovdje ne koristi.

----

|startexample|

Ako je u datoteci `factory_device.json` definiran sljedeći atribut uređaja:

.. code-block:: json

   {
       "...": "...",
       "vendorId": "ETF_SIAU",
       "...": "..."
   }

sljedeća linija iz datoteke `iot_device.h.in`

.. code-block:: c++

   const char *getVendorId() const { return "$vendorId"; }

će biti zamijenjena sa:

.. code-block:: c++

   const char *getVendorId() const { return "ETF_SIAU"; }

|endcollapse|

----

.. _main_cpp_in:

Datoteka `main.cpp.in`
++++++++++++++++++++++++

.. literalinclude:: _build/files/main.cpp.in
   :language: c++

Za generisanje ove datoteke se koristi datoteka :ref:`user_device.json
<user_device_json>`. Vrijedi sličan princip kao za datoteku :ref:`iot_device.h
<iot_device_h_in>`.

.. todo:: Ovaj dio je nepotpun. Potrebno je odrediti šta će se generisati u ovoj
   datoteci, da li će se uopšte vršiti konfigurisanje ove datoteke i sl.
   Najprije je potrebno napraviti osnovnu implementaciju uređaja...
