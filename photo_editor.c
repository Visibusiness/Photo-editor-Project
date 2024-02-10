// Copyright Vișănescu Bogdan-Emilian 313CAb 2023 - 2024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_LENGH 1000
// the struct for the matrix contains the magic word, number of columns,
// number of rows, the matrix, the selection and an integer called rgb
// that is 1 if the photo is grayscale or 3 if the photo is rgb
typedef struct image {
	char format[3];
	int col;
	int row;
	int maxval;
	int **matrix;
	int x1, x2, y1, y2;
	int rgb;
} image;
void select_part(char words[][1000], image *img);
void select_all(image *img);
void free_img(image *img);
// this function builds the rgb integer for the struct
void check_rgb(image *img)
{
	if (!strcmp(img->format, "P2") || !strcmp(img->format, "P5"))
		img->rgb = 1;
	else
		img->rgb = 3;
}

// this function frees a matrix
void free_matrix(int **matrix, int row)
{
	for (int i = 0; i < row; i++)
		free(matrix[i]);
	free(matrix);
}

// this is a swap function
void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

// this function initializes the image with values of 0 and NULL
void initialize_image(image *image)
{
	image->col = 0;
	image->row = 0;
	image->maxval = 0;
	image->matrix = NULL;
	image->x1 = 0;
	image->y1 = 0;
	image->x2 = 0;
	image->y2 = 0;
}

// this function is the action that select all 2 does
// if the parameters are correct
void select_all_component(image *img)
{
	img->x1 = 0;
	img->y1 = 0;
	img->x2 = img->col;
	img->y2 = img->row;
}

// this function allocates a matrix from the image structure
// the image size is int img->row and img->col variables
void allocation_image(image *img)
{
	img->matrix = (int **)calloc(img->row, sizeof(int *));
	for (int i = 0; i < img->row; i++)
		img->matrix[i] = (int *)calloc(img->col * img->rgb, sizeof(int));
}

// this function allocates a matrix that has row rows and col cols
int **allocation_matrix(int row, int col)
{
	int **matrix;
	matrix = (int **)calloc(row, sizeof(int *));
	for (int i = 0; i < row; i++)
		matrix[i] = (int *)calloc(col, sizeof(int));
	return matrix;
}

// this is a function that is used for loading an image
// the function reads image content in ascii and loads it
// into memory using the image structure
// it also checks if the image is loaded succesfully
void ascii(char *filename, image *image)
{
	FILE *file = fopen(filename, "r");
	int ok = 0;
	fscanf(file, "%s", image->format);
	fscanf(file, "%d", &image->col);
	fscanf(file, "%d", &image->row);
	fscanf(file, "%d", &image->maxval);
	allocation_image(image);
	for (int i = 0; i < image->row; i++)
		for (int j = 0; j < image->col * image->rgb; j++)
			if (fscanf(file, "%d", &image->matrix[i][j]) != 1)
				ok = 1;
	if (ok == 1) {
		printf("Failed to load %s\n", filename);
		initialize_image(image);
	} else {
		select_all_component(image);
		printf("Loaded %s\n", filename);
	}
	fclose(file);
}

// binary version for the upper function
void binary(char *filename, image *image)
{
	int ok = 0;
	FILE *file = fopen(filename, "rb");

	fscanf(file, "%s", image->format);
	fscanf(file, "%d", &image->col);
	fscanf(file, "%d", &image->row);
	fscanf(file, "%d", &image->maxval);
	allocation_image(image);
	fseek(file, 1, SEEK_CUR);
	for (int i = 0; i < image->row; i++)
		for (int j = 0; j < image->col * image->rgb; j++)
			if (fread(&image->matrix[i][j], sizeof(char), 1, file) != 1)
				ok = 1;
	if (ok == 1) {
		printf("Failed to load %s\n", filename);
		initialize_image(image);
	} else {
		select_all_component(image);
		printf("Loaded %s\n", filename);
	}
	fclose(file);
}

// this is the load function that checks if the loading conditions are good and
// put or the components together

void load(char *filename, image *img)
{
	free_img(img);
	initialize_image(img);
	FILE *file = fopen(filename, "r");
	if (!file) {
		printf("Failed to load %s\n", filename);
		return;
	}
	fscanf(file, "%s", img->format);
	check_rgb(img);
	fclose(file);
	if (!strcmp(img->format, "P2") || !strcmp(img->format, "P3"))
		ascii(filename, img);
	else
		binary(filename, img);
}

