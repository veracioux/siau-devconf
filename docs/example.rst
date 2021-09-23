Konkretan primjer
-----------------

.. toctree::

Ovdje će biti razmotren jedan jednostavan konkretan primjer: *pametna lampa*.
Lampa se može uključivati/isključivati te se može zadavati jačina svjetlosti
preko WiFi-a.

Tvornička konfiguracija
+++++++++++++++++++++++

Sadržaj datoteke `factory_device.json`
   ..

.. literalinclude:: _build/files/SmartLight.json
   :language: json

Korisnička konfiguracija
++++++++++++++++++++++++

Sadržaj datoteke `user_device.json`
   ..

.. literalinclude:: ../tests/user_device.json
   :language: json

Identifikacijski parametri su proizvoljno odabrani.

Podaci iz uređaja (niz ``data``)
++++++++++++++++++++++++++++++++

.. _data_on_off:

ON/OFF stanje lampe
*******************

.. literalinclude:: _build/files/SmartLight.json
   :language: json
   :lines: 20-23
   :dedent: 8

Stanje koje određuje da li je lampa uključena ili isključena.

.. admonition:: Napomena

   U klasi ``Device`` u datoteci `iot_device.h` će biti generisan ``enum`` tip
   sa nazivom ``StateOnOff``.

Jačina svjetlosti
*****************

.. literalinclude:: _build/files/SmartLight.json
   :language: json
   :lines: 16-19
   :dedent: 8

Funkcije uređaja (niz ``functions``)
++++++++++++++++++++++++++++++++++++

Upali/ugasi
***********

.. literalinclude:: _build/files/SmartLight.json
   :language: json
   :lines: 6-9
   :dedent: 8

Ovo je jedna funkcija koja se sastoji od dvije podfunkcije ``turnOn`` i
``turnOff``. Korisniku se ove podfunkcije prikazuju pod nazivima *Turn On* i
*Turn Off* respektivno.

Pri tome, nisu obje funkcije omogućene istovremeno. Nema smisla paliti sijalicu
ako je već upaljena ili gasiti sijalicu ako je već ugašena. Atribut ``if``
određuje kada je koja podfunkcija omogućena. U ovom slučaju podfunkcija
``turnOn`` je omogućena ako je sijalica u isključenom stanju, tj. kada je
vrijednost :ref:`getState <data_on_off>` jednaka ``Off``. U suprotnom je
omogućena funkcija ``turnOff``.


.. admonition:: Napomena

   Za ove funkcije će biti generisane metode ``turnOn`` i ``turnOff`` bez
   parametara u sklopu klase ``Device`` u datoteci `iot_device.h`. Nalog za
   izvršavanje ove funkcije se zadaje preko topic-a `hodnik/LG-0001/turnOn`,
   odnosno `hodnik/LG-0001/turnOff`.

Postavi jačinu svjetlosti
*************************

.. literalinclude:: _build/files/SmartLight.json
   :language: json
   :lines: 10-14
   :dedent: 8

Vrijednost trenutne jačine svjetlosti.

.. admonition:: Napomena

   Klasa ``Device`` u generisanoj datoteci `iot_device.h` će sadržavati metodu
   ``getBrightness`` koja vraća vrijednost tipa ``float``.

Popis korištenih MQTT poruka
++++++++++++++++++++++++++++

+-----------------------+-----------+-----------------------------------------+---------------------+
| Naziv                 | Publisher | Tema                                    | Payload             |
+=======================+===========+=========================================+=====================+
| ``requestConnection`` | Sistem    | `hodnik/_/requestConnection`            | *LX34-U6M, LG-0001* |
+-----------------------+-----------+-----------------------------------------+---------------------+
| ``approveConnection`` | Sistem    | `hodnik/LG-0001/approveConnection`      |                     |
+-----------------------+-----------+-----------------------------------------+---------------------+
| ``getBrightness``     | Sistem    | `hodnik/LG-0001/getBrightness`          |                     |
+-----------------------+-----------+-----------------------------------------+---------------------+
|                       | Uređaj    | `hodnik/LG-0001/getBrightness/response` | *[0.0,100.0]*       |
+-----------------------+-----------+-----------------------------------------+---------------------+
| ``getState``          | Sistem    | `hodnik/LG-0001/getState`               |                     |
+-----------------------+-----------+-----------------------------------------+---------------------+
|                       | Uređaj    | `hodnik/LG-0001/getState/response`      | *On* ili *Off*      |
+-----------------------+-----------+-----------------------------------------+---------------------+
| ``turnOn``            | Sistem    | `hodnik/LG-0001/turnOn`                 |                     |
+-----------------------+-----------+-----------------------------------------+---------------------+
| ``turnOff``           | Sistem    | `hodnik/LG-0001/turnOff`                |                     |
+-----------------------+-----------+-----------------------------------------+---------------------+
| ``setBrightness``     | Sistem    | `hodnik/LG-0001/setBrightness`          | *[0.0,100.0]*       |
+-----------------------+-----------+-----------------------------------------+---------------------+

Generisane C++ datoteke
+++++++++++++++++++++++

Datoteka `main.cpp`
*******************

Ova datoteka je potpuno automatski generisana, i u većini slučajeve neće biti
potrebno praviti ikakve izmjene.

.. literalinclude:: ../template/main.cpp
   :language: C++

Datoteka `iot_device.h`
***********************

.. literalinclude:: example/iot_device.h
   :language: C++

Datoteka `iot_device.cpp`
*************************

Ova datoteka je automatski generisana na osnovu JSON datoteka i ima sljedeći
izgled:

.. literalinclude:: example/iot_device.cpp
   :language: C++

Implementirana datoteka bi mogla izgledati na primjer ovako:

.. literalinclude:: example/iot_device_impl.cpp
   :language: C++

Programski kod se sada može kompajlirati i poslati na uređaj, bez ikakve dodatne
konfiguracije.

