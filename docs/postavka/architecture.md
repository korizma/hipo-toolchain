## Prilog: Opis računarskog sistema

### Uvod

Apstraktni računarski sistem se sastoji od procesora, operativne memorije, tajmera i terminala. Sve komponente računarskog sistema su međusobno povezane preko sistemske magistrale. Tajmer i terminal su povezani pored sistemske magistrale i direktno sa procesorom preko linija za slanje zahteva za prekid. Uprošćen šematski prikaz posmatranog apstraktnog računarskog sistema prikazan je na sledećoj slici:

```text
              CPU

MEM           TIM

             TERM
```

### Opis procesora

U nastavku je opisan deo 32-bitnog dvoadresnog procesora sa Von-Neuman arhitekturom. Adresibilna jedinica je jedan bajt, a raspored bajtova u reči je little-endian. Veličina memorijskog adresnog prostora je 2^32 B. Nakon inicijalnog odnosno hladnog (engl. cold) restarta isto kao i nakon toplog (engl. warm) restarta procesor počinje da izvršava instrukcije počev od adrese `0x40000000`.

#### Procesorski registri

Procesor poseduje šesnaest opštenamenskih 32-bitnih registara označenih sa `r<num>` gde `<num>` može imati vrednosti od nula do petnaest. Registar `r0` je ožičen na vrednost nula. Registar `r15` se koristi kao `pc` registar. Vrednost registra `r15` sadrži adresu instrukcije koja naredna treba da se izvrši. Registar `r14` se koristi kao `sp` registar. Vrednost registra `r14` sadrži adresu zauzete lokacije na vrhu steka (stek raste ka nižim adresama).

Pored pomenutih opštenamenskih registara postoje sledeći statusni i kontrolni 32-bitni registri: `status` (statusna reč procesora), `handler` (adresa prekidne rutine) i `cause` (uzrok prekida). Statusna reč procesora odnosno `status` registar sastoji se od flegova koji pružaju mogućnost konfiguracije mehanizma prekida. Izgled nižih 16 bita statusne reči procesora jeste:

```text
15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
                                                            I   Tl  Tr
```

Značenje flegova u `status` registru:

- `Tr` (Timer) - maskiranje prekida od tajmera (0 - omogućen, 1 - maskiran),
- `Tl` (Terminal) - maskiranje prekida od terminala (0 - omogućen, 1 - maskiran) i
- `I` (Interrupt) - globalno maskiranje spoljašnjih prekida (0 - omogućeni, 1 - maskirani).

#### Memorijski mapirani registri

Memorijski mapirani registri jesu registri kojima se pristupa instrukcijama za pristup memorijskom adresnom prostoru. Počev od adrese `0xFFFFFF00` memorijskog adresnog prostora nalazi se prostor veličine 256 bajtova rezervisan za memorijski mapirane registre. Memorijski mapirani registri koriste se za rad sa periferijama u računarskom sistemu.

#### Mehanizam prekida

Sistem poseduje samo jednu prekidnu rutinu čija je adresa definisana vrednošću `handler` registra. Uzrok ulaska u datu prekidnu rutinu određen je vrednošću `cause` registra. Moguće vrednosti `cause` registra usled različitih uzroka ulaska u prekidnu rutinu su:

- vrednost 1 u slučaju izvršavanja nekorektne instrukcije (nepostojeći operacioni kod, neispravan način adresiranja itd.),
- vrednost 2 usled pristiglog zahteva za prekid od tajmera (opis principa rada tajmera i način njegove konfiguracije dat je u zasebnom poglavlju),
- vrednost 3 usled pristiglog zahteva za prekid od terminala (opis principa rada terminala dat je u zasebnom poglavlju) i
- vrednost 4 ukoliko je reč o softverskom prekidu.

Svaka mašinska instrukcija procesora izvršava se atomično. Zahtevi za prekid se opslužuju tek nakon što se trenutna mašinska instrukcija atomično izvrši do kraja. Procesor prilikom prihvatanja zahteva za prekid i ulaska u prekidnu rutinu postavlja na stek statusnu reč i povratnu adresu upravo tim redom i zatim globalno maskira prekide.

#### Format procesorskih instrukcija

Svaka instrukcija je veličine četiri bajta. Format instrukcije dat je u nastavku:

```text
I                  II              III             IV
OC       MOD      RegA    RegB    RegC    Disp    Disp    Disp
```

Polje `OC` označava operacioni kod. Vrednost `OC[3:0]` definiše o kojoj mašinskoj instrukciji procesora je reč.

