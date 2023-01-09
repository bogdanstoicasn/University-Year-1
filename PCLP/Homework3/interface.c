/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "interface.h"
#include "function.h"

#define NMAX 50

struct global_image {
	char type[3];
	int width, height, max_value, x_axis, y_axis;
	int **red, **red_crop;
	int **green, **green_crop;
	int **blue, **blue_crop;
};

//this function just ignores comments
void ignore_comments(FILE *fp)
{
	int ch;
	char line[100];
	int ok = 0;
	// Ignore any blank lines
	while ((ch = fgetc(fp)) != EOF && isspace(ch))
		ok++;

	// recursively ignore comments
	// start with a '#'
	if (ch == '#') {
		fgets(line, sizeof(line), fp);
		ignore_comments(fp);
	} else {
		fseek(fp, -1, SEEK_CUR);
	}
}

// discovers the photo type
// working as intended
int file_type(FILE *fptr, char s[NMAX])
{
	char determine_type[3];
	fptr = fopen(s, "r");
	if (!fptr) {
		printf("Failed to load %s\n", s);
		return 0;
	}
	ignore_comments(fptr);
	fscanf(fptr, "%s", determine_type);
	if (determine_type[0] != 'P') {
		fclose(fptr);
		printf("Failed to load %s\n", s);
		return 0;
	}
	if (determine_type[1] >= '7' || determine_type[1] <= '1' ||
		determine_type[1] == '4' || determine_type[2] != 0) {
		fclose(fptr);
		printf("Failed to load %s\n", s);
		return 0;
	}
	switch (determine_type[1]) {
	case '2':
		fclose(fptr);
		return 2;
	case '3':
		fclose(fptr);
		return 3;
	case '5':
		fclose(fptr);
		return 5;
	case '6':
		fclose(fptr);
		return 6;
	}
	fclose(fptr);
	return 0;
}

int operation_identifier(int count, char s[NMAX],
						 int *x1, int *y1, int *x2, int *y2, int *h1, int *h2,
						 int *angle, char parametre[NMAX], char file[NMAX],
						 int *type, struct global_image *image)
{
	char string[NMAX], copy_string[NMAX];
	fgets(string, NMAX, stdin);
	for (size_t i = 0; i < strlen(string); i++)
		if (string[i] == '\n')
			string[i] = '\0';

	strcpy(copy_string, string);

	char *p = strtok(string, " ");

	///load
	if (strcmp(p, "LOAD") == 0)
		return load_function_identifier(copy_string, s);

	/// select + we use select_function_identifier
	if (strcmp(p, "SELECT") == 0)
		return refurbished_select_identifier(count, copy_string, image,
											 x1, y1, x2, y2);

	///histogram + we use histogram_function_identifier
	if (strcmp(p, "HISTOGRAM") == 0)
		return histogram_function_identifier(count, copy_string, image, h1, h2);

	if (strcmp(p, "ROTATE") == 0)
		return rotate_function_identifier(count, copy_string, image, angle);

	if (strcmp(p, "EQUALIZE") == 0)
		return equalize_function_identifier(count, image);

	/// crop
	if (strcmp(p, "CROP") == 0) {
		if (count == 0)
			return 11;
		return 7;
	}

	if (strcmp(p, "APPLY") == 0)
		return apply_function_identifier(count, copy_string, parametre, image);

	/// save + we use save_function_identifier
	if (strcmp(p, "SAVE") == 0) {
		if (count == 0)
			return 11;
		return save_function_identifier(copy_string, file, type, image);
	}

	/// exit
	if (strcmp(p, "EXIT") == 0)
		return 10;

	printf("Invalid command\n");
	return 0;
}

int load_function_identifier(char string[NMAX], char s[NMAX])
{
	char *p = strtok(string, " ");
	int ok = 0;
	while (p) {
		ok++;
		if (ok == 2)
			strcpy(s, p);
		p = strtok(NULL, " ");
	}
	if (ok == 1 || ok >= 3) {
		printf("Invalid command\n");
		return 0;
	}
	return 1;
}

int histogram_function_identifier(int count, char string[NMAX],
								  struct global_image *image, int *h1, int *h2)
{
	char *p = strtok(string, " ");
	int contor = 0, ch1 = -1, ch2 = -1, ch3 = -1, ok = 0;
	while (p) {
		if (p[0] >= 'A' && p[0] <= 'Z') {
			ok++;
		} else {
			if (contor == 0)
				ch1 = atoi(p);

			if (contor == 1)
				ch2 = atoi(p);

			if (contor == 2)
				ch3 = atoi(p);
			contor++;
		}
	p = strtok(NULL, " ");
	}

	if (count == 0)
		return 11;

	if (ch3 != -1) {
		printf("Invalid command\n");
		return 0;
	}
	if (ch1 == -1 || ch2 == -1 || ch2 == 0 || ch1 == 0) {
		printf("Invalid command\n");
		return 0;
	}
	if (image->type[1] == '3' || image->type[1] == '6') {
		printf("Black and white image needed\n");
		return 0;
	}
	if (ch2 < 2 || ch2 > 256 || (ch2 & (ch2 - 1)) != 0) {
		printf("Invalid set of parameters\n");
		return 0;
	}
	*h1 = ch1;
	*h2 = ch2;
	return 4;
}

