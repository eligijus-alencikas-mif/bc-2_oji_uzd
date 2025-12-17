# 2-oji užduotis: Supaprastintos blokų grandinės (Blockchain) kūrimas

## Aprašas

Šioje programoje simuliujama blokų grandinė. Joje yra aprašyti blokai su transakcijom bei kitom blokui aktuoliomis duomenimis. 
Paleidus programą sukuriami 1000 vartotojų su balansais nuo 100 iki 1000000. Vartotojams sugeneruojamos mokėjimo transakcijos ir tos transakcijos surašomos į bloką ir iškasamos (sugeneruojamas maišos kodas _angl. hash_). Po kasimo atvaizduojamas visa blokų grandinė.

## Maišos algoritmas

Maišos algoritmas buvo pakeistas į FNV-1a algoritmą

## Logika
     
Blokų grandinės logika ir validacija yra apibrėžta __blockchain__ klasėje. Į šią klasę perdavus transakcijų masyvą yra sugeneruojamas blokas.

## kompiliavimas

  1. Sukurkite build aplankalą ir įeikite į jį<br>``mkdir build && cd build``
  2. Paleiskite cmake comandą <br> ``cmake ..``
  3. Išeikite iš aplankalo ir sukompiliuokite programą <br> ``cd .. && cmake --build build``
  4. Pragrama bus build aplankale su pavadinimu "Blockchain"

## naudojimas 

Paleisti Blockchain su šitais konsolės parametrais:
``Blockchain -b <number_of_blocks> -t <number_of_transactions> -d <difficulty_target> -u <number_of_users>``

* ``-b`` specifikuoja kiek blokų bus sugeneruojama iš viso
* ``-t`` specifikuoja kiek "transactions" turės kiekvienas blokas
* ``-d`` specifikuoja blokų kasimo lygį (Kiek 0 yra "hash" pradžioje)
* ``-u`` specifikuoja blokų grandinės naudotojų kiekį