Polje `MOD` označava modifikator instrukcije. Vrednost `MOD[3:0]` govori šta tačno data instrukcija treba da uradi.

Polje `RegX` definiše jedan korišćeni registar instrukcije. Vrednost `RegX[3:0]` specificira korišćeni registar navodeći njegov indeks. Opštenamenskim registrima dodeljeni su indeksi koji odgovaraju njihovim imenima. Indeks registra `r0` je nula, indeks registra `r1` je jedan itd. Kontrolnim i statusnim registrima dodeljeni su indeksi navedeni u nastavku. Indeks registra `status` je nula, indeks registra `handler` je jedan i indeks registra `cause` je dva.

Polje `Disp` predstavlja označeni (engl. signed) pomeraj. Vrednost `Disp[11:0]` instrukcije koriste u izračunavanjima u skladu sa operacijom koju obavljaju.

### Pregled procesorskih instrukcija

#### Instrukcija za zaustavljanje procesora

```text
I                 II             III                IV
7654     3210    7654    3210    7654    3210      7654    3210
0000     0000    0000    0000    0000    0000      0000    0000
```

Zaustavlja procesor kao i dalje izvršavanje narednih instrukcija.

#### Instrukcija softverskog prekida

```text
I                 II             III                IV
7654     3210    7654    3210    7654    3210      7654    3210
0001     0000    0000    0000    0000    0000      0000    0000
```

Generiše softverski zahtev za prekid.

```text
push status; push pc; cause<=4; status<=status&(~0x1); pc<=handle;
```

#### Instrukcija poziva potprograma

```text
I                 II             III                IV
7654     3210    7654    3210    7654    3210      7654    3210
0010     MMMM    AAAA    BBBB    0000    DDDD      DDDD    DDDD
```

Poziva potprogram pre čega sačuva povratnu adresu na steku. Zavisno od modifikatora instrukcija skače na sledeću adresu:

```text
MMMM==0b0000: push pc; pc<=gpr[A]+gpr[B]+D;
MMMM==0b0001: push pc; pc<=mem32[gpr[A]+gpr[B]+D];
```

#### Instrukcija skoka

```text
I                 II             III                IV
7654     3210    7654    3210    7654    3210      7654    3210
0011     MMMM    AAAA    BBBB    CCCC    DDDD      DDDD    DDDD
```

Skače bezuslovno ili uslovno, u skladu sa modifikatorom instrukcije, koristeći zadati pomeraj. Zavisno od modifikatora instrukcija vrši sledeći tip skoka:

```text
MMMM==0b0000: pc<=gpr[A]+D;
MMMM==0b0001: if (gpr[B] == gpr[C]) pc<=gpr[A]+D;
MMMM==0b0010: if (gpr[B] != gpr[C]) pc<=gpr[A]+D;
MMMM==0b0011: if (gpr[B] signed> gpr[C]) pc<=gpr[A]+D;
MMMM==0b1000: pc<=mem32[gpr[A]+D];
MMMM==0b1001: if (gpr[B] == gpr[C]) pc<=mem32[gpr[A]+D];
MMMM==0b1010: if (gpr[B] != gpr[C]) pc<=mem32[gpr[A]+D];
MMMM==0b1011: if (gpr[B] signed> gpr[C]) pc<=mem32[gpr[A]+D];
```

#### Instrukcija atomične zamene vrednosti

```text
I                 II             III               IV
7654     3210    7654    3210    7654    3210     7654    3210
0100     0000    0000    BBBB    CCCC    0000     0000    0000
```

Zamenjuje vrednost dva registra atomično bez mogućnosti da zamena bude prekinuta usled asinhronog zahteva za prekid.

```text
temp<=gpr[B]; gpr[B]<=gpr[C]; gpr[C]<=temp;
```

#### Instrukcija aritmetičkih operacija

```text
I                 II             III               IV
7654     3210    7654    3210    7654    3210     7654    3210
0101     MMMM    AAAA    BBBB    CCCC    0000     0000    0000
```

Vrši odgovarajuću aritmetičku operaciju, u skladu sa modifikatorom instrukcije, nad vrednostima u zadatim registrima. Zavisno od modifikatora instrukcija obavlja operaciju:

```text
MMMM==0b0000: gpr[A]<=gpr[B] + gpr[C];
MMMM==0b0001: gpr[A]<=gpr[B] - gpr[C];
MMMM==0b0010: gpr[A]<=gpr[B] * gpr[C];
MMMM==0b0011: gpr[A]<=gpr[B] / gpr[C];
```

