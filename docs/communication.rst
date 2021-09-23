Komunikacija sa uređajima
=========================

.. toctree::

Komunikacija se vrši korištenjem MQTT protokola.

Format MQTT teme
----------------

Svaka tema treba da bude sljedećeg formata::

   lokacija/serialNo/poruka

Jedini izuzetak je :ref:`zahtjev za konekciju <requestConnection>`.

Pri tome `lokacija` uređaja može predstavljati na primjer naziv sobe, a polje
`serialNo` je jedinstveni serijski broj uređaja. Polje `poruka` predstavlja
naziv naziv :ref:`funkcije<functions>` koju uređaj treba izvršiti, ili
:ref:`podatka<data>` koji se čita sa uređaja.

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

.. centered:: [ `serialNo`, `model` ]

Pri tome je `serialNo` jedinstvena identifikacija konkretnog uređaja i zadaje se
kroz datoteku :ref:`user_device.json <user_device_json>`, dok se `model` zadaje
kroz datoteku :ref:`factory_device.json <factory_device_json>`.  String
`serialNo` mora biti nul-terminiran, dok za `model` to nije neophodno.

Ako je konekcija uspješna, smart-home sistem treba da pošalje povratnu poruku
uređaju::

   lokacija/serialNo/approveConnection

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

   lokacija/serialNo/fun

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

   lokacija/serialNo/dat

bez payload-a.

Uređaj će polati povratnu informaciju na temi::

   lokacija/serialNo/dat/response

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

   dnevni_boravak/001-2340/getEnergyConsumption/response

a payload će sadržavati ``float`` koji sadrži traženu vrijednost.

|endcollapse|
