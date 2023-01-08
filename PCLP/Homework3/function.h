#define NMAX 50
struct global_image;


int max_of_numbers(int a,int b);
void swap_function(int* a,int* b);

//void ignore_comments(FILE* fp);
void free_matrix(int line,int **matrix);
int **alloc_matrix(int line, int coloumn);
void free_global_matrix(struct global_image* image);

void select_function_integers(struct global_image *image,int x1,int x2,int y1,int y2);
void select_function_all(struct global_image *image,int *x1,int *y1,int *x2,int *y2);

void crop_function(struct global_image *image);

void equalize_function(struct global_image *image);

void histogram_function(struct global_image image,int h1_star,int h2_bins);

