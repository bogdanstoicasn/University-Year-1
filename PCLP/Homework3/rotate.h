/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)

#define NMAX 50
struct global_image;

void counter_clockwise_rotation_90(struct global_image *image,
								   int x1, int y1, int x2, int y2);

void clockwise_rotation_90(struct global_image *image,
						   int x1, int y1, int x2, int y2);

void rotate_function_helper(struct global_image *image,
							int angle, int x1, int y1, int x2, int y2);

void counter_clockwise_rotation_90_color(struct global_image *image,
										 int x1, int y1, int x2, int y2);

void clockwise_rotation_90_color(struct global_image *image,
								 int x1, int y1, int x2, int y2);