// this function selects a part from the image
void select_part(char words[][1000], image *img)
{
	if (!img->matrix) {
		printf("No image loaded\n");
		return;
	}
	int coord[5];
	for (int i = 1; i < 5; i++) {
		coord[i] = atoi(words[i]);
		if (coord[i] == 0 && words[i][0] != '0') {
			printf("Invalid command\n");
			return;
		}
	}
	// this puts the selection coords in order
	if (coord[1] > coord[3])
		swap(&coord[1], &coord[3]);
	if (coord[2] > coord[4])
		swap(&coord[2], &coord[4]);
	// this checks if coord are valid
	if (coord[1] < 0 || coord[2] < 0 || coord[1] == coord[3] ||
		coord[2] == coord[4] || coord[3] > img->col || coord[4] > img->row) {
		printf("Invalid set of coordinates\n");
		return;
	}
	// this makes the selection
	img->x1 = coord[1];
	img->x2 = coord[3];
	img->y1 = coord[2];
	img->y2 = coord[4];
	printf("Selected %d %d %d %d\n", img->x1, img->y1, img->x2, img->y2);
}

// this is a function that selects all image
void select_all(image *img)
{
	if (!img->matrix) {
		printf("No image loaded\n");
		return;
	}
	select_all_component(img);
	printf("Selected ALL\n");
}

// this is a function that returns the result of a mathematic ecuation
// that is used for calculating the number of stars in the histogram
int number_stars(int value_freq, int max_freq, int x)
{
	return value_freq * x / max_freq;
}

// this is the function that prints the histogram
void histogram(char words[][1000], image img, int index)
{
	if (!img.matrix) {
		printf("No image loaded\n");
		return;
	}
	if (index != 3) {
		printf("Invalid command\n");
		return;
	}
	if (img.rgb == 3) {
		printf("Black and white image needed\n");
		return;
	}
	int x, y;
	int max = 0;
	// these are the arrays used for the histogram
	//  first will contain the number of stars of each bin
	int *nr_stars = (int *)calloc(256, sizeof(int));
	// second is the sum of the freq in a bin
	int *sum_freq = (int *)calloc(256, sizeof(int));
	// this is the frequance of each value of a pixel
	int *freq = (int *)calloc(256, sizeof(int));
	x = atoi(words[1]);
	y = atoi(words[2]);
	for (int i = 0; i < img.row; i++)
		for (int j = 0; j < img.col; j++)
			freq[img.matrix[i][j]]++;
	for (int i = 0; i < y; i++) {
		for (int j = i * (256 / y); j < (i + 1) * (256 / y); j++)
			sum_freq[i] += freq[j];
		if (sum_freq[i] > max)
			max = sum_freq[i];
	}
	// printing the stars
	for (int i = 0; i < y; i++) {
		nr_stars[i] = number_stars(sum_freq[i], max, x);
		printf("%d  |   ", nr_stars[i]);
		for (int j = 0; j < nr_stars[i]; j++)
			printf("*");
		printf("\n");
	}
	free(nr_stars);
	free(sum_freq);
	free(freq);
}

// this is the ascii format save function
void save_ascii(char words[][1000], image img)
{
	FILE *output_file = fopen(words[1], "wt");
	if (!output_file)
		return;
	if (img.rgb == 1)
		fprintf(output_file, "P2\n");
	else
		fprintf(output_file, "P3\n");
	fprintf(output_file, "%d ", img.col);
	fprintf(output_file, "%d\n", img.row);
	fprintf(output_file, "%d\n", img.maxval);
	for (int i = 0; i < img.row; i++) {
		for (int j = 0; j < img.col * img.rgb; j++)
			fprintf(output_file, "%d ", img.matrix[i][j]);
		fprintf(output_file, "\n");
	}

	fclose(output_file);
}

// this is the binary format save function
void save_binary(char words[][1000], image img)
{
	FILE *output_file = fopen(words[1], "wb");
	if (!output_file)
		return;
	if (img.rgb == 1)
		fprintf(output_file, "P5\n");
	else
		fprintf(output_file, "P6\n");

	fprintf(output_file, "%d ", img.col);
	fprintf(output_file, "%d\n", img.row);
	fprintf(output_file, "%d\n", img.maxval);
	for (int i = 0; i < img.row; i++)
		for (int j = 0; j < img.col * img.rgb; j++)
			fwrite(&img.matrix[i][j], sizeof(char), 1, output_file);

	fclose(output_file);
}

