Komunikacija sa uređajima
=========================

.. toctree::

Format topic-a
--------------

Svaki topic treba da bude sljedećeg formata:

.. code-block::
   
   lokacija/uređaj/poruka

Pri tome ``lokacija`` uređaja može predstavljati na primjer naziv sobe, a polje
``uređaj`` je jedinstvena identifikacija uređaja. Polje ``poruka`` predstavlja
naziv poruke i **identično** je nazivu funkcije koju uređaj treba izvršiti, ili
vrijednosti koja se čita sa uređaja.

.. todo:: Odrediti šta će se koristiti kao jedinstvena identifikacija uređaja. U
   primjerima je korišten serijski broj, ali nismo se konačno odlučili da će
   ostati tako.

Aktivacija funkcije uređaja
---------------------------

Da bi se na uređaju aktivirala neka funkcija sa nazivom ``fun`` (koja je
definirana u datoteci :ref:`factory_device.json<factory_device_json>`) potrebno
je poslati poruku na sljedećem topic-u:

.. code-block::

   lokacija/uređaj/fun

Primjer:
   ..

Konkretno, ako neki uređaj ima serijski broj ``001-2340`` i nalazi se u sobi
``dnevni_boravak``, te je potrebno izvršiti funkciju:

.. code-block:: json

   {
       "name": "setTemperature", "friendly": "Set temperature", "valueType":
       "float", "valueRange": [20, 30], "unit": "C"
   }

onda će odgovarajući topic biti:

.. code-block::

   dnevni_boravak/001-2340/setTemperature

Poslani payload treba da sadrži ``float`` koji predstavlja zadanu vrijednost
temperature.

Prijem vrijednosti sa uređaja
-----------------------------

Da bi se sa uređaja dobila neka vrijednost sa nazivom ``val`` (koja je
definirana u datoteci :ref:`factory_device.json<factory_device_json>`) potrebno
je primiti poruku na sljedećem topic-u:

.. code-block::

   lokacija/uređaj/val

Primjer:
   ..

Konkretno, ako neki uređaj ima serijski broj ``001-2340`` i nalazi se u sobi
``dnevni_boravak``, a zanima nas sljedeća vrijednost sa uređaja:

.. code-block:: json

   {
       "name": "getEnergyConsumption", "friendly": "Energy consumption",
       "valueType": "float", "unit": "kWh"
   }

onda će odgovarajući topic biti:

.. code-block::

   dnevni_boravak/001-2340/getEnergyConsumption
