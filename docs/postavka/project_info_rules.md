# Projekat za domaći rad - Projektni zadatak

Elektrotehnički fakultet u Beogradu  
Katedra za računarsku tehniku i informatiku

**Predmet:** Sistemski softver (13E113SS, 13S113SS)  
**Nastavnik:** prof. dr Saša Stojanović  
**Školska godina:** 2025/2026. (Zadatak važi počev od junskog roka 2026.)  
**Verzija dokumenta:** 1.0

**Važne napomene:** Pre čitanja ovog teksta, obavezno pročitati opšta pravila predmeta i pravila vezana za izradu domaćih zadataka! Pročitati potom ovaj tekst u celini i pažljivo, pre započinjanja realizacije ili traženja pomoći. Ukoliko u zadatku nešto nije dovoljno precizno definisano ili su postavljeni kontradiktorni zahtevi, student treba da uvede razumne pretpostavke, da ih temeljno obrazloži i da nastavi da izgrađuje preostali deo svog rešenja na temeljima uvedenih pretpostavki. Zahtevi su namerno nedovoljno detaljni, jer se od studenata očekuje kreativnost i profesionalni pristup u rešavanju praktičnih problema!

## Uvod

Cilj ovog projekta jeste realizacija alata u lancu prevođenja i emulatora za apstraktni računarski sistem. Opis apstraktnog računarskog sistema dat je u prilogu. Alati u lancu prevođenja koje treba realizovati obuhvataju asembler za navedeni apstraktni računarski sistem i linker nezavisan od ciljne arhitekture.

Rešenje projekta obuhvata izvorni kod kojim su implementirani asembler, linker i emulator. U daljem tekstu, rešenje projekta biće kratko nazivano samo implementacija. Izvorni asemblerski kod napisan za apstraktni računarski sistem, koji će asembler prevoditi, linker povezivati i emulator izvršavati, biće u nastavku referisan kao korisnički program kako bi se jasno napravila razlika u odnosu na implementaciju.

U cilju demonstracije i odbrane projekta, implementacija mora da se uspešno izvršava pod Linux operativnim sistemom kao konzolna aplikacija. Odbrana projekta pod Windows operativnim sistemom nije moguća.

## Opšti zahtevi

### Leksička analiza

Implementacija leksičke analize i parsiranja ulaznih tekstualnih datoteka ne predstavlja glavni zadatak projekta, ali jeste neophodna za uspešnu izradu rešenja, zbog čega je za te potrebe dozvoljeno je koristiti generatore leksera i parsera. Bez ulaska u detalje i namenu ovih alata, koji mogu biti korisni prilikom izrade rešenje, skreće se pažnja da su na virtuelnoj mašini instalirani alati `flex` i `bison` koji predstavljaju upravo generatore leksera i parsera respektivno. Pored generatora leksera i parsera dozvoljeno je i preporučeno koristiti standardne biblioteke kao što je STL (Standard Template Library). Takođe, dozvoljeno je koristiti i nestandardne biblioteke ukoliko one ne implementiraju stvari usko vezane sa srž projekta u koju spada generisanje mašinskog koda, pratećih metapodataka, formiranje relokacionih zapisa, relociranje i povezivanje predmetnih programa, emulacija itd.

## Ocenjivanje projekta

Zahtevi u okviru zadataka navedenih u nastavku razvrstani su prema težini i obimu na tri nivoa: A (30 poena), B (35 poena) i C (40 poena). Podela zahteva po nivoima za svaki od zadataka definisana je neposredno nakon uvoda posmatranog zadatka. Prilikom odbrane projekta moguće je ostvariti:

- 30 poena, ako i samo ako su tačno urađeni svi zahtevi propisani za nivo A
- 35 poena, ako i samo ako su tačno urađeni svi zahtevi propisani za nivoe A i B
- 40 poena, ako i samo ako su tačno urađeni svi zahtevi propisani za nivoe A, B i C

Ukoliko student ne implementira ispravno sve zahteve propisane za nivo A za svaki od zadataka nije moguće uopšte pristupiti odbrani projekta. Angažovani na predmetu zadržavaju diskreciono pravo da odbranu projekta označe kao neuspešnu ukoliko ustanove da predato rešenje projekta sadrži grešku prilikom izvršavanja ili na bilo koji način odstupa od zahteva projektnog zadatka.

## Predaja projekta

Potrebno je realizovati prethodno opisane alate prema datim zahtevima pod Linux operativnim sistemom na amd64 arhitekturi koristeći C/C++ programski jezik. Potrebni alati su opisani u okviru materijala za predavanja i vežbe. Preporuka je rešenje izrađivati u okviru VMware virtuelne mašine koju je moguće preuzeti sa eLearning platforme. Virtuelna mašina sadrži već instalirane sve neophodne alate a kao integrisano okruženje za razvoj programa moguće je koristiti VS Code koje treba povezati sa GNU toolchain setom.

Podešavanje okruženja potrebnog za uspešno prevođenje izvornog koda i pokretanje programa takođe spada u deo postavke projektnog zadatka. Odbrana rešenja projektnog zadatka vrši se isključivo pod prethodno opisanim okruženjem u okviru virtuelne mašine. Pristup internetu prilikom odbrane rešenja projektnog zadatka biće onemogućen što znači da treba koristiti samo alate dostupne na virtuelnoj mašini.

### Pravila za predaju projekta

Projekat se predaje isključivo kao jedna `.zip` arhiva unutar koje se nalazi samo jedan direktorijum imena `resenje` čiji je sadržaj opisan u nastavku. Unutar direktorijuma `resenje` može se naći (1) opciono makefile ili neka druga skripta za prevođenje rešenja zadatka, (2) opciono poddirektorijum `misc` predviđen za dodatne stvari kao što su `flex` i `bison` ulazne datoteke (izlazne datoteke generisane ovim alatima ne treba nikako predavati jer će one biti generisane) i (3) obavezno sledeća tri poddirektorijuma: `tests`, `src` i `inc`. Sadržaj ovih poddirektorijuma treba da bude sledeći:

- `tests`: ulazne datoteke za prikaz funkcionalnosti rešenja zadatka,
- `src`: sve `.c` i `.cpp` datoteke sa izvornim kodom i
- `inc`: sve `.h` i `.hpp` datoteke sa izvornim kodom.

Opisani sadržaj `.zip` arhive ujedno treba da bude i jedini njen sadržaj. Nije dozvoljeno predavati izvršne datoteke, datoteke generisane od strane alata `flex` i `bison` niti bilo šta drugo što iznad nije eksplicitno navedeno. Nepoštovanje pravila za predaju projekta po pitanju sadržaja, strukture i naziva direktorijuma, povlači negativne poene ili zabranu izlaska na odbranu u datom ispitnom roku.

Prikaz primitivnog primera (rešenje svakako treba da sadrži mnogo veći broj datoteka sa izvornim kodom u odnosu na prikazano) sadržaja `.zip` arhive nalazi se u nastavku:

```text
.
└── resenje
    ├── makefile
    ├── misc
    │   ├── flex
    │   └── bison
    ├── inc
    │   ├── assembler.hpp
    │   ├── linker.hpp
    │   └── emulator.hpp
    ├── src
    │   ├── assembler.cpp
    │   ├── linker.cpp
    │   └── emulator.cpp
    └── tests
        ├── test1.s
        ├── test2.s
        └── test3.s
```

## Zapisnik revizija

Ovaj zapisnik sadrži spisak izmena i dopuna ovog dokumenta po verzijama.

### Verzija 1.1

| Strana | Izmena |
| --- | --- |
|  |  |
