# Simulatore di macchina RAM

## A chi è rivolto
A chi vuole scrivere ed eseguire programmi in linguaggio RAM (me inizialmente).

## Cos'è una macchina RAM?
Un modello molto semplificato di calcolo che può leggere da uno stream di input, fare elaborazioni interne e stampare i risultati su uno stream di output.

## Come uso il tuo programma?
Apri il file "main.c" e modifica i campi `V` e `Programma` rispettivamente con l'input che vuoi dare in pasto alla macchina e il programma che vuoi eseguire. Lancia poi il comando `make run`.

## Quanto è robusto il tuo programma?
Poco, puoi eseguire programmi RAM sintatticamente o semanticamente errati (ma ci sto lavorando). Ti puoi aspettare segfault quando accedi ad un registro con indice negativo o troppo grande (>= NREG). Puoi sempre però controllare il campo `state` della struttura `Ram` per capire meglio. 
