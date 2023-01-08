/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include "function.h"
#include "rotate.h"
#define NMAX 50
struct global_image {
	char type[3];
	int width, height, max_value, x_axis, y_axis;
	int **red, **red_crop;
	int **green, **green_crop;
	int **blue, **blue_crop;
};

// we rotate by type and angle with switch
void rotate_function_helper(struct global_image *image, int angle,
							int x1, int y1, int x2, int y2)
{
	if (image->type[1] == '2' || image->type[1] == '5') {
		switch (angle) {
		case 90:
			clockwise_rotation_90(image, x1, y1, x2, y2);
			printf("Rotated 90\n"); break;
		case -90:
			counter_clockwise_rotation_90(image, x1, y1, x2, y2);
			printf("Rotated -90\n"); break;
		case 180:
			clockwise_rotation_90(image, x1, y1, x2, y2);
			clockwise_rotation_90(image, x1, y1, x2, y2);
			printf("Rotated 180\n"); break;
		case -180:
			counter_clockwise_rotation_90(image, x1, y1, x2, y2);
			counter_clockwise_rotation_90(image, x1, y1, x2, y2);
			printf("Rotated -180\n"); break;
		case 270:
			counter_clockwise_rotation_90(image, x1, y1, x2, y2);
			printf("Rotated 270\n"); break;
		case -270:
			clockwise_rotation_90(image, x1, y1, x2, y2);
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
			clockwise_rotation_90_color(image, x1, y1, x2, y2);
			printf("Rotated 90\n"); break;
		case -90:
			counter_clockwise_rotation_90_color(image, x1, y1, x2, y2);
			printf("Rotated -90\n"); break;
		case 180:
			clockwise_rotation_90_color(image, x1, y1, x2, y2);
			clockwise_rotation_90_color(image, x1, y1, x2, y2);
			printf("Rotated 180\n"); break;
		case -180:
			counter_clockwise_rotation_90_color(image, x1, y1, x2, y2);
			counter_clockwise_rotation_90_color(image, x1, y1, x2, y2);
			printf("Rotated -180\n"); break;
		case 270:
			counter_clockwise_rotation_90_color(image, x1, y1, x2, y2);
			printf("Rotated 270\n"); break;
		case -270:
			clockwise_rotation_90_color(image, x1, y1, x2, y2);
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

// both rotations are done and they work as intended
void counter_clockwise_rotation_90(struct global_image *image,
								   int x1, int y1, int x2, int y2)
{
	//we do the rotation in the alloc bellow then
	// we link with the image matrix
	int **output = alloc_matrix(image->x_axis, image->y_axis);
	if (!output) {
		printf("Failed to rotate\n");
		return;
	}
	for (int i = 0; i < image->y_axis; i++)
		for (int j = 0; j < image->x_axis; j++)
			output[image->x_axis - j - 1][i] = image->red_crop[i][j];

	free_matrix(image->y_axis, image->red_crop);

	int tmp = image->x_axis;
	image->x_axis = image->y_axis;
	image->y_axis = tmp;

	image->red_crop = output;

	int count_image = 0, count_select = 0;
	for (int i = 0; i < image->height; i++)
		for (int j = 0; j < image->width; j++)
			count_image++;
	for (int i = 0; i < image->x_axis; i++)
		for (int j = 0; j < image->y_axis; j++)
			count_select++;

	// check if all image or selection the count_select and count_image
	if (count_image == count_select) {
		free_matrix(image->height, image->red);

		image->red = alloc_matrix(image->width, image->height);

		int aux = image->width;
		image->width = image->height;
		image->height = aux;

		for (int i = 0; i < image->height; i++)
			for (int j = 0; j < image->width; j++)
				image->red[i][j] = image->red_crop[i][j];

	} else {
		for (int i = y1; i < y2; i++)
			for (int j = x1; j < x2; j++)
				image->red[i][j] = image->red_crop[i - y1][j - x1];
	}
}

void clockwise_rotation_90(struct global_image *image,
						   int x1, int y1, int x2, int y2)
{
	//we do the rotation in the alloc bellow then
	// we link with the image matrix
	int **output = alloc_matrix(image->x_axis, image->y_axis);
	if (!output) {
		printf("Failed to rotate\n");
		return;
	}
	for (int i = 0; i < image->y_axis; i++)
		for (int j = 0; j < image->x_axis; j++)
			output[j][image->y_axis - 1 - i] = image->red_crop[i][j];

	free_matrix(image->y_axis, image->red_crop);

	int tmp = image->x_axis;
	image->x_axis = image->y_axis;
	image->y_axis = tmp;

	image->red_crop = output;

	int count_image = 0, count_select = 0;
	for (int i = 0; i < image->height; i++)
		for (int j = 0; j < image->width; j++)
			count_image++;
	for (int i = 0; i < image->x_axis; i++)
		for (int j = 0; j < image->y_axis; j++)
			count_select++;

	// check if all image or selection the count_select and count_image
	if (count_image == count_select) {
		free_matrix(image->height, image->red);

		image->red = alloc_matrix(image->width, image->height);

		int aux = image->width;
		image->width = image->height;
		image->height = aux;

		for (int i = 0; i < image->height; i++)
			for (int j = 0; j < image->width; j++)
				image->red[i][j] = image->red_crop[i][j];
	} else {
		for (int i = y1; i < y2; i++)
			for (int j = x1; j < x2; j++)
				image->red[i][j] = image->red_crop[i - y1][j - x1];
	}
}

void counter_clockwise_rotation_90_color(struct global_image *image,
										 int x1, int y1, int x2, int y2)
{
	//we do the rotation in the alloc bellow then
	// we link with the image matrix
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
			output[image->x_axis - j - 1][i] = image->red_crop[i][j];
			output_b[image->x_axis - j - 1][i] = image->blue_crop[i][j];
			output_g[image->x_axis - j - 1][i] = image->green_crop[i][j];
		}

	free_matrix(image->y_axis, image->red_crop);
	free_matrix(image->y_axis, image->blue_crop);
	free_matrix(image->y_axis, image->green_crop);

	int tmp = image->x_axis;
	image->x_axis = image->y_axis;
	image->y_axis = tmp;

	image->red_crop = output;
	image->blue_crop = output_b;
	image->green_crop = output_g;

	int count_image = 0, count_select = 0;
	for (int i = 0; i < image->height; i++)
		for (int j = 0; j < image->width; j++)
			count_image++;
	for (int i = 0; i < image->x_axis; i++)
		for (int j = 0; j < image->y_axis; j++)
			count_select++;

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
				image->red[i][j] = image->red_crop[i][j];
				image->blue[i][j] = image->blue_crop[i][j];
				image->green[i][j] = image->green_crop[i][j];
			}
	} else {
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				image->red[i][j] = image->red_crop[i - y1][j - x1];
				image->blue[i][j] = image->blue_crop[i - y1][j - x1];
				image->green[i][j] = image->green_crop[i - y1][j - x1];
			}
		}
	}
}

