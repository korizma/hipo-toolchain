# Testovi

Sta zelim sve da pokrijem:

Testovi samo za asembler:

- Provera svake greske koje moze da se desi u asembleru
    - Nedefinisan simbol
    - Pogresni Equ simboli
    - Definicija simbola vise puta
    - Deklaracija direktiva ili instrukcija van sekcije
- Provera da li dobro racuna equ simbole
    - Sa simbolima
    - Bez simbola
    - Sa mnogo simbola iz vise sekcija
- Provera da li relokacioni simboli imaju smisla 


Testovi za linker, relocatable deo:

- Provera da li dobro linkuje sve
    - Da li su razreseni dobro globalni simobli
    - Da li se konflikti simbola dobro gledaju
    - Da li su se sekcije lepo spojile
    - Da li moze da se opet linkuje taj fajl
- Greske
    - Konflikti simbola
    - Neciji extern je local u drugom fajlu

Ocekivana struktura za svaki test:
(folder) ime_testa
- asm fajlovi
- bin fajlovi
- sh skripta za runnovanje
- md fajl sta se ocekuje i koja je svrha testa, detaljno ali kratko objasni sta se desava