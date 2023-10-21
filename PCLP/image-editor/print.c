/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include "print.h"

#define NMAX 50

struct global_image {
	char type[3];
	int width, height;
	int max_value, x_axis, y_axis;
	int **red;
	int **green;
	int **blue;
};

// save function
// it gets the name of file in "file" and the type in "type"
// save accordingly
void file_printer_for_tests(int *type, char file[NMAX],
							struct global_image image)
{
	FILE *output;
	if (*type == 2 || *type == 5)
		output = fopen(file, "wt");
	else
		output = fopen(file, "wb");

	switch (*type) {
	case 2:
		fprintf(output, "P2\n%d %d\n%d\n", image.width,
				image.height, image.max_value);

		for (int i = 0; i < image.height; i++) {
			for (int j = 0; j < image.width; j++) {
				fprintf(output, "%d", image.red[i][j]);

				if (j < image.width - 1)
					fprintf(output, " ");
			}
			fprintf(output, "\n");
		}
		printf("Saved %s\n", file);
		break;
	case 3:
		fprintf(output, "P3\n%d %d\n%d\n", image.width,
				image.height, image.max_value);

		for (int i = 0; i < image.height; i++) {
			for (int j = 0; j < image.width; j++) {
				fprintf(output, "%d %d %d ", image.red[i][j],
						image.green[i][j], image.blue[i][j]);
			}
			fprintf(output, "\n");
		}
		printf("Saved %s\n", file);
		break;
	case 5:
		fprintf(output, "P5\n%d %d\n%d\n", image.width,
				image.height, image.max_value);

		for (int i = 0; i < image.height; i++) {
			for (int j = 0; j < image.width; j++) {
				unsigned char c = (unsigned char)image.red[i][j];
				fwrite(&c, sizeof(unsigned char), 1, output);
			}
		}
		printf("Saved %s\n", file);
		break;
	case 6:
		fprintf(output, "P6\n%d %d\n%d\n", image.width,
				image.height, image.max_value);

		for (int i = 0; i < image.height; i++) {
			for (int j = 0; j < image.width; j++) {
				unsigned char c = (unsigned char)image.red[i][j];
				fwrite(&c, sizeof(unsigned char), 1, output);

				unsigned char d = (unsigned char)image.green[i][j];
				fwrite(&d, sizeof(unsigned char), 1, output);

				unsigned char e = (char)image.blue[i][j];
				fwrite(&e, sizeof(unsigned char), 1, output);
			}
		}
		printf("Saved %s\n", file);
		break;
	default:
		printf("Failed to load %s\n", file);
		break;
	}
	fclose(output);
}
