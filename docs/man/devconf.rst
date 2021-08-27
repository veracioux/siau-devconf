=======
devconf
=======

SYNOPSIS
========

TODO: Trenutno je implementirana samo `--template` opcija.

::

   devconf [--help|-h]
   devconf --template [<DIR>]
   devconf --input-dir <DIR> --output-dir <DIR>

DESCRIPTION
===========

Program *devconf* se koristi za generisanje predloška za programiranje MBED
uređaja za pametnu kuću. Preporučuje se da rezultujuće datoteke budu smještene u
isti direktorij koji će se koristiti kao *workspace* za programiranje kroz
okruženje MBED.

Osnovni predlošci datoteka se mogu generisati korištenjem komande

::

   devconf --template

Programer treba ove datoteke popuniti konfiguracijskim parametrima po
želji.

Zatim se pomoću komande

::

   devconf --input-dir <DIR> --output-dir <DIR>

generiše MBED workspace za programiranje uređaja.

OPTIONS
=======

`-t [\<DIR\>]`, `--template [\<DIR\>]`

   Generiše potrebne predloške JSON datoteka koje korisnik treba da
   popuni željenim početnim konfiguracijskim parametrima. Ako se ne
   zada argument `\<DIR\>`, onda će se datoteke generisati u trenutnom
   direktoriju.

`-i \<DIR\>`, `--input-dir=\<DIR\>`

   `\<DIR\>` je direktorij koji sadrži ulazne datoteke *factory_device.json*,
   *user_device.json*, *main.cpp.in*. 

`-o \<DIR\>`, `--output-dir=\<DIR\>`

   `\<DIR\>` je direktorij u koji će se smjestiti izlazne datoteke, a također
   predstavlja **workspace za programiranje MBED uređaja.**

`-h`, `--help`

   Printa sažetu verziju ovog dokumenta.

AUTHOR
======

SIAU: Tim 1

COPYRIGHT
=========

SIAU ETF