#### Instrukcija logičkih operacija

```text
I                 II             III               IV
7654     3210    7654    3210    7654    3210     7654    3210
0110     MMMM    AAAA    BBBB    CCCC    0000     0000    0000
```

Vrši odgovarajuću logičku operaciju, u skladu sa modifikatorom instrukcije, nad vrednostima u zadatim registrima. Zavisno od modifikatora instrukcija obavlja operaciju:

```text
MMMM==0b0000: gpr[A]<=~gpr[B];
MMMM==0b0001: gpr[A]<=gpr[B] & gpr[C];
MMMM==0b0010: gpr[A]<=gpr[B] | gpr[C];
MMMM==0b0011: gpr[A]<=gpr[B] ^ gpr[C];
```

#### Instrukcija pomeračkih operacija

```text
I                 II             III               IV
7654     3210    7654    3210    7654    3210     7654    3210
0111     MMMM    AAAA    BBBB    CCCC    0000     0000    0000
```

Vrši odgovarajuću pomeračku operaciju, u skladu sa modifikatorom instrukcije, nad vrednostima u zadatim registrima. Zavisno od modifikatora instrukcija obavlja operaciju:

```text
MMMM==0b0000: gpr[A]<=gpr[B] << gpr[C];
MMMM==0b0001: gpr[A]<=gpr[B] >> gpr[C];
```

#### Instrukcija smeštanja podatka

```text
I                 II             III               IV
7654     3210    7654    3210    7654    3210     7654    3210
1000     MMMM    AAAA    BBBB    CCCC    DDDD     DDDD    DDDD
```

Smešta podatak u memoriju. Zavisno od modifikatora instrukcija obavlja operaciju:

```text
MMMM==0b0000: mem32[gpr[A]+gpr[B]+D]<=gpr[C];
MMMM==0b0010: mem32[mem32[gpr[A]+gpr[B]+D]]<=gpr[C];
MMMM==0b0001: gpr[A]<=gpr[A]+D; mem32[gpr[A]]<=gpr[C];
```

#### Instrukcija učitavanja podatka

```text
I                 II             III               IV
7654     3210    7654    3210    7654    3210     7654    3210
1001     MMMM    AAAA    BBBB    CCCC    DDDD     DDDD    DDDD
```

Učitava podatak u registar. Zavisno od modifikatora instrukcija obavlja operaciju:

```text
MMMM==0b0000: gpr[A]<=csr[B];
MMMM==0b0001: gpr[A]<=gpr[B]+D;
MMMM==0b0010: gpr[A]<=mem32[gpr[B]+gpr[C]+D];
MMMM==0b0011: gpr[A]<=mem32[gpr[B]]; gpr[B]<=gpr[B]+D;
MMMM==0b0100: csr[A]<=gpr[B];
MMMM==0b0101: csr[A]<=csr[B]|D;
MMMM==0b0110: csr[A]<=mem32[gpr[B]+gpr[C]+D];
MMMM==0b0111: csr[A]<=mem32[gpr[B]]; gpr[B]<=gpr[B]+D;
```

Sve kombinacije instrukcija i operanada, za koje ne postoji razumno tumačenje, proglasiti greškom.

### Opis terminala

Terminal predstavlja ulazno/izlaznu periferiju koja se sastoji od displeja (izlaz) i tastature (ulaz). Terminal poseduje dva programski dostupna registra `term_out` i `term_in` kojima se pristupa kroz memorijski adresni prostor (memorijski mapirani registri). Navedeni programski dostupni registri mapirani su u memorijski adresni prostor na sledeći način:

| Registar | Opseg adresa |
| --- | --- |
| `term_out` | `[0xFFFFFF00-0xFFFFFF03]` |
| `term_in` | `[0xFFFFFF04-0xFFFFFF07]` |

Registar `term_out` predstavlja registar izlaznih podataka. Terminal prati upise u ovaj registar i prilikom svakog upisa vrednosti u `term_out` registar ispisuje znak na displej. Ispisani znak određen je sadržajem ASCII tabele za vrednost koja je upisana u `term_out` registar.

