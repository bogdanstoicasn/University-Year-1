#### Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#### GNU GENERAL PUBLIC LICENSE 
#### Version 3, 29 June 2007
# README check

## Descriere

Acesta este un README care contine detalii si explicatii despre tema 3,
care are numele de "Editor de imagini".

## Particularitati

Fisiere *.c: kernel.c, function.c, interface.c, rotate.c, print.c, main.c
Headere: kernel.h, function.h, interface.h, rotate.h, print.h
Codul este compilat cu standardul din 1999.
Pot aparea definiri de constante(exemplu: NMAX cu valoarea 50).


## Editor de imagini(image_editor)

Tema abordata este construirea unui editor simplu de imagini, pentru terminal,
fara a utiliza unealta de manipulare a imaginilor grafice, NETPBM.


Punctele cheie ale temei sunt:
> citirea imagini , care poate fi in format binary/ASCII ("read.c")
> determinarea comenzii si a parametrilor ei ("interface.c")
> functia de crop("function.c")
> aplicare kernelului de imagine pentru BLUR, GAUSSIAN BLUR, EDGE
SHARPEN ("kernel.c")
> rotirea imagini dupa unghiul ales ("rotate.c")
> salvarea imaginii in functie de tip ("print.c")

Este folosita doar alocare dinamica pe matrice.

Diferenta dintre o imagine ASCII si o imagine binary incepe de la 
citirea pixelilor, insa pana acolo, tipul de imagine, lungimea, latimea si
valoarea maxima a pixelilor sunt in format ASCII. Pentru a retine valorile
pixelilor intr-o matrice de numere, din fisierele format ASCII citim cu o 
variabila de tip "int", iar din fisierele binare citim cu "unsigned char", din
bit in bit, si transformam in "int".
Imaginile PGM retin valorile in matricea "red", iar imaginile PPM retin
valorile in matricele "red", "green", "blue". Se foloseste aceeasi structura
pentru ambele imagini.


Primele probleme au fost intampinate la determinarea comenzilor,
comenzilor inexistente, precum si determinarea cazurilor speciale. Astfel,
pentru fiecare comanda am folosit o functie care abordeaza toate cazurile
posibile si returneaza un numar corespunzator.


Functia de crop ia coordonatele retinute in select si creeaza o noua matrice,
care este trecuta in locul vechii matrice de imagine.


Aplicarea nucleelor de imagini:
> se face pe selectia curenta

> se aloca matrici noi pentru valorile obtinute dupa operatie

> se trec valorile in matricea imaginii

Rotirea imaginii este executata de o functie care roteste indiferent de
latimea si lungimea fisierului. Din cauza comenzii crop, aceasta operatie
accesa zone de memorie necunoscute. Am rezolvat aceast probleme prin
intershimbarea valorilor din selectie, astfel realizand o functie care poate
fi utilizata si in programe scrise in viitor.

Salvarea imaginii:
> determinarea tipului de fisier in care trebuie sa scriem si
deschiderea corespunzatoare

> scrirea datelor in fisier: tip de imagine,latime, inaltime,
valoare maxima, pixeli
	


## Resurse/materiale citite:

->The C Programming Language. 2nd Edition
	by Brian Kernighan and Dennis Ritchie
	
->[Kernels](https://youtube.com/playlist?list=PLzH6n4zXuckoRdljSlM2k35BufTYXNNeF)

->[Computerphile](https://youtube.com/@Computerphile)
	*check his account ;)*
	
->[NETPBM](https://netpbm.sourceforge.net/)





