Komunikacija sa uređajima
=========================

.. toctree::

Komunikacija se vrši korištenjem MQTT protokola.

Format MQTT teme
----------------

Svaka tema treba da bude sljedećeg formata::
   
   lokacija/uređaj/poruka

Jedini izuzetak je **zahtjev za konekciju**::

   lokacija/requestConnection

koji će biti kasnije objašnjen.

.. todo:: Kasnije objasniti :)

Pri tome `lokacija` uređaja može predstavljati na primjer naziv sobe, a polje
`uređaj` je jedinstvena identifikacija uređaja. Polje `poruka` predstavlja naziv
poruke i **identično** je nazivu :ref:`funkcije<functions>` koju uređaj treba
izvršiti, ili :ref:`podatka<data>` koji se čita sa uređaja.

.. todo:: Odrediti šta će se koristiti kao jedinstvena identifikacija uređaja. U
   primjerima je korišten serijski broj, ali nismo se konačno odlučili da će
   ostati tako.

Zahtjev za konekciju
--------------------

.. todo:: napisati

Aktivacija funkcije uređaja
---------------------------

Da bi se na uređaju aktivirala neka funkcija sa nazivom ``fun`` (koja je
definirana u datoteci :ref:`factory_device.json<factory_device_json>`) potrebno
je poslati poruku na sljedećoj temi::

   lokacija/uređaj/fun

----

.. include:: /inc/startdetails.rst

Ako neki uređaj ima serijski broj `001-2340` i nalazi se u sobi
`dnevni_boravak`, te je potrebno izvršiti funkciju:

.. code-block:: json

   {
       "name": "setTemperature", "friendly": "Set temperature", "valueType":
       "float", "valueRange": [20, 30], "unit": "C"
   }

onda će odgovarajuća tema biti::

   dnevni_boravak/001-2340/setTemperature

Poslani payload treba da sadrži ``float`` koji predstavlja zadanu vrijednost
temperature.

.. include:: /inc/enddetails.rst

----

Prijem podatka sa uređaja
-------------------------

Da bi se sa uređaja dobio neki podatak sa nazivom ``dat`` (koji je definiran u
datoteci :ref:`factory_device.json<factory_device_json>`) potrebno je primiti
poruku na sljedećoj temi::

   lokacija/uređaj/dat

----

.. include:: /inc/startdetails.rst

Ako neki uređaj ima serijski broj `001-2340` i nalazi se u sobi
`dnevni_boravak`, a zanima nas sljedeći :ref:`podatak<data>` sa uređaja:

.. code-block:: json

   {
       "name": "getEnergyConsumption", "friendly": "Energy consumption",
       "valueType": "float", "unit": "kWh"
   }

onda će odgovarajuća tema biti::

   dnevni_boravak/001-2340/getEnergyConsumption

.. include:: /inc/enddetails.rst
.. raw:: html

   <br>

