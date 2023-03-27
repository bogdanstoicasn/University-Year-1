#### Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
# README check

## Descriere

Acesta este un README care contine detalii si explicatii despre tema 2,
care are numele de "Simulator de octave".


## Particularitati

Fisierele sunt: main.c, function.c, function.h.
Se folosesc bibliotecile: stdio.h, stdlib.h, math.h.
Codul este compilat cu standardul din 1999.
Pot aparea definiri de constante(exemplu: NMAX cu valoarea 10007).


## Simulator de octave(my_octave)

Prima idee pentru tema aceasta ar fi fost folosirea unui "triplu pointer", insa 
trebuia sa utilizez si alti vectori pentru a retine anumite variabile.
Asadar, am ales abordarea cu vectori de structuri, pentru ca structura poate 
stoca multe date utile pentru problema.

Alocarea dinamica a vectorului este relativ simpla, necesitand doar "realloc", 
nefiind importanta operatia(inmultire, citire, Strassen). Partea mai muncitoreasca
apare la functia de alocare a matricei, deoarece trebuia sa ne asiguram functia 
prin programare defensiva. 

Prima problema s-a ivit chiar in "inmultirea matricelor". Daca faceam inmultire,
operatie de eliminare a unui element, si dupa ordonare, programul meu ajungea 
intr-o zona de memorie care nu ii apartinea. Am rezolvat aceasta problema prin
simplificarea functiei de inmultire("matrix_multiplication"): am scos conditia 
care verifica daca numarul de coloane din prima matrice este egal cu numarul 
de linii din a doua matrice, si am lasat doar inmultirea, care se petrece in
conditii optime. Verific conditia de mai sus direct in fisierul "main.c",astfel
programul meu ruland fara probleme.

Eliminarea unui element se face prin stergerea din memorie("free_matrix") a 
matricei corespunzatoare si deplasarea cu o pozitie inapoi, 
a elementelor ce il urmeaza.

Operatia de ordonare:
	-se foloseste o sortare cu O(n^2) pasi;
	-se interschimba valorile vectorului de structuri cand se indeplineste 
	conditia ceruta;

Cerintele de redimensionare si transpunere a unei matrici sunt abordate la fel:
	-se ia un dublu **pointer** catre adresa de memorie a matricei care trebuie 
	modificata;
	-se creeaza o matrice noua, modificata in alta locatie de memorie;
	-se leaga, noua matrice modificata, de pozitia pe care se afla matricea veche, 
	in vector;
	-se elibereaza memoria alocata pentru matricea veche prin variabila **pointer**

Algoritmul Strassen:
	
	"Strassen_algorithm_helper"
	-mai intai se trateaza cazul banal, cand matricea ajunge la dimensiunea:
	o linie, o coloana;
	-altfel se impart, recursiv, matricele ce trebuie inmultite, in 4 cadrane din
	ce in ce mai mici, pana se ajunge la cazul banal;
	-apoi se fac inmultirile si se obtine matricea finala;

	"Strassen_algorithm"
	-se copiaza matricele de inmultit;
	-se aplica "Strassen_algorithm_helper";
	-matricea obtinuta se trece in a treia matrice;
