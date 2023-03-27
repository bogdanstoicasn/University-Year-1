#### Copyright 2022 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
# README check


## Descriere

Acesta este un README care contine detalii si explicatii despre tema 1,care este compusa din 3 probleme: infinite_product, simple_queries,gigel_and_the_checkboard, another_queries.


## Particularitati
Se folosesc bibliotecile: stdio.h, math.h.
Pot aparea definiri de constante (exemplu:"NMAX"). 


## Infinite product

Problema este rezolvata cu ajutorul a 3 fisiere: "main_prod.c", "function_prod.c", "function_prod.h". Fisierul "function_prod.c" contine functiile necesare: functie care calculeaza produsul scalar("scalar_product") si functie care calculeaza norma("norm_sum"). "main_prod.c" contine programul propriu-zis, si incepe prin declararea si initializarea variabilelor cu 0 (suma scalara si normele) sau -1 (primele maxime si maximele secunde). 

	PAS 1: Se citeste numarul total de perechi(variabila n).

	PAS 2: Se deschide o structura repetitiva cu numar cunoscut de pasi 

	PAS 3: Se citeste pereche cu pereche si este prelucrata imediat ce s-a facut citirea(citirea se face in baza 8).

	PAS 4: Se apeleaza functia de calculare a produsului scalar.

	PAS 5: Se depisteaza elemetul maxim si urmatorul element maxim pentru ambele numere din pereche.

	PAS 6: Se apeleaza functia de calculare a normei pentru ambele elemente din pereche.

	PAS 7: Se calculeaza norma finala cu ajutorul bibliotecii "math.h".

	PAS 8: Afisarile propriu-zise: produs scalar, maxime secunde(sau -1 daca nu exista) si normele(precizie de 7 zecimale).




## Simple queries

Programul este compus din 3 fisiere: "main_simple.c", "function_simple.c", "function_simple.h". "function_simple.c" contine functiile: 
	
	1. Cel mai mare divizor comun a doua numere(cmmdc). 
	
	2. Calculator pentru indexul celui mai mare element intr-un vector("index_calculator").
	
	3. Printarea numarului de aparitii al unei litere introduse de la tastatura("scan_Q").
	
	4. Eliminarea numarului de aparitii al unei litere introduse la tastatura("delete_E").
	
	5. Afisarea primelor k elemente in ordine aparitiilor("scan_T").
	
	6. Cazul de eroare("error_case") (mai putin de 5 caractere de la ultima eroare si cel putin 2 caractere diferite in toata citirea de elemente)

Fisierul principal contine contor pentru toate caracterele litere mici(count_all_char), contor pentru aparitia erorii(count_error), variabila de adevar pentru caractere diferite(ok) si un vector care memoreaza frecventa literelor mici.
	
	PAS 1: Initializarea vectorului cu 0.
	
	PAS 2: Citirea si copierea primului caracter(se copiaza in variabila "copy").
	
	PAS 3: Deschiderea unei structuri repetitive care se va incheia la introducerea unui caracter diferit de litera mica, Q, T sau E.
	
	PAS 4.1: Daca caracterul citit e litera mica creste frecventa literei, contorul de caractere totale si contorul de eroare.
	
	PAS 4.2: Daca caracterul citit este Q, T sau E se aplica una din functiile: "scan_Q", "scan_T" sau "delete_E".
	
	PAS 5.1: Daca caracterele(litere mici) sunt diferite se aplica functia "error_case", iar daca eroare se afisaza counter_error ia valoare 0("error_case" returneaza 0).
	
	PAS 6: Citirea unui nou caracter si verificare cu precedentul(litere mici si diferite).
	
	

	 
## Gigel_and_the_checkboard

