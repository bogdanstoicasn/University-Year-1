
struct global_image;

struct RGB **alloc_struct(int line,int coloumn);
void free_matrix(int line,int **matrix);
void ignore_comments(FILE* fp);
void open_text_file(struct global_image *image,FILE *fptr);
void print_text_file(struct global_image image,FILE *output);
int **alloc_matrix(int line, int coloumn);