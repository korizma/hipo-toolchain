# Opis implementacije asemblera

## Tabela simbola

### Statusi ulaza u Tabelu simbola

`STS_COMPLETE` 
- simboli koju su u tom trenutku ucitani preko labele, `simbol: ...`
- simboli koji su `extern`
- simboli koji su imena sekcija

`STS_REFERENCE` 
- simbol je referisan u nekoj direktivi ili instrukciji bez prethodnog znacenja

`STS_GLOBAL_REFERENCE` 
- simbol je deklarisan kao `global`

`STS_EQU`
- simbol je deklarisan preko `equ`, obrada se radi na kraju