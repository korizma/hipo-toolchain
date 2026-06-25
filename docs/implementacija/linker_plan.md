# Plan za izradu linkera

Linker ima dva moda:
- `hex`
- `relocatable`

## `relocatable` mod

Ovaj mod izbacuje datoteku istog formata kao i asembler.

Jedino radi kada nema konflikta u imenima simbola. To ukljucuje i sekcije. Treba adekvatno javiti gresku.

Efektivno sabira vise sym tabela.

Posebne situacije

**Eksterni simbol postaje poznat simbol (equ)**

Ako simbol koji nije bio poznat jer je bio `extern` postane poznat onda sve relokacije treba da se razrese.

Izlaz je ista datoteka kao i za asm.

Moze se koristiti iste funkcije kao i za asm.

## `hex` mod

Ovaj mod izbacuje executable datoteku.

Treba odraditi:

- Poredjati sekcije
    - Istoimene iz drugih fajlova se redjaju jedna za drugom
    - Ucitavaju se u memoriju kao sto su fajlovi navedeni u komandi
    - Neke sekcije mogu da se smeste na odredjenu adresu
- Razresiti simbole
- Odraditi sve relokacije

Greske su ako:
- Simbol ime konflikt (ne sekcije)
- Ima nerazresenih simbola
- Preklapanja zbog opcije -place

## Potencijalan tok koda:

Prva stvar koju sam primetio da moze da se iskoristi za oba jeste da oba moda moraju da ukombinuju tabele simbola.
To je jedini presek.

Dodatne stvari koje ce trebati za `relocatable` mod jeste da se nakon sabiranja sym tabela razrese sve relokacije novih equ simbola.

Stvari za `hex` mod su:

Redjanje sekcija u memoriju.
Razresiti simbole.
Odraditi sve relokacije.

## Prvi koraci

- Smisliti strukture koje ce mi trebati.
- Da li cu da pisem u C ili C++, za sada mi ima smisla pisati u C++ ali proceduralno.
- Refaktorisati asembler u C++ tako da moze da se njegove neke funkcionalnosti koriste u linkeru.
    - Preveliki cukanje overhead je sam cist C, zbog nedinamickih nizova itd.
