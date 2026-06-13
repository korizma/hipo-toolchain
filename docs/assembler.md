## Zadatak 1: Asembler

### Uvod

Cilj ovog zadatka jeste realizacija jednoprolaznog asemblera za procesor opisan u prilogu. Ulaz asemblera je tekstualna datoteka sa izvornim asemblerskim kodom napisanim u skladu sa sintaksom opisanom u nastavku. Izlaz asemblera je tekstualna datoteka koja predstavlja predmetni program (dozvoljeno je generisati kao izlaz, pored tekstualne datoteke, binarnu datoteku radi jednostavnijeg učitavanja izlaza asemblera u linker).

Format predmetnog programa bazirati na školskoj varijanti ELF formata čiji je referentni primer tekstualna datoteka kakva je korišćena na vežbama u delu gradiva koje se tiče konstrukcije asemblera. Dozvoljeno je praviti izmene u školskoj varijanti ELF formata (sekcije, tipovi zapisa o relokacijama, dodatna polja u postojećim tipovima zapisa, novi podaci o predmetnom programu i slično) ukoliko je to neophodno u skladu sa potrebama ciljne arhitekture. Prilikom razrešavanja svih nedefinisanih detalja za potrebe rešenja voditi se principima koje koristi GNU asembler.

### Podela zahteva po nivoima

Podela zahteva po nivoima za ovaj zadatak tiče se asemblerskih direktiva i naredbi (ostale zahteve podrazumevano treba implementirati). Nivo u sklopu kojeg treba implementirati neku asemblersku direktivu naveden je u uglastim zagradama ispred tražene asemblerske direktive. Nivo u sklopu kojeg treba implementirati neku asemblersku naredbu naveden je unutar reda odgovarajuće tabele za posmatranu asemblersku naredbu.

### Pokretanje iz terminala

Rezultat prevođenja implementacije ovog zadatka treba da ima `asembler` za naziv. Sve informacije potrebne za izvršavanje zadaju se kao argumenti komandne linije. Jednim pokretanjem asembler vrši asembliranje jedne ulazne datoteke. Naziv ulazne datoteke sa izvornim asemblerskim kodom zadaje se kao samostalni argument komandne linije. Način pokretanja asembler jeste sledeći:

```console
asembler [opcije] <naziv_ulazne_datoteke>
```

#### Opcije komandne linije

Opis opcija komandne linije, zajedno sa opisom njihovih parametara, koje mogu biti zadate prilikom pokretanja asembler nalazi se u nastavku:

```console
-o <naziv_izlazne_datoteke>
```

Opcija komandne linije `-o` postavlja svoj parametar `<naziv_izlazne_datoteke>` za naziv izlazne datoteke koja predstavlja rezultat asembliranja.

#### Primer pokretanja

Primer komande, kojom se inicira asembliranje izvornog koda u okviru datoteke `ulaz.s` sa ciljem dobijanja `izlaz.o` predmetnog programa, dat je u nastavku:

```console
./asembler -o izlaz.o ulaz.s
```

### Sintaksa izvornog asemblerskog koda

Sintaksa izvornog asemblerskog koda može se grubo podeliti na opšte detalje, asemblerske direktive i asemblerske naredbe. Opšti detalji definišu izgled jedne linije izvornog koda po pitanju zapisa labela, asemblerskih naredbi, asemblerskih direktivi, komentara itd. Asemblerska direktiva predstavlja operaciju koju asembler treba da izvrši u toku asembliranja. Asemblerska naredba predstavlja simbolički zapis mašinskih instrukcija koji asembler treba da prevede u binarnu reprezentaciju.

#### Opšti detalji

Opšti detalji, predstavljeni u vidu (1) funkcionalnih zahteva koje asembler treba da ispuni i (2) sintaksnih pravila za koja asembler treba da proveri da li su ispoštovana, navedeni su redom po stavkama u nastavku:

- jedna linija izvornog koda sadrži najviše jednu asemblersku naredbu ili direktivu,
- zakomentarisan sadržaj se ignoriše u potpunosti prilikom asembliranja,
- jednolinijski komentar, koji se implicitno završava na kraju linije, započinje `#` karakterom,
- labela, čija se definicija završava dvotačkom, mora se naći na samom početku linije izvornog koda (opciono nakon proizvoljnog broja belih znakova) i
- labela može da stoji i samostalno, bez prateće asemblerske naredbe ili asemblerske direktive u istoj liniji izvornog koda, što je ekvivalentno tome da stoji na samom početku prve naredne linije izvornog koda koja ima sadržaj.

#### Asemblerske direktive

**[nivo A] `.global <lista_simbola>`**

Izvozi simbole navedene u okviru liste parametara. Lista parametara može sadržati samo jedan simbol ili više njih razdvojenih zapetama.

**[nivo A] `.extern <lista_simbola>`**

Uvozi simbole navedene u okviru liste parametara. Lista parametara može sadržati samo jedan simbol ili više njih razdvojenih zapetama.

**[nivo A] `.section <ime_sekcije>`**

Započinje novu asemblersku sekciju, čime se prethodno započeta sekcija automatski završava, proizvoljnog imena navedenog kao parametar asemblerske direktive.

**[nivo A] `.word <lista_simbola_ili_literala>`**

Alocira prostor fiksne veličine po četiri bajta za svaki inicijalizator (simbol ili literal) naveden u okviru liste parametara. Lista parametara može sadržati samo jedan inicijalizator ili više njih razdvojenih zapetama. Asemblerska direktiva alocirani prostor inicijalizuje vrednošću navedenih inicijalizatora.

