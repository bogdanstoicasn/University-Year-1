/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)

#define NMAX 50
struct global_image;

int **kernel_matrixes_creator(char parameter[NMAX]);

void checker_0_and_255_case(double *a);

void apply_kernel(struct global_image *image, int x1, int y1, int x2, int y2,
				  int **kernel, int div);

void kernel_interface_helper(struct global_image *image,
							 char parameter[NMAX],
							 int x1, int y1, int x2, int y2);
