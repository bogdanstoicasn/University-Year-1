
void file_reader_first_version(int *count,char s[NMAX],int type,FILE *fptr,struct global_image* image);

void text_file_reader_pgm_edition(char s[NMAX],FILE *fptr,struct global_image* image);
void text_file_reader_ppm_edition(char s[NMAX],FILE *fptr,struct global_image* image);
void binary_file_reader_pgm_edition(char s[NMAX],FILE *fptr, struct global_image* image);
void binary_file_reader_ppm_edition(char s[NMAX],FILE *fptr,struct global_image* image);
void skip_to_eol(FILE *input);
