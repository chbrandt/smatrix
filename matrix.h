/* 
 * File:   matrix.h
 * Author: chbrandt
 *
 * Created on August 7, 2009, 5:14 PM
 */

#ifndef _MATRIX_H
#define	_MATRIX_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* _MATRIX_H */


typedef struct matrix Matrix;


Matrix* matrix_create(void);
Matrix* matrix_multiply(Matrix* , Matrix* );
Matrix* matrix_add(Matrix* , Matrix* );
Matrix* matrix_transpose(Matrix* );
float matrix_getelem(Matrix* , int i, int j);
void matrix_setelem(Matrix* , int i, int j, float data);
void matrix_print(Matrix* );
void matrix_destroy(Matrix* );