void clockwise_rotation_90_color(struct global_image *image,
								 int x1, int y1, int x2, int y2)
{
	//we do the rotation in the alloc bellow then
	// we link with the image matrix
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
			output[j][image->y_axis - 1 - i] = image->red_crop[i][j];
			output_b[j][image->y_axis - 1 - i] = image->blue_crop[i][j];
			output_g[j][image->y_axis - 1 - i] = image->green_crop[i][j];
		}

	free_matrix(image->y_axis, image->red_crop);
	free_matrix(image->y_axis, image->blue_crop);
	free_matrix(image->y_axis, image->green_crop);

	int tmp = image->x_axis;
	image->x_axis = image->y_axis;
	image->y_axis = tmp;

	image->red_crop = output;
	image->blue_crop = output_b;
	image->green_crop = output_g;
	int count_image = 0, count_select = 0;

	for (int i = 0; i < image->height; i++)
		for (int j = 0; j < image->width; j++)
			count_image++;
	for (int i = 0; i < image->x_axis; i++)
		for (int j = 0; j < image->y_axis; j++)
			count_select++;
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
				image->red[i][j] = image->red_crop[i][j];
				image->blue[i][j] = image->blue_crop[i][j];
				image->green[i][j] = image->green_crop[i][j];
			}
	} else {
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				image->red[i][j] = image->red_crop[i - y1][j - x1];
				image->blue[i][j] = image->blue_crop[i - y1][j - x1];
				image->green[i][j] = image->green_crop[i - y1][j - x1];
			}
		}
	}
}