Problema este rezolvata cu ajutorul a 3 fisiere:"main_checkboard.c", "function_checkboard.c", "function_checkboard.h". "function_checkboard.c" contine functiile:
	
	1. Citeste tabla sub forma de matrice patratica si returneaza numarul de linii/coloane("scanf_matrix").
	
	2. Calculator pentru ultima pozitie a piesei de pe tabla si al distantei parcurse("pos_distance"): 
		-structura repetitiva care functioneaza cat timp elementul este diferit de 0
		-valoare(variabila value) care retine elementul de pe pozitia piesei
		-pozitia piesei ia valoarea 0
		-mutare pe linie sau pe coloana in functie de pozitie si de value
		-cazuri speciale, in care value trece de numarul de linii/coloane
		-distanta parcursa
	
	3. Functie recursiva care afisaza litere mari in functie de numarul introdus("print_index").
	
In fisierul principal se declara numarul de linii/coloane(n), matricea(matrix) si un vector, care retine linie/coloana/distanta in aceasta ordine(x_y_distance).
	
	PAS 1: Initializarea vectorului x_y_distance cu 0.
	
	PAS 2: Apelarea functiei "pos_distance".
	
	PAS 3: Afisarile: distanta, numar de linii, numar de coloane sub forma de litera mare (se apeleaza "print_index" care afisaza caractere)
	

	
## Another_queries

Programul este compus din 3 fisiere: "main_another.c", "function_another.c", "function_another.h". "function_another.c" contine functiile:
	
	1. Citirea matricei("scan_matrix")
	
	2. Verficator care arata daca un numar se afla pe linie("check_number_line")
	
	3. Verficator care arata daca un numar se afla pe coloana("check_number_coloumn")
	
	4. Verficator care arata daca un numar se afla in caseta, stiind numarul casetei("check_numer_box"):
		-functia primeste numarul casetei, care mai apoi e transformat in coordonatele primului element din caseta.
		-se fac 2 structuri repetitive cu numar cunoscut de pasi in care se verifica aparitia numarului 
		-returneaza 0(apare 1 data) sau 1(nu apare)
	
	5. Verficator care arata daca un numar se afla in caseta, daca stim linia si coloana("check_numer_box_by_line_coloumn"): 
		-primeste ca parametrii linia si coloana, care sunt transformate in coordonatele primului element din caseta
		-se fac 2 structuri repetitive cu numar cunoscut de pasi in care se verifica aparitia numarului 
		-returneaza 1(apare minim o data) sau 0(nu apare)
	
	6. Verificator daca un numar se poate pune la coordonatele respective("check_numer_cell"):
		-daca nu este pe linie, nici pe coloana, nici in careu, atunci numarul se poate pune
	
	7. Punerea unui element in casuta goala("change_cell")
	
	8. Verificator de stare a tabelului("table_state"):
		-verifica daca elemente se repeta pe linii
		-verifica daca elemente se repeta pe coloane
		-verifica daca elemente se repeta in caseta
		-verifica daca mai exista pozitii goale
		-returneaza 2 daca ai castigat, returneaza 1 pentru tabel invalid, returneaza 0 daca mai poti adauga elemente
	
	9.Verifica pentru fiecare element daca poate fi pus pe linia dorita si afiseaza 1 sau 0("scan1")(citire+2)
	
	10.Verifica pentru fiecare element daca poate fi pus pe coloana dorita si afiseaza 1 sau 0("scan2")(citire+3)
	
	11.Verifica pentru fiecare element daca poate fi pus in caseta dorita si afiseaza 1 sau 0("scan3")(citire+4)
	
	12.Verifica pentru fiecare element daca poate fi pus la coordonatele dorite si afiseaza 1 sau 0("scan4")(citire+5) 
	
	13. Pune elementul dorit la coordonatele dorite daca acestea sunt libere("scan5") (citire+6)
	
	
In fisierul principal se "main_another.c" se declara variabilele: numarul de linii/coloane(n), numarul de pasi(m) si matricea(matrix).

	PAS 1: Se citesc numarul de linii/coloane, numarul de pasi si matricea.
	
	PAS 2: Se deschide o structura repetitiva care se incheie la terminarea numarului de pasi.
	
	PAS 3: Se decrementeaza numarul de pasi si se citeste operatia.
	
	PAS 4: Se efectueaza operatia dorita.
