## Zadatak 3: Emulator

### Uvod

Cilj ovog zadatka jeste realizacija interpretativnog emulatora za računarski sistem opisan u prilogu. Ulaz emulatora jeste datoteka za inicijalizaciju memorije dobijena kao izlaz linkera uz navedenu `-hex` opciju komandne linije. Emulacija je moguća samo ukoliko je ulaznu datoteku moguće uspešno učitati u memorijski adresni prostor emuliranog računarskog sistema. Nakon pokretanja emulatora jedini ispis u konzoli jeste ispis direktno iz korisničkog programa, dok implementacija ne ispisuje ništa samostalno do završetka emulacije. Emulacija se završava u onom trenutku kada emulirani procesor izvrši `halt` instrukciju korisničkog programa. Nakon završetka emulacije implementacija ispisuje stanje emuliranog procesora u sledećem formatu:

```text
-----------------------------------------------------------------
Emulated processor executed halt instruction
Emulated processor state:
 r0=0x00000000    r1=0x00000000    r2=0x00000000    r3=0x00000000
 r4=0x00000000    r5=0x00000000    r6=0x00000000    r7=0x00000000
 r8=0x00000000    r9=0x00000000   r10=0x00000000   r11=0x00000000
r12=0x00000000   r13=0x00000000   r14=0x00000000   r15=0x00000000
```

### Podela zahteva po nivoima

Podela zahteva po nivoima za ovaj zadatak definisana je u nastavku. Za nivo A potrebno je emulirati celokupan posmatrani računarski sistem osim periferija terminal i tajmer. Za nivo B, pored svega definisanog nivoom A, potrebno je emulirati i periferiju terminal. Za nivo C, pored svega definisanog nivoom B, potrebno je emulirati i periferiju tajmer.

### Pokretanje iz terminala

Rezultat prevođenja implementacije ovog zadatka treba da ima `emulator` za naziv. Sve informacije potrebne za izvršavanje zadaju se kao argumenti komandne linije. Jednim pokretanjem emulator emulira jedno izvršavanje programa iz ulazne datoteke. Naziv ulazne datoteke, koja predstavlja izlaz linkera uz navedenu `-hex` opciju komandne linije, zadaje se kao samostalni argument komandne linije. Način pokretanja emulator jeste sledeći:

```console
emulator <naziv_ulazne_datoteke>
```

#### Primer pokretanja

Primer komande, kojom se pokreće emulacija na osnovu zapisa za inicijalizaciju memorije u ulaznoj datoteci `mem_content.hex`, dat je u nastavku:

```console
./emulator mem_content.hex
```

