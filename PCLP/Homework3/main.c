/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>

#include "function.h"
#include "interface.h"
#include "read.h"
#include "print.h"
#include "rotate.h"
#include "kernel.h"
#define NMAX 50

struct global_image {
	char type[3];
	int width, height;
	int max_value, x_axis, y_axis;
	int **red;
	int **green;
	int **blue;
};

struct temporary {
	int x1, x2, y1, y2;
};

void assign(struct temporary *coord, int x1, int y1, int x2, int y2)
{
	coord->x1 = x1;
	coord->y1 = y1;
	coord->x2 = x2;
	coord->y2 = y2;
}

int main(void)
{
	char name_of_file[NMAX], parameter[NMAX], file[NMAX];
	struct global_image image;
	struct temporary coord;
	int file_type_determine = 0, type, h1_stars = 0, h2_bins = 0;
	int angle = 0, count = 0;
	int x1, x2, y1, y2;
	FILE *fptr = NULL;
	while (1) {
		int oppp = operation_identifier(count, name_of_file, &x1,
										&y1, &x2, &y2, &h1_stars, &h2_bins,
										&angle, parameter, file, &type, &image);
	    switch (oppp) {
		case 1: {
			file_type_determine = file_type(fptr, name_of_file);
			if (file_type_determine == 0 && count != 0) {
			    free_global_matrix(&image);
				count = 0;
			}
			if (file_type_determine != 0) {
				file_reader_first_version(&count, name_of_file,
										  file_type_determine,
										  fptr, &image, &x1, &y1, &x2, &y2);
			}
			break;
		}
		case 2://idk, it works
			printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
			break;
		case 3:
			printf("Selected ALL\n");
			break;
		case 4:
			histogram_function(image, h1_stars, h2_bins);
			break;
		case 5:
			equalize_function(&image);
			break;
		case 6:
			assign(&coord, x1, y1, x2, y2);
			rotate_function_helper(&image, angle, &x1, &y1, &x2, &y2, coord);
			break;
		case 7:
			crop_function(&image, &x1, &y1, &x2, &y2);
			break;
		case 8:
			kernel_interface_helper(&image, parameter, x1, y1, x2, y2);
			break;
		case 9:///works
			file_printer_for_tests(&type, file, image);
			break;
		case 10:
			(count != 0) ? free_global_matrix(&image) :
			printf("No image loaded\n");
			return 0;
		case 11:
			printf("No image loaded\n");
			break;
		default:
			break;
		}
	}
	return 0;
}
