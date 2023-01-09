/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "function.h"
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

int **kernel_matrixes_creator(char parameter[NMAX])
{
	int i, j;
	int **kernel = alloc_matrix(3, 3);

	if (!kernel) {
		printf("Alloc failed");
		return NULL;
	}

	if (strcmp(parameter, "EDGE") == 0) {
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				kernel[i][j] = -1;
		kernel[1][1] = 8;
	}

	if (strcmp(parameter, "SHARPEN") == 0) {
		for (i = 0; i < 3; i++)
			for (j = 0; j <= i; j++)
				if ((i + j) % 2 == 0) {
					kernel[i][j] = 0;
					kernel[j][i] = 0;
				} else {
					kernel[i][j] = -1;
					kernel[j][i] = -1;
				}
		kernel[1][1] = 5;
	}

	if (strcmp(parameter, "BLUR") == 0) {
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				kernel[i][j] = 1;
	}

	if (strcmp(parameter, "GAUSSIAN_BLUR") == 0) {
		for (i = 0; i < 3; i++)
			for (j = 0; j <= i; j++)
				if ((i + j) % 2 == 0) {
					kernel[i][j] = 1;
					kernel[j][i] = 1;
				} else {
					kernel[i][j] = 2;
					kernel[j][i] = 2;
				}
		kernel[1][1] = 4;
	}

	return kernel;
}

void checker_0_and_255_case(double *a)
{
	if (*a < 0)
		*a = 0;
	if (*a > 255)
		*a = 255;
}

void apply_kernel(struct global_image *image, int x1, int y1, int x2, int y2,
				  int **kernel, int div)
{
	int **output_r = alloc_matrix(y2 - y1, x2 - x1);
	int **output_g = alloc_matrix(y2 - y1, x2 - x1);
	int **output_b = alloc_matrix(y2 - y1, x2 - x1);
	if (!output_r) {
		printf("Failed to APPLY\n");
		return;
	}

	if (!output_g) {
		free_matrix(y2 - y1, output_r);
		printf("Failed to APPLY\n");
		return;
	}

	if (!output_r) {
		free_matrix(y2 - y1, output_r);
		free_matrix(y2 - y1, output_g);
		printf("Failed to APPLY\n");
		return;
	}

	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			double sum_r = 0, sum_g = 0, sum_b = 0;
			int ok = 0, boundary = 0;
			output_r[i - y1][j - x1] = image->red[i][j];
			output_g[i - y1][j - x1] = image->green[i][j];
			output_b[i - y1][j - x1] = image->blue[i][j];
			for (int m = -1; m <= 1; m++) {
				for (int n = -1; n <= 1; n++) {
					if (i < 1 || j < 1 ||
						i >= image->height - 1 || j >= image->width - 1) {
						boundary++;
					} else {
						sum_r += image->red[i + m][j + n] *
								 kernel[m + 1][n + 1];
						sum_g += image->green[i + m][j + n] *
								 kernel[m + 1][n + 1];
						sum_b += image->blue[i + m][j + n] *
								 kernel[m + 1][n + 1];
						ok++;
					}
				}
			}
			sum_r /= div;
			sum_g /= div;
			sum_b /= div;

			checker_0_and_255_case(&sum_b);
			checker_0_and_255_case(&sum_g);
			checker_0_and_255_case(&sum_r);

			if (ok != 0) {
				output_r[i - y1][j - x1] = round(sum_r);
				output_b[i - y1][j - x1] = round(sum_b);
				output_g[i - y1][j - x1] = round(sum_g);
			}
		}
	}
	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			image->red[i][j] = output_r[i - y1][j - x1];
			image->green[i][j] = output_g[i - y1][j - x1];
			image->blue[i][j] = output_b[i - y1][j - x1];
		}
	}
	// free the resources my boy
	free_matrix(3, kernel);
	free_matrix(y2 - y1, output_b);
	free_matrix(y2 - y1, output_r);
	free_matrix(y2 - y1, output_g);
}

void kernel_interface_helper(struct global_image *image, char parameter[NMAX],
							 int x1, int y1, int x2, int y2)
{
	int **kernel = kernel_matrixes_creator(parameter);
	if (!kernel) {
		printf("Alloc failed");
		return;
	}

	if (strcmp(parameter, "EDGE") == 0)
		apply_kernel(image, x1, y1, x2, y2, kernel, 1);

	if (strcmp(parameter, "SHARPEN") == 0)
		apply_kernel(image, x1, y1, x2, y2, kernel, 1);

	if (strcmp(parameter, "BLUR") == 0)
		apply_kernel(image, x1, y1, x2, y2, kernel, 9);

	if (strcmp(parameter, "GAUSSIAN_BLUR") == 0)
		apply_kernel(image, x1, y1, x2, y2, kernel, 16);

	printf("APPLY %s done\n", parameter);
}
