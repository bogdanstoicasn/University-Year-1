#define NMAX 50
struct global_image;

int max_of_numbers(int a, int b);

void swap_function(int *a, int *b);

void free_matrix(int line, int **matrix);

int **alloc_matrix(int line, int coloumn);

void free_global_matrix(struct global_image *image);

void crop_function(struct global_image *image, int *x1, int *y1, int *x2, int *y2);

void equalize_function(struct global_image *image);

void histogram_function(struct global_image image, int h1_star, int h2_bins);
