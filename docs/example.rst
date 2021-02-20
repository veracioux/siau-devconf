Konkretan primjer
-----------------

.. toctree::

Ovdje će biti razmotren jedan jednostavan konkretan primjer: *pametna lampa*.
Lampa se može uključivati/isključivati te se može zadavati jačina svjetlosti
preko WiFi-a.

Tvornička konfiguracija
+++++++++++++++++++++++

Sadržaj datoteke ``factory_device.json``
   ..

.. literalinclude:: _build/files/SmartLight.json
   :language: json

Podaci iz uređaja (niz ``data``)
################################

.. _data_on_off:

ON/OFF stanje lampe
*******************

.. literalinclude:: _build/files/SmartLight.json
   :language: json
   :lines: 21-24
   :dedent: 8

Stanje koje određuje da li je lampa uključena ili isključena.

.. admonition:: Napomena

   U klasi ``Device`` u datoteci ``device.h`` će biti generisan ``enum`` tip sa
   nazivom ``StateOnOff``.

Jačina svjetlosti
*****************

.. literalinclude:: _build/files/SmartLight.json
   :language: json
   :lines: 17-20
   :dedent: 8

Funkcije uređaja (niz ``functions``)
####################################

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
   parametara u sklopu klase ``Device`` u datoteci ``device.h``. Nalog za
   izvršavanje ove funkcije se zadaje preko topic-a ... (TODO kojeg?).

Postavi jačinu svjetlosti
*************************

.. literalinclude:: _build/files/SmartLight.json
   :language: json
   :lines: 10-14
   :dedent: 8

Vrijednost trenutne jačine svjetlosti.

.. admonition:: Napomena

   Klasa ``Device`` u generisanoj datoteci ``device.h`` će sadržavati metodu
   ``getBrightness`` koja vraća vrijednost tipa ``float``.

Korisnička konfiguracija
++++++++++++++++++++++++
    
Sadržaj datoteke ``user_device.json``
   ..

.. code-block:: json

   {
       "name": "Pametna sijalica 1",
       "databaseId": "123",
       "serialNo": "LG-0001",
       "location": "hodnik",
       "ip": "192.168.1.100",
       "subnet_mask": "255.255.255.0",
       "default_gateway": "192.168.1.1"
   }

Identifikacijski parametri su proizvoljno odabrani.
