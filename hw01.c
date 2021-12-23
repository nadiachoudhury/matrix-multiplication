/* 
Assignment 1: NxM Matrix Multiplication 
CSCI-2500 Fall 2021
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

/******************************************************************************/
/* Function Calls */
/******************************************************************************/

typedef struct{
  double** data;
  int rows;
  int cols;
} matrix; 

int mm_alloc(matrix* mat);
int mm_free (matrix* mat);
int mm_print(matrix* mat);
int mm_read (char* filename, matrix* mat);
matrix* mm_matrix_mult(matrix* mat1, matrix* mat2);


int mm_alloc(matrix* mat)
{
/*
Notes:
  Allocate an array of pointers to a double
  Then, allocate an row/array of doubles and assign each double pointer that row
  Make sure you're allocating using mat->data in the matrix struct
  You can use calloc for allocation and zeroing out all the bytes
  - otherwise, you'd want to manually set all arrays to 0 (good practice)
  If your malloc/calloc returns null, the allocation failed
*/
  
  mat->data = (double**)calloc(mat->rows, sizeof(double)); //allocates an array
  if (mat->data == NULL){
    printf("mm_alloc: allocation failed.\n");
    exit(-1);
  }

  for (int i=0; i<mat->rows; ++i){
    mat->data[i] = (double*)calloc(mat->cols, sizeof(double));
    if (mat->data[i] == NULL){
      printf("mm_alloc: allocation failed.\n");
      exit(-1);
    }
  }

  for (int j=0; j<mat->rows; ++j){
    for (int k=0; k<mat->cols; ++k){
      mat->data[j][k] = 0;
    }
  }
  return 0;
}

int mm_free(matrix* mat)
{
/*
Notes:
  Reverse the allocation process, but you'll be calling free() instead
*/
  
  for (int i=0; i<mat->rows; ++i){
    free(mat->data[i]);
  }

  free(mat->data);
  return 0;
}

int mm_print(matrix* mat)
{
/*
Notes:
  Print all the data values in your matrix
  Go row by row
  At the end of each row, print a newline
  Between each data value, insert a tab
  Use 2 digits of precision after the decimal (use "%10.2lf\t" as format string)
  See output.txt for expected output formatting
*/

  printf("\n/******** START of 2-D Matrix **********************************/\n");
  for (int j=0; j<mat->rows; ++j){
    for (int k=0; k<mat->cols; ++k){
      printf("%10.2lf\t", mat->data[j][k]); //if k=cols, print new line
    }
    printf("\n");
  }
  printf("/******** END of 2-D Matrix ************************************/\n");

  return 0;
}

int mm_read(char* filename, matrix* mat) 
{
/*
Notes:
  Use fopen to open the matrix file, then use fgets or getline to read lines
  The first line of the matrix contains integers for "#rows #columns"
  - Use these to allocate your arrays for matrix data
  Each subsequent line contains a single data value
  - You can use sscanf to parse each matrix entry
  - First value is data[0][0], next is data[0][1] ... data[#rows-1][#columns-1]
*/
  FILE *fp;
  fp = fopen(filename, "r"); //opens file to read
  char line[100];
  int row, column;

  if (fp == NULL){ //file doesn't exist
    printf("mm_read: failed to open file.\n");
    exit(-1);
  }

  if (fgets(line, 100, fp) != NULL){
    if (2 != sscanf(line, "%d %d", &row, &column)){
      printf("mm_read: failed to read matrix dimensions.\n");
      exit(-1);
    }
  
  } else { //file empty
    printf("mm_read: failed to read from file.");
    exit(-1);
  }

  mat->rows = row;
  mat->cols = column;

  int i=0;
  int j=0;
  int count=0;
  mm_alloc(mat);

  while (fgets(line, 100, fp) != NULL){

    sscanf(line, "%lf", &(mat->data[i][j]));
    count++;
    j++;
    if (j == column){ //if last column in row, reset columns and move on to next row
      j=0; 
      i++;
    }
    if (i == mat->rows){
      break;
    }

  }

  if (count != mat->rows * mat->cols){
    printf("mm_read: failed to read matrix values.\n");
    exit(-1);
  }


  return 0;
}

matrix* mm_matrix_mult(matrix* mat1, matrix* mat2)
{
/*
Notes:
  First check that it is actually possible to multiply the two inputs
  If so, construct a result matrix
  Then perform the multiplication
  Then return a pointer to your result matrix
*/
  if (mat1->cols != mat2->rows){
    printf("mm_matrix_mult: dimension mismatch between matrices.\n");
    exit(-1);
  }

  matrix* result_matrix = (matrix*)malloc(sizeof(matrix));
  result_matrix->rows = mat1->rows;
  result_matrix->cols = mat2->cols;

  result_matrix->data = (double**)calloc(result_matrix->rows, sizeof(double));

  for (int i=0; i<result_matrix->rows; ++i){
    result_matrix->data[i] = (double*)calloc(result_matrix->cols, sizeof(double));
  }

  for (int i=0; i < result_matrix->rows; ++i){
    for (int j=0; j < result_matrix->cols; ++j){

      for (int k=0; k< mat2->rows; ++k){
        result_matrix->data[i][j] += mat1->data[i][k] * mat2->data[k][j];
      } 
    }
  }

  return result_matrix;
}

int main()
{
  /*
   * you must keep this function AS IS. 
   * you cannot modify it!
   */
  char filename_A[256];
  char filename_B[256];
  matrix* A = (matrix*)malloc(sizeof(matrix));
  matrix* B = (matrix*)malloc(sizeof(matrix));
  matrix* C = NULL;

  printf("Please enter the matrix A data file name: ");
  if (1 != scanf("%s", filename_A)) {
    printf("main: scanf failed to read filename\n");
    exit(-1);
  }
  mm_read(filename_A, A);
  mm_print(A);
  
  printf("Please enter the matrix B data file name: ");
  if (1 != scanf("%s", filename_B)) {
    printf("main: scanf failed to read filename\n");
    exit(-1);
  }
  mm_read(filename_B, B);
  mm_print(B);
  
  C = mm_matrix_mult(A, B);
  mm_print(C);

  mm_free(A);
  mm_free(B);
  mm_free(C);
  free(A);
  free(B);
  free(C);
  
  return 0;
}
