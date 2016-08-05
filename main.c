/* 
 * File:   main.c
 * Author: chbrandt
 *
 * Created on August 7, 2009, 5:12 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

/*
 * 
 */
int main(int argc, char** argv) {

  Matrix *A, *B, *C;

  A = matrix_create();
  fprintf(stderr,"Matriz A:\n");
  matrix_print(A);
  fprintf(stderr,"--\n");

  B = matrix_create();
  fprintf(stderr,"Matriz B:\n");
  matrix_print(B);
  fprintf(stderr,"--\n");

  C = matrix_add(A,B);
  fprintf(stderr,"Matriz C=A+B:\n");
  matrix_print(C);
  fprintf(stderr,"--\n");

  matrix_destroy(C);

  C = matrix_multiply(A,B);
  fprintf(stderr,"Matriz C=A*B:\n");
  matrix_print(C);
  fprintf(stderr,"--\n");

  matrix_destroy(C);

  C = matrix_transpose(A);
  fprintf(stderr,"Matriz C, transposta de A:\n");
  matrix_print(C);
  fprintf(stderr,"--\n");

  matrix_destroy(C);

  matrix_destroy(A);
  matrix_destroy(B);

    return (EXIT_SUCCESS);
}
