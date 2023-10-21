/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include "function.h"
#include "rotate.h"

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

// we rotate by type and angle with switch
void rotate_function_helper(struct global_image *image, int angle,
							int *x1, int *y1, int *x2, int *y2,
							struct temporary coord)
{
	if (image->type[1] == '2' || image->type[1] == '5') {
		switch (angle) {
		case 90:
			clockwise_rotation_90(image, x1, y1, x2, y2, coord);
			printf("Rotated 90\n"); break;
		case -90:
			counter_clockwise_rotation_90(image, x1, y1, x2, y2, coord);
			printf("Rotated -90\n"); break;
		case 180:
			clockwise_rotation_90(image, x1, y1, x2, y2, coord);
			clockwise_rotation_90(image, x1, y1, x2, y2, coord);
			printf("Rotated 180\n"); break;
		case -180:
			counter_clockwise_rotation_90(image, x1, y1, x2, y2, coord);
			counter_clockwise_rotation_90(image, x1, y1, x2, y2, coord);
			printf("Rotated -180\n"); break;
		case 270:
			counter_clockwise_rotation_90(image, x1, y1, x2, y2, coord);
			printf("Rotated 270\n"); break;
		case -270:
			clockwise_rotation_90(image, x1, y1, x2, y2, coord);
			printf("Rotated -270\n"); break;
		case 360:
			printf("Rotated 360\n"); break;
		case -360:
			printf("Rotated -360\n"); break;
		case 0:
			printf("Rotated 0\n"); break;
		default:
			printf("Unsupported rotation angle\n"); break;
		}
	}
	if (image->type[1] == '3' || image->type[1] == '6') {
		switch (angle) {
		case 90:
			clockwise_rotation_90_color(image, x1, y1, x2, y2, coord);
			printf("Rotated 90\n"); break;
		case -90:
			counter_clockwise_rotation_90_color(image, x1, y1, x2, y2, coord);
			printf("Rotated -90\n"); break;
		case 180:
			clockwise_rotation_90_color(image, x1, y1, x2, y2, coord);
			clockwise_rotation_90_color(image, x1, y1, x2, y2, coord);
			printf("Rotated 180\n"); break;
		case -180:
			counter_clockwise_rotation_90_color(image, x1, y1, x2, y2, coord);
			counter_clockwise_rotation_90_color(image, x1, y1, x2, y2, coord);
			printf("Rotated -180\n"); break;
		case 270:
			counter_clockwise_rotation_90_color(image, x1, y1, x2, y2, coord);
			printf("Rotated 270\n"); break;
		case -270:
			clockwise_rotation_90_color(image, x1, y1, x2, y2, coord);
			printf("Rotated -270\n"); break;
		case 360:
			printf("Rotated 360\n"); break;
		case -360:
			printf("Rotated -360\n"); break;
		case 0:
			printf("Rotated 0\n"); break;
		default:
			printf("Unsupported rotation angle\n");
			break;
		}
	}
}

// rotate -90 degrees PGM image
void counter_clockwise_rotation_90(struct global_image *image,
								   int *x1, int *y1, int *x2, int *y2,
								   struct temporary coord)
{
	// we do the rotation in the alloc bellow then
	// if rotate all image => we create new matrix for it
	// else we just put the rotated selection in it's place
	int **output = alloc_matrix(image->x_axis, image->y_axis);
	if (!output) {
		printf("Failed to rotate\n");
		return;
	}
	for (int i = 0; i < image->y_axis; i++)
		for (int j = 0; j < image->x_axis; j++)
			output[image->x_axis - j - 1][i] =
				image->red[i + coord.y1][j + coord.x1];

	int tmp = image->x_axis;
	image->x_axis = image->y_axis;
	image->y_axis = tmp;

	// necessary when all the image is rotated
	int aux = *x1;
	*x1 = *y1;
	*y1 = aux;

	int change = *x2;
	*x2 = *y2;
	*y2 = change;

	int count_image = image->width * image->height;
	int count_select = image->x_axis * image->y_axis;

