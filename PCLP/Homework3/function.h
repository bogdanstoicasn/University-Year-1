#define NMAX 50

struct global_image;


int max_of_numbers(int a,int b);
void swap_function(int* a,int* b);


void free_matrix(int line,int **matrix);
int **alloc_matrix(int line, int coloumn);
void print_matrix(struct global_image element);
void free_global_matrix(int type,struct global_image* image);


void ignore_comments(FILE* fp);
int file_type(FILE *fptr,char s[NMAX]);


void file_reader_first_version(int *count,char s[NMAX],int type,FILE *fptr,struct global_image* image);
void text_file_reader_pgm_edition(char s[NMAX],FILE *fptr,struct global_image* image);
void text_file_reader_ppm_edition(char s[NMAX],FILE *fptr,struct global_image* image);
void binary_file_reader_pgm_edition(char s[NMAX],FILE *fptr, struct global_image* image);
void binary_file_reader_ppm_edition(char s[NMAX],FILE *fptr,struct global_image* image);
void file_printer_for_tests(int *type,char file[NMAX],struct global_image image);


int operation_identifier(int count,char s[NMAX],int *x1,int *y1, int *x2,int *y2,int *h1,int *h2,int *angle,char parametre[NMAX],char file[NMAX],int *type,struct global_image);


int select_function_identifier(int count,char string[NMAX],struct global_image image,int *x1,int *y1,int *x2,int *y2);
void select_function_integers(struct global_image *image,int x1,int x2,int y1,int y2);
void select_function_all(struct global_image *image,int *x1,int *y1,int *x2,int *y2);


int histogram_function_identifier(int count,char string[NMAX],struct global_image image,int *h1,int *h2);


int save_function_identifier(char string[NMAX],char file[NMAX],int *type,struct global_image image);


void crop_function(struct global_image *image);


void equalize_function(struct global_image *image);
int equalize_function_identifier(int count,struct global_image image);


int load_function_identifier(char string[NMAX],char s[NMAX]);


int rotate_function_identifier(int count,char string[NMAX],struct global_image image,int *angle);
void counter_clockwise_rotation_90(struct global_image *image);
void clockwise_rotation_90(struct global_image *image);
void rotate_function_helper(struct global_image *image,int angle);


void histogram_function(struct global_image image,int h1_star,int h2_bins);


int apply_function_identifier(int count,char string[NMAX],char parameter[NMAX],struct global_image image);


int **kernel_matrixes_creator(char parameter[NMAX]);
void checker_0_and_255_case(double *a);
void apply_kernel(struct global_image *image,int x1,int y1,int x2,int y2,int **kernel,int div);
void kernel_interface_helper(struct global_image *image,char parameter[NMAX],int x1,int y1,int x2,int y2);


void refurbished_case1_loading(FILE *fptr,char s[NMAX],int *count,struct global_image *image);