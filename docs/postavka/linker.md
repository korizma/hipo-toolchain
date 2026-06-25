## Zadatak 2: Linker

### Uvod

Cilj ovog zadatka jeste realizacija linkera nezavisnog od ciljne arhitekture koji na osnovu metapodataka (tabela simbola, relokacioni zapisi itd.) vrši povezivanje jednog ili više predmetnih programa generisanih od strane asemblera iz prvog zadatka.

Ulaz linkera je izlaz asemblera pri čemu je moguće zadati veći broj predmetnih programa koje je potrebno povezati. Linker podrazumevano smešta sekcije, počevši od nulte adrese, jednu odmah iza druge onim redom kako su definisane unutar predmetnog programa. Veći broj predmetnih programa na svom ulazu linker obrađuje u redosledu njihovog navođenja preko komandne linije. Prilikom smeštanja sekcije istog imena kao prethodno smeštena sekcija dolazi do njenog umetanja počev od mesta gde se istoimena sekcija ranije završila, stvarajući time agregaciju istoimenih sekcija, pri čemu nema preklapanja sa sekcijama sledbenicama što se postiže njihovim guranjem ka višim adresama.

Izlaz linkera je tekstualna datoteka sa sadržajem u skladu sa opisom u nastavku (dozvoljeno je generisati kao izlaz, pored tekstualne datoteke, binarnu datoteku radi jednostavnijeg učitavanja izlaza linkera u emulator).

### Podela zahteva po nivoima

Podela zahteva po nivoima za ovaj zadatak tiče se opcija komandne linije (ostale zahteve podrazumevano treba implementirati). Nivo u sklopu kojeg treba implementirati neku opciju komandne linije naveden je u uglastim zagradama ispred tražene opcije komandne linije.

### Pokretanje iz terminala

Rezultat prevođenja implementacije ovog zadatka treba da ima `linker` za naziv. Sve informacije potrebne za izvršavanje zadaju se kao argumenti komandne linije. Jednim pokretanjem linker vrši povezivanje jedne ili više ulaznih datoteka. Nazivi ulaznih datoteka, koje predstavljaju predmetne programe, zadaju se kao samostalni argumenti komandne linije. Način pokretanja linker jeste sledeći:

```console
linker [opcije] <naziv_ulazne_datoteke>...
```

#### Opcije komandne linije

Opis opcija komandne linije, zajedno sa opisom njihovih parametara, koje mogu biti zadate prilikom pokretanja linker u proizvoljnom redosledu nalazi se u nastavku:

**[nivo A] `-o <naziv_izlazne_datoteke>`**

Opcija komandne linije `-o` postavlja svoj parametar `<naziv_izlazne_datoteke>` za naziv izlazne datoteke koja predstavlja rezultat povezivanja.

**[nivo A] `-place=<ime_sekcije>@<adresa>`**

Opcija komandne linije `-place` eksplicitno definiše adresu počev od koje se smešta sekcija zadatog imena pri čemu su adresa i ime sekcije određeni `<ime_sekcije>@<adresa>` parametrom. Ovu opciju moguće je navoditi više puta za različita imena sekcija kako bi se definisala adresa za veći broj sekcija iz ulaznih datoteka. Sve sekcije za koje ova opcija nije navedena smeštaju se na podrazumevani način, opisan u sklopu uvoda ovog zadatka, počev odmah iza kraja sekcije koja je smeštena na najvišu adresu.

**[nivo A] `-hex`**

Opcija komandne linije `-hex` predstavlja smernicu linkeru da kao rezultat povezivanja generiše zapis, na osnovu kojeg se može izvršiti inicijalizacija memorije, u vidu skupa parova (adresa, sadržaj). Sadržaj predstavlja mašinski kod koji treba da se nađe na zadatoj adresi. Parovi se generišu samo za one adrese na koje treba smestiti sadržaj sa definisanom početnom vrednošću. Format zapisa, na primeru u kojem je početna vrednost sadržaja jednaka njegovoj adresi, prikazan je u nastavku:

```text
0000: 00 01 02 03 04 05 06 07
0008: 08 09 0A 0B 0C 0D 0E 0F
0010: 10 11 12 13 14 15 16 17
```

Povezivanje je moguće samo u slučaju da ne postoje (1) višestruke definicije simbola, (2) nerazrešeni simboli i (3) preklapanja između sekcija iz ulaznih predmetnih programa kada se uzmu u obzir `-place` opcije komandne linije. Ukoliko za zadate ulazne datoteke linkera nije ispunjen neki od prethodnih uslova linker mora da prijavi grešku uz odgovarajuću poruku. Nazivi simbola ili sekcija koji su uzrok greške treba da budu deo poruke o grešci.

Prilikom pokretanja linkera navođenje tačno jedne od `-relocatable` i `-hex` opcija komandne linije je obavezno. Linker ne treba da generiše nikakav izlaz ako nije navedena tačno jedna od dve prethodno navedene opcije komandne linije.

**[nivo B] `-relocatable`**

Opcija komandne linije `-relocatable` predstavlja smernicu linkeru da kao rezultat povezivanja generiše predmetni program, istog formata kao i izlaz asemblera, u kojem se sve sekcije smeštaju takođe od nulte adrese (potpuno se ignorišu potencijalno navedene `-place` opcije komandne linije). Predmetni program dobijen na ovakav način može kasnije biti naveden kao ulaz linkera.

Povezivanje je moguće samo u slučaju da nema višestrukih definicija simbola. Ukoliko za zadate ulazne datoteke linkera postoji višestruka definicija simbola linker mora da prijavi grešku uz odgovarajuću poruku. Naziv višestruko definisanog simbola treba da bude deo poruke o grešci.

Prilikom pokretanja linkera navođenje tačno jedne od `-relocatable` i `-hex` opcija komandne linije je obavezno. Linker ne treba da generiše nikakav izlaz ako nije navedena tačno jedna od dve prethodno navedene opcije komandne linije.

#### Primer pokretanja

Primer komande, kojom se pokreće povezivanje predmetnih programa `ulaz1.o` i `ulaz2.o` pri čemu se (1) definišu adrese na koje se smeštaju odgovarajuće sekcije i (2) zahteva generisanje zapisa za inicijalizaciju memorije, dat je u nastavku:

```console
./linker -hex
    -place=data@0x4000F000 -place=text@0x40000000
    -o mem_content.hex
    ulaz1.o ulaz2.o
```