// this puts all save functions together
void save(char words[][1000], image img)
{
	// checks if the save can be done
	if (!img.matrix) {
		printf("No image loaded\n");
		return;
	}
	// saves the result photo eather ascii eather binary
	if (!strcmp(words[2], "ascii"))
		save_ascii(words, img);
	else
		save_binary(words, img);
	printf("Saved %s\n", words[1]);
}

// this function frees a matrix from a image structure
void free_img(image *img)
{
	for (int i = 0; i < img->row; i++)
		free(img->matrix[i]);
	free(img->matrix);
}

// this function ensures that the value of a pixel is mentained inside [0, 255]
int clamp(int value, int min, int max)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	return value;
}

// this function makes the equalize of an image
void equalize(image *img)
{
	// this checks if the equalize can be done
	if (!img->matrix) {
		printf("No image loaded\n");
		return;
	}
	if (img->rgb == 3) {
		printf("Black and white image needed\n");
		return;
	}
	// this is an array that will contain the frequance of a value of a pixel
	int *freq = calloc(256, sizeof(int));
	// this calculates the freq array
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			freq[img->matrix[i][j]]++;

	// this replace the old values with the new ones
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++) {
			int s = 0;
			for (int k = 0; k <= img->matrix[i][j]; k++)
				s += freq[k];
			img->matrix[i][j] =
			(int)round((double)255 * s / (img->row * img->col));
			img->matrix[i][j] = clamp(img->matrix[i][j], 0, 255);
		}
	printf("Equalize done\n");
	free(freq);
}

// this function is exits the program
void exit_program(image *img)
{
	if (!img->matrix) {
		printf("No image loaded\n");
		exit(0);
	}
	free_img(img);
	exit(0);
}

// this function crops an image
void crop(image *img)
{
	if (!img->matrix) {
		printf("No image loaded\n");
		return;
	}
	// allocating an auxiliary matrix
	int **aux =
	allocation_matrix(img->y2 - img->y1, img->rgb * (img->x2 - img->x1));
	// this copies the cropped part into aux
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < img->rgb * (img->x2 - img->x1); j++)
			aux[i][j] = img->matrix[i + img->y1][j + img->x1 * img->rgb];
	// this replaces the image matrix with aux
	free_img(img);
	img->col = img->x2 - img->x1;
	img->row = img->y2 - img->y1;
	allocation_image(img);
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->rgb * img->col; j++)
			img->matrix[i][j] = aux[i][j];
	for (int i = 0; i < img->row; i++)
		free(aux[i]);
	free(aux);
	// this selects all the new croped image
	select_all_component(img);
	printf("Image cropped\n");
}

// this function rotates at 90 degrees a image
void rotate_90(image *image)
{
	int x1 = image->x1;
	int x2 = image->x2;
	int y1 = image->y1;
	int y2 = image->y2;
	int row = y2 - y1;
	int col = x2 - x1;
	int **aux = allocation_matrix(col, row * image->rgb);
	for (int i = 0; i < col; i++)
		for (int j = 0; j < row; j++)
			for (int k = 0; k < image->rgb; k++)
				aux[i][image->rgb * j + k] =
				image->matrix[y2 - 1 - j][image->rgb * (x1 + i) + k];

	if (x1 == 0 && y1 == 0 && x2 == image->col && y2 == image->row) {
		free_img(image);
		image->matrix = aux;
		swap(&image->col, &image->row);
		swap(&image->x1, &image->y1);
		swap(&image->y2, &image->x2);
		return;
	}
	for (int i = 0; i < col; i++)
		for (int j = 0; j < row * image->rgb; j++)
			image->matrix[i + y1][j + image->rgb * x1] = aux[i][j];

	for (int i = 0; i < col; i++)
		free(aux[i]);
	free(aux);
}

// this function makes the rotation by rotating 90 degrees repeatedly
void rotate(image *img, char words[][1000])
{	int y2 = img->y2;
	int y1 = img->y1;
	int x1 = img->x1;
	int x2 = img->x2;
	int nr, angle = atoi(words[1]);
	if (!img->matrix) {
		printf("No image loaded\n");
		return;
	}
	if (img->x2 - img->x1 != img->y2 - img->y1)
		if (x1 != 0 || x2 != img->col || y1 != 0 || y2 != img->row) {
			printf("The selection must be square\n");
			return;
	}
	if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}
	if (!angle % 360) {
		printf("Rotated %d\n", angle);
		return;
	}
	if (angle / 90 < 0)
		nr = 4 + angle / 90;
	else
		nr = angle / 90;
	for (int i = 0; i < nr; i++)
		rotate_90(img);
	printf("Rotated %d\n", angle);
}

