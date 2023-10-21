/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)

#define NMAX 50
struct global_image;

void file_reader_first_version(int *count, char s[NMAX],
							   int type, FILE *fptr,
							   struct global_image *image,
							   int *x1, int *y1, int *x2, int *y2);

void text_file_reader_pgm_edition(char s[NMAX], FILE *fptr,
								  struct global_image *image);

void text_file_reader_ppm_edition(char s[NMAX], FILE *fptr,
								  struct global_image *image);

void binary_file_reader_pgm_edition(char s[NMAX], FILE *fptr,
									struct global_image *image);

void binary_file_reader_ppm_edition(char s[NMAX], FILE *fptr,
									struct global_image *image);