Registar `term_in` predstavlja registar ulaznih podataka. Terminal vrši sledeće dve operacije svaki put kada se pritisne bilo koje dugme na tastaturi: (1) upisuje ASCII kod pritisnutog tastera u `term_in` registar kako bi čitanjem njegove vrednosti bilo moguće ustanoviti koje dugme je pritisnuto i (2) generiše zahtev za prekid. Dve prethodno opisane operacije terminal vrši čim se pritisne bilo koje dugme; pri čemu treba naglasiti da terminal nipošto ne čeka terminator unosa odnosno enter dugme. Terminal ne vrši echo pritisnutog dugmeta odnosno ne prikazuje ga na displeju. Za potrebe implementacije ove funkcionalnosti emulatora moguće je, na primer, koristiti `<termios.h>` zaglavlje. Ukoliko korisnički program ne pročita vrednost `term_in` registra na vreme, odnosno pre nego što korisnik pritisne naredno dugme, ASCII kod prethodno pritisnutog dugmeta biće bespovratno izgubljen. Ne treba vršiti baferisanje ASCII kodova u okviru implementacije. Smatrati da je procesor posmatranog računarskog sistema dovoljno brz da garantovano, ukoliko je korisnički program ispravno napisan, može da stigne da pročita vrednost `term_in` registra u prekidnoj rutini pre nego što ona bude pregažena.

### Opis tajmera

Tajmer predstavlja periferiju koja periodično generiše zahtev za prekid. Tajmer poseduje jedan programski dostupan registar `tim_cfg` kojem se pristupa kroz memorijski adresni prostor (memorijski mapiran registar). Navedeni programski dostupan registar mapiran je u memorijski adresni prostor na sledeći način:

| Registar | Opseg adresa |
| --- | --- |
| `tim_cfg` | `[0xFFFFFF10-0xFFFFFF13]` |

Registar `tim_cfg` predstavlja konfiguracioni registar tajmera. Perioda generisanja zahteva za prekid od strane tajmera definisana je vrednošću `tim_cfg` registra na sledeći način: `0x0 -> 500ms`, `0x1 -> 1000ms`, `0x2 -> 1500ms`, `0x3 -> 2000ms`, `0x4 -> 5000ms`, `0x5 -> 10s`, `0x6 -> 30s` i `0x7 -> 60s`. Inicijalna vrednost `tim_cfg` registra, nakon pokretanja odnosno resetovanja emuliranog računarskog sistema, jeste `0x00000000`.

## Primer korisničkog programa

Program prikazan u nastavku predstavlja primer programa napisanog na asembleru opisanom u prvom zadatku. Izvorni asemblerski kod programa je razdvojen na dve tekstualne datoteke `handler.s` i `main.s`. Program obavlja sledeće radnje:

- postavlja adresu prekidne rutine,
- usled prekida od terminala ispisuje se na displeju znak koji odgovara pritisnutom dugmetu,
- usled prekida od tajmera ispisuje se na displeju znak `T`,
- konfiguriše tajmer tako da generiše zahtev za prekid na svaku sekundu i
- čeka da korisnik pritisne pet puta dugme pre nego što zaustavi procesor.

### `handler.s`

```asm
.equ term_out, 0xFFFFFF00
.equ term_in, 0xFFFFFF04
.equ ascii_code, 84 # ascii('T')
.extern my_counter
.global handler
.section my_code_handler
handler:
   push %r1
   push %r2
   csrrd %cause, %r1
   ld $2, %r2
   beq %r1, %r2, my_isr_timer
   ld $3, %r2
   beq %r1, %r2, my_isr_terminal
# obrada prekida od tajmera
my_isr_timer:
   ld $ascii_code , %r1
   st %r1, term_out
   jmp finish
# obrada prekida od terminala
my_isr_terminal:
   ld term_in, %r1
   st %r1, term_out
   ld my_counter, %r1
   ld $1, %r2
   add %r2, %r1
   st %r1, my_counter
finish:
   pop %r2
   pop %r1
   iret
.end
```

### `main.s`

```asm
.equ tim_cfg, 0xFFFFFF10
.equ init_sp, 0xFFFFFF00
.extern handler
.section my_code_main
   ld $init_sp, %sp
   ld $handler, %r1
   csrwr %r1, %handler
   ld $0x1, %r1
   st %r1, tim_cfg
wait:
   ld my_counter, %r1
   ld $5, %r2
   bne %r1, %r2, wait
   halt
.global my_counter
.section my_data
my_counter:
   .word 0
.end
```

Komande za prevođenje, povezivanje i emuliranje ovog primera jesu:

```console
./asembler -o handler.o handler.s
./asembler -o main.o main.s
./linker -hex
       -place=my_code_main@0x40000000
       -place=my_code_handler@0xC0000000
       -o program.hex handler.o main.o
./emulator program.hex
```

