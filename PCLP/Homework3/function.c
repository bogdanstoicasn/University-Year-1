/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "function.h"
#include "interface.h"

#define NMAX 50
#define MAX_UP 255
#define MIN_DOWN 0

struct global_image {
	char type[3];
	int width, height;
	int max_value, x_axis, y_axis;
	int **red;
	int **green;
	int **blue;
};

// max of 2 nuumbers
int max_of_numbers(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

// swap 2 numbers if first is bigger
void swap_function(int *a, int *b)
{
	if (*a > *b) {
		int aux = *a;
		*a = *b;
		*b = aux;
	}
}

// alloc matrix with defensive programming
int **alloc_matrix(int line, int coloumn)
{
	int **a = (int **)malloc(line * sizeof(int *));
	if (!a) {
		fprintf(stderr, "malloc() failed\n");
		return NULL;
	}
	for (int i = 0; i < line; i++) {
		a[i] = (int *)malloc(coloumn * sizeof(int));
		if (!a[i]) {
			fprintf(stderr, "malloc() failed\n");
			while (--i >= 0)
				free(a[i]);
			free(a);
			return NULL;
		}
	}
	return a;
}

// freeing the allocated matrix
void free_matrix(int line, int **matrix)
{
	for (int i = 0; i < line; i++)
		free(matrix[i]);
	free(matrix);
}

/// function which frees memory by type
void free_global_matrix(struct global_image *image)
{
	int type = image->type[1] - '0';
	switch (type) {
	case 2:
		free_matrix(image->height, image->red);
		break;
	case 3: {
		free_matrix(image->height, image->red);
		free_matrix(image->height, image->green);
		free_matrix(image->height, image->blue);
		break;
		}
	case 5:
		free_matrix(image->height, image->red);
		break;
	case 6: {
		free_matrix(image->height, image->red);
		free_matrix(image->height, image->green);
		free_matrix(image->height, image->blue);
		break;
		}
	default:
		fprintf(stderr, "Failed to free\n");
		break;
	}
}

// crop function with coordinates: x1->x2, y1->y2
void crop_function(struct global_image *image,
				   int *x1, int *y1, int *x2, int *y2)
{
	if (image->type[1] == '2' || image->type[1] == '5') {
		int **output = alloc_matrix(*y2 - *y1, *x2 - *x1);
		if (!output) {
			fprintf(stderr, "Failed to crop\n");
			return;
		}
		for (int i = *y1; i < *y2; i++)
			for (int j = *x1; j < *x2; j++)
				output[i - *y1][j - *x1] = image->red[i][j];

		free_global_matrix(image);
		image->height = *y2 - *y1;
		image->y_axis = *y2 - *y1;
		image->width = *x2 - *x1;
		image->x_axis = *x2 - *x1;
		*x1 = 0;
		*x2 = image->width;
		*y1 = 0;
		*y2 = image->height;
		image->red = output;
	}
	if (image->type[1] == '3' || image->type[1] == '6') {
		int **output_r = alloc_matrix(*y2 - *y1, *x2 - *x1);
		int **output_g = alloc_matrix(*y2 - *y1, *x2 - *x1);
		int **output_b = alloc_matrix(*y2 - *y1, *x2 - *x1);
		if (!output_r) {
			fprintf(stderr, "Failed to crop\n");
			return;
		}
		if (!output_g) {
			free_matrix(*y2 - *y1, output_r);
			fprintf(stderr, "Failed to crop\n");
			return;
		}
		if (!output_g) {
			free_matrix(*y2 - *y1, output_r);
			free_matrix(*y2 - *y1, output_g);
			fprintf(stderr, "Failed to crop\n");
			return;
		}
		for (int i = *y1; i < *y2; i++)
			for (int j = *x1; j < *x2; j++) {
				output_r[i - *y1][j - *x1] = image->red[i][j];
				output_g[i - *y1][j - *x1] = image->green[i][j];
				output_b[i - *y1][j - *x1] = image->blue[i][j];
			}

		free_global_matrix(image);
		image->red = output_r;
		image->blue = output_b;
		image->green = output_g;
		image->height = *y2 - *y1;
		image->y_axis = *y2 - *y1;
		image->width = *x2 - *x1;
		image->x_axis = *x2 - *x1;
		*x1 = 0;
		*x2 = image->width;
		*y1 = 0;
		*y2 = image->height;
	}

	printf("Image cropped\n");
}

// histogram calculated with bins and stars
void histogram_function(struct global_image image, int h1_star, int h2_bins)
{
	int i, j;
	int *histogram_map = NULL, max_frequency = 0;
	histogram_map = calloc(h2_bins, sizeof(int));
	if (!histogram_map) {
		printf("Alloc failed\n");
		return;
	}

	int interval = (image.max_value + 1) / h2_bins;
	for (i = 0; i < image.height; i++)
		for (j = 0; j < image.width; j++)
			histogram_map[image.red[i][j] / interval]++;

	for (i = 0; i < h2_bins; i++)
		if (histogram_map[i] > max_frequency)
			max_frequency = histogram_map[i];

	for (i = 0; i < h2_bins; i++) {
		double length_of_stars = (histogram_map[i] * h1_star / max_frequency);

		printf("%d\t|\t", (int)length_of_stars);

		for (j = 0; j < ceil(length_of_stars); j++)
			printf("*");
		printf("\n");
	}
	free(histogram_map);
}

// equalize function
void equalize_function(struct global_image *image)
{
	int *frequency = calloc(256, sizeof(int));
	double area = image->width * image->height;

	if (!frequency) {
		printf("Alloc failed\n");
		return;
	}

	// frequency
	for (int i = 0; i < image->height; i++)
		for (int j = 0; j < image->width; j++)
			frequency[(image->red[i][j])]++;

	for (int i = 0; i < image->height; i++) {
		for (int j = 0; j < image->width; j++) {
			int sum = 0;
			for (int k = 0; k <= image->red[i][j]; k++)
				sum += frequency[k];

			double final_result;
			final_result = 255 * 1.00000 *  (sum / area);

			image->red[i][j] = round(final_result);
			if (image->red[i][j] > MAX_UP)
				image->red[i][j] = MAX_UP;
			if (image->red[i][j] < MIN_DOWN)
				image->red[i][j] = MIN_DOWN;
		}
	}
	free(frequency);
	printf("Equalize done\n");
}
