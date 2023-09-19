#### Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#### GNU GENERAL PUBLIC LICENSE 
#### Version 3, 29 June 2007
# README check

## Description

This is a README that contains details and explanations about Theme 3, which is named "Image Editor."

## Characteristics

Files *.c: kernel.c, function.c, interface.c, rotate.c, print.c, main.c
Headers: kernel.h, function.h, interface.h, rotate.h, print.h
The code is compiled with c99.

## Image editor(image_editor)

The topic addressed is the development of a simple image editor for the terminal,
without using the graphical image manipulation tool, NETPBM.


The key points of the topic are:

> Image reading, which can be in binary/ASCII format ("read.c").

> Determining the command and its parameters ("interface.c").

> The cropping function ("function.c").

> Applying image kernels for BLUR, GAUSSIAN BLUR, EDGE SHARPEN ("kernel.c").

> Rotating the image by the chosen angle ("rotate.c").

> Saving the image according to its type ("print.c").

The difference between an ASCII image and a binary image begins with the pixel reading,
but up to that point, the image type, length, width, and maximum pixel value are in ASCII format.
To store pixel values in a matrix of numbers, we read from ASCII format files using an "int" variable,
and from binary format files, we read bit by bit and convert to "int".
PGM images store values in the "red" matrix, while PPM images store values in the "red," "green," and "blue" matrices.
The same structure is used for both types of images.

The first challenges were encountered when determining the commands, non-existent commands, as well as handling special cases.
Therefore, for each command, a function was used that handles all possible cases and returns a corresponding number.

The crop function takes the coordinates stored in "select" and creates a new matrix, which replaces the old image matrix.


Applying image kernels:

> It is done on the current selection.

> New matrices are allocated for the values obtained after the operation.

> The values are then transferred into the image matrix.

Image rotation is performed by a function that can rotate regardless of the width and height of the file.
Due to the crop command, this operation accesses unknown memory areas.
This issue has been resolved by swapping the values in the selection, thus creating a function that can be used in future programs as well.

Saving the image:

> Determining the type of file to write and opening the corresponding file.

> Writing the data to the file, including the image type, width, height, maximum value, and pixels.
	


## Resources:

->The C Programming Language. 2nd Edition
	by Brian Kernighan and Dennis Ritchie
	
->[Kernels](https://youtube.com/playlist?list=PLzH6n4zXuckoRdljSlM2k35BufTYXNNeF)

->[Computerphile](https://youtube.com/@Computerphile)
	*check his account ;)*
	
->[NETPBM](https://netpbm.sourceforge.net/)





