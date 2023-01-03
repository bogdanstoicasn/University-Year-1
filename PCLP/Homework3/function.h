#define NMAX 30
struct global_image;
int file_type(FILE *fptr,char s[NMAX]);
void free_matrix(int line,int **matrix);
int **alloc_matrix(int line, int coloumn);
void ignore_comments(FILE* fp);
void text_file_reader_pgm_edition(char s[NMAX],FILE *fptr,struct global_image* image);
void file_reader_first_version(int *count,char s[NMAX],int type,FILE *fptr,struct global_image* image);
void print_matrix(struct global_image element);
void text_file_reader_ppm_edition(char s[NMAX],FILE *fptr,struct global_image* image);
void free_global_matrix(int type,struct global_image* image);
void file_printer_for_tests(int *type,char file[NMAX],struct global_image image);
void binary_file_reader_pgm_edition(char s[NMAX],FILE *fptr, struct global_image* image);
void binary_file_reader_ppm_edition(char s[NMAX],FILE *fptr,struct global_image* image);
int operation_identifier(int count,char s[NMAX],int *x1,int *y1, int *x2,int *y2,int *h1,int *h2,int *angle,char parametre[NMAX],char file[NMAX],int *type,struct global_image);
int max_of_numbers(int a,int b);
void swap_function(int* a,int* b);
int select_function_identifier(char string[NMAX],struct global_image image,int *x1,int *y1,int *x2,int *y2);
int histogram_function_identifier(char string[NMAX],struct global_image image,int *h1,int *h2);
int save_function_identifier(char string[NMAX],char file[NMAX],int *type,struct global_image image);
void select_function_integers(struct global_image *image,int x1,int x2,int y1,int y2);
void select_function_all(struct global_image *image);
void crop_function(struct global_image *image);


void counter_clockwise_rotation_90(struct global_image *image);
void clockwise_rotation_90(struct global_image *image);
void rotate_function_helper(struct global_image *image,int angle);

void histogram_function(struct global_image image,int h1_star,int h2_bins);