**[nivo A] `.skip <literal>`**

Alocira prostor čija je veličina jednaka broju bajtova definisanom literalom navedenim kao parametar. Asemblerska direktiva alocirani prostor inicijalizuje nulama.

**[nivo B] `.ascii <string>`**

Alocira prostor fiksne veličine po jedan bajt za svaki karakter stringa (niska karaktera između znakova navoda). Asemblerska direktiva alocirani prostor inicijalizuje vrednostima koje odgovaraju navedenim karakterima prema ASCII tabeli.

**[nivo C] `.equ <novi_simbol>, <izraz>`**

Definiše novi simbol čija je vrednost jednaka navedenom izrazu.

**[nivo A] `.end`**

Završava proces asembliranja ulazne datoteke. Ostatak ulazne datoteke se odbacuje odnosno ne vrši se njegovo asembliranje.

#### Asemblerske naredbe

| Format | Efekat | Nivo |
| --- | --- | --- |
| `halt` | Zaustavlja izvršavanje instrukcija | A |
| `int` | Izaziva softverski prekid | A |
| `iret` | `pop pc; pop status;` | A |
| `call operand` | `push pc; pc <= operand;` | A |
| `ret` | `pop pc;` | A |
| `jmp operand` | `pc <= operand;` | A |
| `beq %gpr1, %gpr2, operand` | `if (gpr1 == gpr2) pc <= operand;` | A |
| `bne %gpr1, %gpr2, operand` | `if (gpr1 != gpr2) pc <= operand;` | A |
| `bgt %gpr1, %gpr2, operand` | `if (gpr1 signed> gpr2) pc <= operand;` | A |
| `push %gpr` | `sp <= sp - 4; mem32[sp] <= gpr;` | A |
| `pop %gpr` | `gpr <= mem32[sp]; sp <= sp + 4;` | A |
| `xchg %gprS, %gprD` | `temp <= gprD; gprD <= gprS; gprS <= temp;` | A |
| `add %gprS, %gprD` | `gprD <= gprD + gprS;` | A |
| `sub %gprS, %gprD` | `gprD <= gprD - gprS;` | A |
| `mul %gprS, %gprD` | `gprD <= gprD * gprS;` | A |
| `div %gprS, %gprD` | `gprD <= gprD / gprS;` | A |
| `not %gpr` | `gpr <= ~gpr;` | A |
| `and %gprS, %gprD` | `gprD <= gprD & gprS;` | A |
| `or %gprS, %gprD` | `gprD <= gprD \| gprS` | A |
| `xor %gprS, %gprD` | `gprD <= gprD ^ gprS;` | A |
| `shl %gprS, %gprD` | `gprD <= gprD << gprS;` | A |
| `shr %gprS, %gprD` | `gprD <= gprD >> gprS;` | A |
| `ld operand, %gpr` | `gpr <= operand;` | A |
| `st %gpr, operand` | `operand <= gpr;` | A |
| `csrrd %csr, %gpr` | `gpr <= csr` | A |
| `csrwr %gpr, %csr` | `csr <= gpr;` | A |

Oznaka `gprX` predstavlja oznaku nekog od programski dostupnih opštenamenskih registara ciljne arhitekture. Programski dostupni opštenamenski registri su `r0`, `r1`, `r2`, `r3`, `r4`, `r5`, `r6`, `r7`, `r8`, `r9`, `r10`, `r11`, `r12`, `r13`, `r14`/`sp` i `r15`/`pc`.

Oznaka `csrX` predstavlja oznaku nekog od programski dostupnih kontrolnih i statusnih registara ciljne arhitekture. Programski dostupni kontrolni i statusni registri su: `status`, `handler` i `cause`.

Oznaka `operand` obuhvata sve sintaksne notacije za navođenje operanada. Sintaksne notacije se razlikuju zavisno od toga da li se radi o asemblerskim naredbama za rad sa podacima ili asemblerskim naredbama skoka.

Asemblerske naredbe za rad sa podacima podržavaju različite sintaksne notacije za operand, opisane u nastavku, kojima se definiše vrednost podatka:

- `$<literal>` - vrednost `<literal>`
- `$<simbol>` - vrednost `<simbol>`
- `<literal>` - vrednost iz memorije na adresi `<literal>`
- `<simbol>` - vrednost iz memorije na adresi `<simbol>`
- `%<reg>` - vrednost u registru `<reg>`
- `[%<reg>]` - vrednost iz memorije na adresi `<reg>`
- `[%<reg> + <literal>]` - vrednost iz memorije na adresi `<reg> + <literal>`[^1]
- `[%<reg> + <simbol>]` - vrednost iz memorije na adresi `<reg> + <simbol>`[^2]

Asemblerske naredbe skoka i poziva potprograma podržavaju različite sintaksne notacije za operand, opisane u nastavku, kojima se definiše vrednost odredišne adrese skoka:

- `<literal>` - vrednost `<literal>`
- `<simbol>` - vrednost `<simbol>`

[^1]: Ukoliko vrednost literala nije moguće zapisati na širini od 12 bita kao označenu vrednost prijaviti grešku u procesu asembliranja.
[^2]: Ukoliko konačna vrednost simbola nije poznata u trenutku asembliranja ili konačnu vrednost simbola nije moguće zapisati na širini od 12 bita kao označenu vrednost prijaviti grešku u procesu asembliranja.