// this function makes the apply of different filters
void apply(char words[][1000], image *img, int index)
{
	// checks if the apply can be done
	if (!img->matrix) {
		printf("No image loaded\n");
		return;
	}
	if (index != 2) {
		printf("Invalid command\n");
		return;
	}
	if (strcmp(words[1], "EDGE") && strcmp(words[1], "SHARPEN") &&
		strcmp(words[1], "BLUR") && strcmp(words[1], "GAUSSIAN_BLUR")) {
		printf("APPLY parameter invalid\n");
		return;
	}
	if (img->rgb == 1) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	int **matrix = allocation_matrix(3, 3);
	if (!strcmp(words[1], "EDGE")) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				matrix[i][j] = -1;
		matrix[1][1] = 8;
	} else if (!strcmp(words[1], "SHARPEN")) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if ((i + j) % 2 == 0)
					matrix[i][j] = 0;
				else
					matrix[i][j] = -1;
		matrix[1][1] = 5;
	} else if (!strcmp(words[1], "BLUR")) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				matrix[i][j] = 1;
	} else if (!strcmp(words[1], "GAUSSIAN_BLUR")) {
		matrix[0][0] = 1; matrix[0][1] = 2; matrix[0][2] = 1;
		matrix[1][0] = 2; matrix[1][1] = 4; matrix[1][2] = 2;
		matrix[2][0] = 1; matrix[2][1] = 2; matrix[2][2] = 1;
	}
	int **aux = allocation_matrix(img->row, 3 * img->col);
	int y1 = img->y1, y2 = img->y2;
	int x1 = img->x1, x2 = img->x2;
	if (img->y2 == img->row)
		y2--;
	if (img->x1 < 1)
		x1++;
	if (img->y1 < 1)
		y1++;
	if (img->x2 == img->col)
		x2--;
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < 3 * img->col; j++)
			aux[i][j] = img->matrix[i][j];
	for (int i = y1; i < y2; i++)
		for (int j = 3 * x1; j < 3 * x2; j++) {
			int m = 0, n = 0, s = 0;
			for (int k = i - 1; k < i + 2; k++) {
				n = 0;
				for (int l = j - 3; l < j + 4; l += 3) {
					s += (double)aux[k][l] * matrix[m][n];
					n++;
				}
				m++;
			}
			if (!strcmp(words[1], "BLUR"))
				img->matrix[i][j] = round(clamp(s / 9, 0, 255));
			else if (!strcmp(words[1], "GAUSSIAN_BLUR"))
				img->matrix[i][j] = round(clamp(s / 16, 0, 255));
			else
				img->matrix[i][j] = round(clamp(s, 0, 255));
		}
	for (int i = 0; i < img->row; i++)
		free(aux[i]);
	free(aux);
	free_matrix(matrix, 3);
	printf("APPLY %s done\n", words[1]);
}

// this is the main function
int main(void)
{
	// declaring the useful data
	image img;
	initialize_image(&img);
	// this will be the command that i will read every time
	char command[MAX_LENGH];
	// this is an auxiliary token
	char *token;
	// this is the number of words in a command
	int index;
	// this is an array of char pointers in wich
	char words[MAX_LENGH][MAX_LENGH];
	while (1) {
		// reads the command and breaking it into words
		index = 0;
		fgets(command, 1000, stdin);
		token = strtok(command, " '\n'");
		while (token) {
			// copies the words into words[][]
			strcpy(words[index], token);
			index++;
			token = strtok(NULL, " '\n'");
		}
		// the main command into the program
		if (!strcmp(words[0], "LOAD"))
			load(words[1], &img);
		else if (!strcmp(words[0], "SELECT") && strcmp(words[1], "ALL"))
			select_part(words, &img);
		else if (!strcmp(words[0], "SELECT") && !strcmp(words[1], "ALL"))
			select_all(&img);
		else if (!strcmp(words[0], "HISTOGRAM"))
			histogram(words, img, index);
		else if (!strcmp(words[0], "SAVE"))
			save(words, img);
		else if (!strcmp(words[0], "EXIT"))
			exit_program(&img);
		else if (!strcmp(words[0], "EQUALIZE"))
			equalize(&img);
		else if (!strcmp(words[0], "CROP"))
			crop(&img);
		else if (!strcmp(words[0], "ROTATE"))
			rotate(&img, words);
		else if (!strcmp(words[0], "APPLY"))
			apply(words, &img, index);
		else
			printf("Invalid command\n");
	}
	return 0;
}