	// check if all image or selection with count_select and count_image
	if (count_image == count_select) {
		free_matrix(image->height, image->red);

		image->red = alloc_matrix(image->width, image->height);

		int aux = image->width;
		image->width = image->height;
		image->height = aux;

		for (int i = 0; i < image->height; i++)
			for (int j = 0; j < image->width; j++)
				image->red[i][j] = output[i][j];

	} else {
		for (int i = coord.y1; i < coord.y2; i++)
			for (int j = coord.x1; j < coord.x2; j++)
				image->red[i][j] = output[i - coord.y1][j - coord.x1];
	}
	free_matrix(image->y_axis, output);
}

// rotate 90 degrees PGM image
void clockwise_rotation_90(struct global_image *image,
						   int *x1, int *y1, int *x2, int *y2,
						   struct temporary coord)
{
	// we do the rotation in the alloc bellow then
	// if rotate all image => we create new matrix for it
	// else we just put the rotated selection in it's place
	int **output = alloc_matrix(image->x_axis, image->y_axis);
	if (!output) {
		printf("Failed to rotate\n");
		return;
	}
	for (int i = 0; i < image->y_axis; i++)
		for (int j = 0; j < image->x_axis; j++)
			output[j][image->y_axis - 1 - i] =
				image->red[i + coord.y1][j + coord.x1];

	int tmp = image->x_axis;
	image->x_axis = image->y_axis;
	image->y_axis = tmp;

	// necessary when all the image is rotated
	int aux = *x1;
	*x1 = *y1;
	*y1 = aux;

	int change = *x2;
	*x2 = *y2;
	*y2 = change;

	int count_image = image->width * image->height;
	int count_select = image->x_axis * image->y_axis;

	// check if all image or selection with count_select and count_image
	if (count_image == count_select) {
		free_matrix(image->height, image->red);

		image->red = alloc_matrix(image->width, image->height);

		int aux = image->width;
		image->width = image->height;
		image->height = aux;

		for (int i = 0; i < image->height; i++)
			for (int j = 0; j < image->width; j++)
				image->red[i][j] = output[i][j];
	} else {
		for (int i = coord.y1; i < coord.y2; i++)
			for (int j = coord.x1; j < coord.x2; j++)
				image->red[i][j] = output[i - coord.y1][j - coord.x1];
	}
	free_matrix(image->y_axis, output);
}

// rotate -90 degrees PPM image
void counter_clockwise_rotation_90_color(struct global_image *image,
										 int *x1, int *y1, int *x2, int *y2,
										 struct temporary coord)
{
	// we do the rotation in the alloc bellow
	int **output = alloc_matrix(image->x_axis, image->y_axis);
	int **output_b = alloc_matrix(image->x_axis, image->y_axis);
	int **output_g = alloc_matrix(image->x_axis, image->y_axis);
	if (!output) {
		printf("Failed to rotate\n");
		return;
	}
	if (!output_b) {
		free_matrix(image->x_axis, output);
		printf("Failed to rotate\n");
		return;
	}
	if (!output_g) {
		free_matrix(image->x_axis, output);
		free_matrix(image->x_axis, output_b);
		printf("Failed to rotate\n");
		return;
	}
	for (int i = 0; i < image->y_axis; i++)
		for (int j = 0; j < image->x_axis; j++) {
			output[image->x_axis - j - 1][i] =
				image->red[i + coord.y1][j + coord.x1];
			output_b[image->x_axis - j - 1][i] =
				image->blue[i + coord.y1][j + coord.x1];
			output_g[image->x_axis - j - 1][i] =
				image->green[i + coord.y1][j + coord.x1];
		}
	int tmp = image->x_axis;
	image->x_axis = image->y_axis;
	image->y_axis = tmp;

	// necessary when all the image is rotated
	int aux = *x1;
	*x1 = *y1;
	*y1 = aux;

	int change = *x2;
	*x2 = *y2;
	*y2 = change;

	int count_image = image->width * image->height;
	int count_select = image->x_axis * image->y_axis;

