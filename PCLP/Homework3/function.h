#define NMAX 30
struct global_image;
int file_type(FILE *fptr,char s[NMAX]);
void free_matrix(int line,int **matrix);
int **alloc_matrix(int line, int coloumn);
void ignore_comments(FILE* fp);
void text_file_reader_pgm_edition(char s[NMAX],FILE *fptr,struct global_image* image);
void file_reader_first_version(char s[NMAX],int type,FILE *fptr,struct global_image* image);
void print_matrix(struct global_image element);
void text_file_reader_ppm_edition(char s[NMAX],FILE *fptr,struct global_image* image);
void free_global_matrix(int type,struct global_image* image);
void file_printer_for_tests(int *type,struct global_image image);
void binary_file_reader_pgm_edition(char s[NMAX],FILE *fptr, struct global_image* image);
void binary_file_reader_ppm_edition(char s[NMAX],FILE *fptr,struct global_image* image);
int operation_identifier(char s[NMAX],int *x1,int *y1, int *x2,int *y2,int *h1,int *h2,int *angle,char parametre[NMAX],struct global_image);