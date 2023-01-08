/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)

#define NMAX 50
struct global_image;

void ignore_comments(FILE *fp);

int file_type(FILE *fptr, char s[NMAX]);

int operation_identifier(int count, char s[NMAX],
						 int *x1, int *y1, int *x2, int *y2, int *h1, int *h2,
						 int *angle, char parametre[NMAX], char file[NMAX],
						 int *type, struct global_image image
						);

int load_function_identifier(char string[NMAX], char s[NMAX]);

int histogram_function_identifier(int count, char string[NMAX],
								  struct global_image image, int *h1, int *h2
								);

int save_function_identifier(char string[NMAX], char file[NMAX],
							 int *type, struct global_image image
							);

int rotate_function_identifier(int count, char string[NMAX],
							   struct global_image image, int *angle
								);

int equalize_function_identifier(int count, struct global_image image);

int apply_function_identifier(int count, char string[NMAX],
							  char parameter[NMAX],
							  struct global_image image
							);

int refurbished_select_identifier(int count, char string[NMAX],
								  struct global_image image,
								  int *x1, int *y1, int *x2, int *y2
								  );
