Komunikacija sa uređajima
=========================

.. toctree::

Komunikacija se vrši korištenjem MQTT protokola.

Format MQTT teme
----------------

Svaka tema treba da bude sljedećeg formata::
   
   lokacija/uređaj/poruka

Jedini izuzetak je :ref:`zahtjev za konekciju <requestConnection>`.

Pri tome `lokacija` uređaja može predstavljati na primjer naziv sobe, a polje
`uređaj` je jedinstvena identifikacija uređaja. Polje `poruka` predstavlja naziv
poruke i **identično** je nazivu :ref:`funkcije<functions>` koju uređaj treba
izvršiti, ili :ref:`podatka<data>` koji se čita sa uređaja.

.. todo:: Odrediti šta će se koristiti kao jedinstvena identifikacija uređaja. U
   primjerima je korišten serijski broj, ali nismo se konačno odlučili da će
   ostati tako.

.. _requestConnection:

Zahtjev za konekciju
--------------------

Uređaji se periodično pokušavaju povezati na sistem pametne kuće, šaljući
zahtjev za konekciju preko MQTT. Kada sistem primi zahtjev, korisniku se
prikaže opcija da doda taj uređaj.

Zahtjev za konekciju se šalje preko sljedeće MQTT teme::

   lokacija/_/requestConnection

Payload poruke je sljedeći:

.. centered:: [ :kbd:`model, serialNo` ]

Pri tome je `model` jedinstvena identifikacija modela uređaja, a `serialNo`
je jedinstvena identifikacija konkretnog uređaja (broj lične karte uređaja).

Ako je konekcija uspješna, smart-home sistem treba da pošalje povratnu poruku
uređaju::

   lokacija/serialNo/approveConnection

bez payload-a.

----

|startexample|

Ako neki uređaj modela `MDL-123` ima serijski broj `001-2340` i nalazi se u sobi
`dnevni_boravak`, zahtjev za konekciju će biti na temu::

   dnevni_boravak/_/requestConnection

sa payload-om

.. centered:: :kbd:`[ MDL-123, 001-2340 ]`

Pri uspješnoj konekciji, povratna informacija iz smart-home sistema će biti na
temu::

   dnevni_boravak/001-2340/approveConnection

sa praznim payload-om.

----

Aktivacija funkcije uređaja
---------------------------

Da bi se na uređaju aktivirala neka funkcija sa nazivom ``fun`` (koja je
definirana u datoteci :ref:`factory_device.json<factory_device_json>`) potrebno
je poslati poruku na sljedećoj temi::

   lokacija/uređaj/fun

----

|startexample|

Ako neki uređaj ima serijski broj `001-2340` i nalazi se u sobi
`dnevni_boravak`, te je potrebno izvršiti funkciju:

.. code-block:: json

   {
       "name": "setTemperature", "friendly": "Set temperature", "valueType":
       "float", "valueRange": [20, 30], "unit": "C"
   }

onda će odgovarajuća tema biti::

   dnevni_boravak/001-2340/setTemperature

Poslani payload treba da sadrži ``float`` iz intervala [20, 30] koji predstavlja
zadanu vrijednost temperature.

|endcollapse|

----

Prijem podatka sa uređaja
-------------------------

Da bi se sa uređaja dobio neki podatak sa nazivom ``dat`` (koji je definiran u
datoteci :ref:`factory_device.json<factory_device_json>`) potrebno je poslati
zahtjev za taj podatak na sljedećoj temi::

   lokacija/uređaj/dat

bez payload-a.

Uređaj će polati povratnu informaciju na temi::

   lokacija/uređaj/dat_response

sa payload-om koji sadrži traženi podatak.

----

|startexample|

Ako neki uređaj ima serijski broj `001-2340` i nalazi se u sobi
`dnevni_boravak`, a zanima nas sljedeći :ref:`podatak<data>` sa uređaja:

.. code-block:: json

   {
       "name": "getEnergyConsumption", "friendly": "Energy consumption",
       "valueType": "float", "unit": "kWh"
   }

onda smart-home sistem treba poslati poruku na temi::

   dnevni_boravak/001-2340/getEnergyConsumption

Uređaj će poslati odgovor na temi::

   dnevni_boravak/001-2340/getEnergyConsumption_response

a payload će sadržavati ``float`` koji sadrži traženu vrijednost.

|endcollapse|