int save_function_identifier(char string[NMAX], char file[NMAX],
							 int *type, struct global_image *image)
{
	char *p = strtok(string, " ");
	int ok = 0;
	while (p) {
		ok++;
		if (ok == 2)
			strcpy(file, p);
		p = strtok(NULL, " ");
	}

	*type = image->type[1] - '0';
	if (ok == 3) {
		if (*type == 5) {
			*type = 2;
			return 9;
		}
		if (*type == 6) {
			*type = 3;
		return 9;
		}
		return 9;
	}
	if (*type == 2) {
		*type = 5;
		return 9;
	}
	if (*type == 3) {
		*type = 6;
		return 9;
	}
	return 9;
}

int rotate_function_identifier(int count, char string[NMAX],
							   struct global_image *image, int *angle)
{
	char *p = strtok(string, " ");
	p = strtok(NULL, " ");
	int copy_angle = -1;
	while (p) {
		copy_angle = atoi(p);
		for (size_t i = 0; i < strlen(p); i++) {
			if ((p[i] < '0' || p[i] > '9') && p[i] != '-') {
				printf("Invalid command\n");
				return 0;
				}
			}
		p = strtok(NULL, " ");
	}
	if (copy_angle == -1) {
		printf("Invalid command\n");
		return 0;
	}

	if (count == 0)
		return 11;

	*angle = copy_angle;
	switch (*angle) {
	case 90:
		break;
	case -90:
		break;
	case 180:
		break;
	case -180:
		break;
	case 270:
		break;
	case -270:
		break;
	case 360:
		break;
	case -360:
		break;
	case 0:
		break;
	default:
		printf("Unsupported rotation angle\n");
		return 0;
	}

	int count_select = image->height * image->width;
	int count_image = image->x_axis * image->y_axis;

	if (image->x_axis == image->y_axis)
		return 6;
	if (count_image == count_select)
		return 6;

	printf("The selection must be square\n");
	return 0;
}

int equalize_function_identifier(int count, struct global_image *image)
{
	if (count == 0)
		return 11;
	if (image->type[1] == '3' || image->type[1] == '6') {
		printf("Black and white image needed\n");
		return 0;
	}
	return 5;
}

int apply_function_identifier(int count, char string[NMAX],
							  char parameter[NMAX],
							  struct global_image *image)
{
	if (count == 0)
		return 11;
	char *p = strtok(string, " ");
	int ok = 0;
	while (p) {
		ok++;
		if (ok == 2)
			strcpy(parameter, p);
		p = strtok(NULL, " ");
	}

	if (ok == 1 || ok >= 3) {
		printf("Invalid command\n");
		return 0;
	}
	if (strcmp(parameter, "EDGE") == 0 ||
		strcmp(parameter, "SHARPEN") == 0 ||
	    strcmp(parameter, "BLUR") == 0 ||
	    strcmp(parameter, "GAUSSIAN_BLUR") == 0) {
		if (image->type[1] == '2' || image->type[1] == '5') {
			printf("Easy, Charlie Chaplin\n");
			return 0;
		}

		return 8;
	}
	printf("APPLY parameter invalid\n");
	return 0;

	return 8;
}

int refurbished_select_identifier(int count, char string[NMAX],
								  struct global_image *image,
								  int *x1, int *y1, int *x2, int *y2)
{
	if (count == 0)
		return 11;

	char *p = strtok(string, " ");
	int cpx1 = -1, cpx2 = -1, cpy1 = -1, cpy2 = -1;
	p = strtok(NULL, " ");
	if (strcmp(p, "ALL") == 0) {
		*x1 = 0;
		*x2 = image->width;
		*y1 = 0;
		*y2 = image->height;
		image->x_axis = *x2;
		image->y_axis = *y2;
		return 3;
	}

	int cnt = 0, nr = 0;
	while (p) {
		cnt++;
		for (int i = 0; i < (int)strlen(p); i++) {
			if ((p[i] < '0' || p[i] > '9') && p[i] != '-') {
				printf("Invalid command\n");
				return 0;
			}
		}
		nr = atoi(p);
		switch (cnt) {
		case 1:
			cpx1 = nr;
			break;
		case 2:
			cpy1 = nr;
			break;
		case 3:
			cpx2 = nr;
			break;
		case 4:
			cpy2 = nr;
			break;
		default:
			printf("Invalid command\n");
			return 0;
		}
		p = strtok(NULL, " ");
	}
	if (cnt != 4) {
		printf("Invalid command\n");
		return 0;
	}
	swap_function(&cpx1, &cpx2);
	swap_function(&cpy1, &cpy2);
	if (cpx1 < 0 || cpx2 < 0 || cpy1 < 0 || cpy2 < 0) {
		printf("Invalid set of coordinates\n");
		return 0;
	} else if (cpx2 > image->width || cpy2 > image->height) {
		printf("Invalid set of coordinates\n");
		return 0;
	} else if (cpx1 == cpx2 || cpy2 == cpy1) {
		printf("Invalid set of coordinates\n");
		return 0;
	}
	*x1 = cpx1; *x2 = cpx2; *y1 = cpy1; *y2 = cpy2;
	image->x_axis = *x2 - *x1; image->y_axis = *y2 - *y1;
	return 2;

	//return 0;
}
