#### Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
# README check

## Description

This is a README that contains details and explanations about assignment 2,
which is named "Octave Simulator."


## Specifics

Files: main.c, function.c, function.h.
The code is compiled with the c99 standard.


## Octave simulator(my_octave)

The initial idea for this assignment would have been to use a 'triple pointer,'
but I needed to use other arrays to store certain variables.
Therefore, I opted for the approach with arrays of structures because a structure can
store a lot of useful data for the problem.

The dynamic allocation of the vector is relatively simple, requiring only 'realloc,'
with the operation (multiplication, reading, Strassen) not being critical.
The more labor-intensive part comes in the matrix allocation function, as we needed to ensure the function
through defensive programming.

The first issue arose during 'matrix multiplication.' If I performed multiplication, the operation of removing an element,
and then sorting, my program would end up in a memory area that didn't belong to it. I solved this problem by simplifying the
'matrix_multiplication' function: I removed the condition that checks if the number of columns in the first matrix is equal
to the number of rows in the second matrix, and I left only the multiplication, which occurs under optimal conditions.
I directly check the above condition in the 'main.c' file, allowing my program to run without problems.

The removal of an element is done by freeing the memory ('free_matrix') of the corresponding matrix and shifting
back by one position the elements that follow it.

The sorting operation:
	-it uses a sorting method with O(n^2) steps.
	-it swaps the values of the structure vector when the required condition is met.

The requirements for resizing and transposing a matrix are addressed as follows:

	-a **double pointer** to the memory address of the matrix that needs modification is taken.
	-a new, modified matrix is created in another memory location.
	-the new modified matrix is linked to the position where the old matrix is in the vector.
	-the memory allocated for the old matrix is released through **the pointer** variable

The Strassen Algorithm:

	'Strassen_algorithm_helper':

		-first, the base case is handled when the matrix reaches a size of one row and one column.
		-otherwise, the matrices to be multiplied are recursively divided into four increasingly smaller quadrants until the base case is reached.
		-then, the multiplications are performed, resulting in the final matrix.
	
 	'Strassen_algorithm':

		-the matrices to be multiplied are copied.
		-'Strassen_algorithm_helper' is applied.
		-the resulting matrix is passed into a third matrix.
