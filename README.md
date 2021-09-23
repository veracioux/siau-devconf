# Konfigurator uređaja

Ovaj podmodul ima zadatak da omogući programeru da isprogramira novi pametni
uređaj. Na osnovu odabranih konfiguracijskih parametara se generiše polazni
programski kod koji programer treba dodatno implementirati, nakon čega se može
uploadovati na mikrokontroler uređaja korištenjem [MBED](https://mbed.org)
platforme.  Konfiguracijski parametri se zadaju u JSON formatu.

# NAPOMENA: JOŠ UVIJEK NIJE TESTIRANO NA WINDOWS-u

# Opis principa

Pošto se razvoj pametne kuće vrši u više iteracija te se paralelno vrši
razvoj različitih komponenti koje trebaju da budu sinhronizovane, potrebno je
uniformizirati pristupe. Na primjer tim koji kreira logiku i GUI treba da ima
uniforman interfejs sa svim uređajima, tako da samo tim koji vrši
konfiguraciju uređaja mora brinuti o finim detaljima uređaja. Iz tog razloga
će svi uređaji biti predstavljeni preko JSON datoteka. Ostali timovi mogu ove
datoteke pročitati kako bi mogli napraviti interfejs sa ovim uređajima.
Također, ove datoteke će se iskoristiti za generisanje osnovnih C++ source
predložaka, koje programer uređaja treba da implementira.

# Dokumentacija

Kompletna dokumentacija se može pronaći [ovdje](https://siau-devconf.rtfd.io).

# Setup

Da bi se izgradio program `devconf`, potrebno je instalirati sljedeće programe.

## Potrebni programi i biblioteke

- CMake >= 3.10
- Qt >= 5.9
  - qt5-mqtt
- make
- python3, docutils (samo za generisanje manpage-a)

Trebalo bi da se svi ovi alati mogu instalirati korištenjem vašeg omiljenog
package manager-a.

## Build

Potrebno je pokrenuti komandu:

    make

Ovo će kreirati program `devconf` i manpage, koji će biti smješteni u folderu
`_build/`. Alternativno, moguće je pozvati:

    make app

odnosno

    make man

da bi se ove komponente izgradile pojedinačno.

**NAPOMENA:** Ovako generisan program `devconf` se smije pozivati samo iz root
foldera projekta, u suprotnom neće raditi korektno.

Ako je potrebno program kompajlirati direktno iz `CMakeLists.txt`, ova datoteka
se nalazi u folderu `src/.`

## Instalacija

Program `devconf` se može instalirati korištenjem komande:

    make install PREFIX=<DIR>

**NAPOMENA:** `<DIR>` **mora biti apsolutna putanja. Ovo je od krucijalnog značaja!**

Ako se komanda pozove samo kao `make install`, tj. ako se ne zada varijabla
`PREFIX`, program će biti instaliran u folderu `/usr/local`. U tom slučaju
je potreban `sudo` pristup.

# Komanda `devconf`

Ova komanda će se koristiti za generisanje *source datoteka* na osnovu *JSON
datoteka* i predložaka *C++ datoteka*. Dokumentacija (source) ove komande se
nalazi u datoteci `docs/man/devconf.rst`. Ova dokumentacija se može build-at
putem:

    make man

pri čemu će se generisati **Linux Manpage** `_build/devconf.1.gz`. Ovaj manpage se
može otvoriti korištenjem komande (na Linux-u):

    man -l _build/devconf.1.gz

Međutim, source ovog manpage-a je već itekako čitljiv, pogotovo na
GitHub/GitLab-u.