	// check if all image or selection the count_select and count_image
	if (count_image == count_select) {
		free_matrix(image->height, image->red);
		free_matrix(image->height, image->blue);
		free_matrix(image->height, image->green);

		image->red = alloc_matrix(image->width, image->height);
		image->green = alloc_matrix(image->width, image->height);
		image->blue = alloc_matrix(image->width, image->height);

		int aux = image->width;
		image->width = image->height;
		image->height = aux;

		for (int i = 0; i < image->height; i++)
			for (int j = 0; j < image->width; j++) {
				image->red[i][j] = output[i][j];
				image->blue[i][j] = output_b[i][j];
				image->green[i][j] = output_g[i][j];
			}
	} else {
		for (int i = coord.y1; i < coord.y2; i++) {
			for (int j = coord.x1; j < coord.x2; j++) {
				image->red[i][j] = output[i - coord.y1][j - coord.x1];
				image->blue[i][j] = output_b[i - coord.y1][j - coord.x1];
				image->green[i][j] = output_g[i - coord.y1][j - coord.x1];
			}
		}
	}
	free_matrix(image->y_axis, output);
	free_matrix(image->y_axis, output_b);
	free_matrix(image->y_axis, output_g);
}

// rotate 90 degrees PPM image
void clockwise_rotation_90_color(struct global_image *image,
								 int *x1, int *y1, int *x2, int *y2,
								 struct temporary coord)
{
	// we do the rotation in the alloc bellow
	int **output = alloc_matrix(image->x_axis, image->y_axis);
	int **output_b = alloc_matrix(image->x_axis, image->y_axis);
	int **output_g = alloc_matrix(image->x_axis, image->y_axis);
	if (!output) {
	    printf("Failed to rotate\n");
	    return;
	}
	if (!output_b) {
		free_matrix(image->x_axis, output);
		printf("Failed to rotate\n");
		return;
	}
	if (!output_g) {
		free_matrix(image->x_axis, output);
		free_matrix(image->x_axis, output_b);
		printf("Failed to rotate\n");
		return;
	}
	for (int i = 0; i < image->y_axis; i++)
		for (int j = 0; j < image->x_axis; j++) {
			output[j][image->y_axis - 1 - i] =
				image->red[i + coord.y1][j + coord.x1];
			output_b[j][image->y_axis - 1 - i] =
				image->blue[i + coord.y1][j + coord.x1];
			output_g[j][image->y_axis - 1 - i] =
				image->green[i + coord.y1][j + coord.x1];
		}
	int tmp = image->x_axis;
	image->x_axis = image->y_axis;
	image->y_axis = tmp;

	// necessary when all image is rotated
	int aux = *x1;
	*x1 = *y1;
	*y1 = aux;

	int change = *x2;
	*x2 = *y2;
	*y2 = change;

	int count_image = image->width * image->height;
	int count_select = image->x_axis * image->y_axis;

	// check if all image or selection the count_select and count_image
	if (count_image == count_select) {
		free_matrix(image->height, image->red);
		free_matrix(image->height, image->blue);
		free_matrix(image->height, image->green);

		image->red = alloc_matrix(image->width, image->height);
		image->green = alloc_matrix(image->width, image->height);
		image->blue = alloc_matrix(image->width, image->height);

		int aux = image->width;
		image->width = image->height;
		image->height = aux;

		for (int i = 0; i < image->height; i++)
			for (int j = 0; j < image->width; j++) {
				image->red[i][j] = output[i][j];
				image->blue[i][j] = output_b[i][j];
				image->green[i][j] = output_g[i][j];
			}
	} else {
		for (int i = coord.y1; i < coord.y2; i++) {
			for (int j = coord.x1; j < coord.x2; j++) {
				image->red[i][j] = output[i - coord.y1][j - coord.x1];
				image->blue[i][j] = output_b[i - coord.y1][j - coord.x1];
				image->green[i][j] = output_g[i - coord.y1][j - coord.x1];
			}
		}
	}
	free_matrix(image->y_axis, output);
	free_matrix(image->y_axis, output_b);
	free_matrix(image->y_axis, output_g);
}