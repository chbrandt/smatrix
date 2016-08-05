#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

struct matrix {
    struct matrix* right;
    struct matrix* below;
    int line;
    int column;
    float info;
};


Matrix* matrix_create(void);
Matrix* matrix_multiply(Matrix*, Matrix*);
Matrix* matrix_add(Matrix*, Matrix*);
Matrix* matrix_transpose(Matrix*);
float matrix_getelem(Matrix*, int i, int j);
void matrix_setelem(Matrix*, int i, int j, float data);
void matrix_print(Matrix*);
void matrix_destroy(Matrix*);

Matrix* matrix_create_HeadElems(int Nlinhas, int Ncolunas);
Matrix* matrix_copy_Elem(Matrix*, int i, int j, float data);
void matrix_removeelem(Matrix*, int i, int j);


/*********************************************************************/
/* CREATE MATRIX */
// - input: Read from stdin matrix order and elements;
// - output: pointer to matrix Head node;
// - dep:  matrix_create_HeadElems()
//         matrix_setelem()

Matrix* matrix_create(void) {

    int i, j;                   // line and column for each matrix element
    int Nlinhas, Ncolunas;      // matrix order
    float data;                 // element value
    Matrix* pHead;              // Head node address

    /*
     * Verify whether the user is entering right values for matix dimension.
     */
    printf("\nType the number of raws and columns of the matrix, then hit Return key.\n");
    scanf("%d %d", &Nlinhas, &Ncolunas);
    printf("%d %d\n", Nlinhas, Ncolunas);
    if ((Nlinhas == 0) || (Ncolunas == 0)) {
        printf("\n\nError: a matrix should have at least 1 element.\n\n");
        return NULL;
    }

    /*
     * Create raws and columns head nodes..
     */
    pHead = matrix_create_HeadElems(Nlinhas, Ncolunas);
    if (pHead == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate matrix.\n");
        return NULL;
    }

    /*
     * Reading element values..
     */
    printf("Type the raw, column and value of the elements.\n");
    printf("Finish the input process typing \"0\"\n");
    while (1) {

        scanf("%d", &i);
        /* If first element is null, finish reading procedure.. */
        if (i == 0)
            break;

        scanf("%d", &j);
        /* Also for second element, if it is zero, finish with reading.. */
        if (j == 0)
            break;

        /* Read element/data value.. */
        scanf("%f", &data);

        /* Set given element value.. */
        matrix_setelem(pHead, i, j, data);

    }

    // Return Head node address..
    return pHead;
}
/* -- */


/*********************************************************************/
/* READ ELEMENT (i,j) */
// - input: matrix address, line and column of desired element;
// - output: element value(data);
// - dep: ;

float matrix_getelem(Matrix* pHead, int i, int j) {

    Matrix* p;  // auxiliar pointer to walk through matrix elements.

    p = pHead;

    /* Verify whether required element is inside matrix.. */
    if ((i > -1 * pHead->line) || (j > -1 * pHead->column) || (!i) || (!j)) {
        printf("WARNING: Element (%d,%d) is outside matrix dimension.", i, j);
        printf(" Returning value 0!\n");
        /* If the element is not inside matrix, return zero... */
        return 0;
    }

    /* If the element i,j exist, continue.. */
    
    /* Walk till desired line.. */
    while (p->line != i)
        p = p->below;

    /* Search for required element within columns.. */
    do {
        p = p->right;
    } while ((p->column != -1) && (p->column < j));

    /* If the element exist (allocated), return their value.. */
    if (p->column == j)
        return p->info;

    // Otherwise, return zero..
    return 0;
}
/* -- */


/*********************************************************************/
/* SET ELEMENT (i,j) IN GIVEN MATRIX */
// - input: matrix address, location (i,j) and data/value to set;
// - output: ;
// - dep: matrix_getelem()
//        matrix_removeelem()

void matrix_setelem(Matrix* pHead, int i, int j, float data) {

    Matrix* pnew;       // Auxiliar pointer for element allocation
    Matrix* plhead;     // Pointer to locate line (i)
    Matrix* pchead;     // Pointer to locate column (j)
    Matrix* paux;       // Auxiliar pointer to walk and update addresses

    /*
     * Check whether the given data value is zero. And if the element
     * already exist, if true, remove it. Exit function if any condition.
     */
    if (data == 0) {
        if (matrix_getelem(pHead, i, j))
            matrix_removeelem(pHead, i, j);
        return;
    }

    /* Initialize line/column pointers.. */
    plhead = pHead->below;
    pchead = pHead->right;

    /* Searchs for corresponding element line.. */
    while (plhead->line != i) {
        plhead = plhead->below;
    }
    /* Also, search for correponding element column.. */
    while (pchead->column != j) {
        pchead = pchead->right;
    }

    /* Create/allocate matrix element.. */
    pnew = (Matrix*) malloc(sizeof (Matrix));
    if (pnew == NULL){
        fprintf(stderr, "WARNING: Element (%d,%d) could not be allocated.\n", i, j);
        return;
    }

    pnew->info = data;
    pnew->line = i;
    pnew->column = j;

    /* By this time, we have pointers in head nodes (line and column).  */
    /* With the "line pointer" walk till corresponding element position.. */
    do {
        paux = plhead;
        plhead = plhead->right;
    } while ((plhead->column < j) && (plhead->column != -1));

    /* Check whether it already exist; If yes, just update their value.. */
    if (plhead->column == j) {
        free(pnew);
        plhead->info = data;
        return;
    } else {
        /* Otherwise, update "line" pointers between elements */
        pnew->right = plhead;
        paux->right = pnew;
    }

    /* If a new element was added, update "column" pointers. */
    do {
        paux = pchead;
        pchead = pchead->below;
    } while ((pchead->line < i) && (pchead->line != -1));
    
    pnew->below = pchead;
    paux->below = pnew;

    
    return;
}
/* -- */


/*********************************************************************/
/* PRINT MATRIX */
// - input: Matrix to print;
// - output: Print matrix elements to stdin;
// -dep: ;

void matrix_print(Matrix* pHead) {

    Matrix* paux;
    int i, j, M, N;
    i = j = 0;

    if (pHead != NULL) {

        /* Matrix dimension */
        M = -1 * pHead->line;
        N = -1 * pHead->column;

        /* Walk through allocated elements and print each value and position. */
        for (paux = pHead->below, i = 1; i <= M; paux = paux->below, i++) {
            for (paux = paux->right, j = 1; j <= N; j++) {
                if (paux->column == j) {
                    printf("%d %d %f\n", i, j, paux->info);
                    paux = paux->right;
                }
            }
        }
        printf("0\n");
    }

    return;
}
/* -- */


/*********************************************************************/
/* ADD TWO MATRICES */
// - input: Two matrices, "A" and "B";
// - output: Matrix C=A+B;
// - dep:  matrix_create_HeadElems()
//         matrix_copy_Elem()

Matrix* matrix_add(Matrix* pHeadA, Matrix* pHeadB) {

    int i, ind;                     // index to control line number and column, respct.
    float _info;                    // element(s) scanned value/data
    int Nlinhas, Ncolunas;          // order of input matrices
    Matrix *pHeadSum, *pauxSum;     // auxiliar pointers to manage summed matrix
    Matrix *pauxA, *pauxB;          // auxiliar pointers to scan input matrices
    Matrix *pColAux, *pColSum;      // auxiliar pointers to update summed matrices pointers


    /* Verify both matrices dimensions, whether they are equal. */
    if ((pHeadA->line != pHeadB->line) || (pHeadA->column != pHeadB->column)) {
        printf("\nERROR: matrix_add: Sizes of given matrices do not match.\n\n");
        return NULL;
    }


    Nlinhas = -1 * pHeadA->line;
    Ncolunas = -1 * pHeadA->column;

    /* Create head nodes for new matrix. */
    pHeadSum = matrix_create_HeadElems(Nlinhas, Ncolunas);
    if (pHeadSum == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate matrix.\n");
        return NULL;
    }

    /* Initialize auxiliar pointers to scan matrices */
    pauxA = pHeadA->below;
    pauxB = pHeadB->below;
    pauxSum = pHeadSum->below;
    pColSum = pHeadSum->right;

    /* For each line of matrices... */
    for (i = 0; i < Nlinhas; i++) {

        // Go to the first element allocated..
        pauxA = pauxA->right;
        pauxB = pauxB->right;

        // Both matrices having non-zero elements in that line...
        while ((pauxA->column != -1) && (pauxB->column != -1)) {

            // Check whether elements location match..
            if (pauxA->column < pauxB->column) {

                _info = pauxA->info;
                pauxSum = matrix_copy_Elem(pauxSum, pauxA->line, pauxA->column, _info);
                if (pauxSum == NULL) {
                    fprintf(stderr, "ERROR: Unable to allocate matrix.\n");
                    matrix_destroy(pHeadSum);
                    return NULL;
                }
                pauxA = pauxA->right;

            } else {

                if (pauxB->column < pauxA->column) {

                    _info = pauxB->info;
                    pauxSum = matrix_copy_Elem(pauxSum, pauxB->line, pauxB->column, _info);
                    if (pauxSum == NULL) {
                        fprintf(stderr, "ERROR: Unable to allocate matrix.\n");
                        matrix_destroy(pHeadSum);
                        return NULL;
                    }
                    pauxB = pauxB->right;

                } else {

                    _info = pauxA->info + pauxB->info;
                    pauxSum = matrix_copy_Elem(pauxSum, pauxA->line, pauxA->column, _info);
                    if (pauxSum == NULL) {
                        fprintf(stderr, "ERROR: Unable to allocate matrix.\n");
                        matrix_destroy(pHeadSum);
                        return NULL;
                    }
                    pauxA = pauxA->right;
                    pauxB = pauxB->right;
                }
            }

            // For each element found in each matrix or both(addition being non-zero),
            // update pointers..
            if ( _info ){
                ind = pauxSum->column - 1;
                pColAux = &pColSum[ind];

                while (pColAux->below != &pColSum[ind]) {
                    pColAux = pColAux->below;
                }

                pauxSum->below = pColAux->below;
                pColAux->below = pauxSum;
                pColAux = pauxSum->below;
            }
        }

        // If "pauxA" have returned to line header, read matrix B elements..
        while (pauxB->column != -1) {

            _info = pauxB->info;
            pauxSum = matrix_copy_Elem(pauxSum, pauxB->line, pauxB->column, _info);
            if (pauxSum == NULL) {
                fprintf(stderr, "ERROR: Unable to allocate matrix.\n");
                matrix_destroy(pHeadSum);
                return NULL;
            }
            pauxB = pauxB->right;

            ind = pauxSum->column - 1;
            pColAux = &pColSum[ind];

            while (pColAux->below != &pColSum[ind]) {
                pColAux = pColAux->below;
            }

            pauxSum->below = pColAux->below;
            pColAux->below = pauxSum;
            pColAux = pauxSum->below;

        }

        // If "pauxB" have returned to line header, read matrix A elements..
        while (pauxA->column != -1) {

            _info = pauxA->info;
            pauxSum = matrix_copy_Elem(pauxSum, pauxA->line, pauxA->column, _info);
            if (pauxSum == NULL) {
                fprintf(stderr, "ERROR: Unable to allocate matrix.\n");
                matrix_destroy(pHeadSum);
                return NULL;
            }
            pauxA = pauxA->right;

            ind = pauxSum->column - 1;
            pColAux = &pColSum[ind];
            while (pColAux->below != &pColSum[ind]) {
                pColAux = pColAux->below;
            }

            pauxSum->below = pColAux->below;
            pColAux->below = pauxSum;
            pColAux = pauxSum->below; // Volta o ponteiro para os headers.

        }

        // Finished all scanning on that line, go to next one..
        pauxSum = pauxSum->right;
        pauxA = pauxA->below;
        pauxB = pauxB->below;
        pauxSum = pauxSum->below;
    }

    // Return Head node of new matrix.
    return pHeadSum;
}
/* -- */


/*********************************************************************/
/* MATRICES MULTIPLICATION */
// - input: Two matrices, "A" and "B";
// - output: Matrix C=A*B;
// - dep:  matrix_create_HeadElems()

Matrix* matrix_multiply(Matrix* pHeadA, Matrix* pHeadB) {

    int i, j, ind;
    int Nlinhas, Ncolunas;
    float produto;
    Matrix *pnew, *pauxA, *pauxB;
    Matrix *pHeadProd, *pauxProd;
    Matrix *pColProd, *pColAux;

    /* Verify whether matrices dimensions match for multiplication.. */
    if (pHeadA->column != pHeadB->line) {
        printf("/n ERROR: Multiply: Dimensions of given Matrices don't match"
                " ( [A]n_columns != [B]n_lines )\n\n");
        return NULL;
    }

    Nlinhas = -1 * pHeadA->line;
    Ncolunas = -1 * pHeadB->column;

    /* Create head nodes for new matrix.. */
    pHeadProd = matrix_create_HeadElems(Nlinhas, Ncolunas);
    if (pHeadProd == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate matrix.\n");
        return NULL;
    }

    // Initialize auxiliar pointers..
    pColProd = pHeadProd->right;
    pauxProd = pHeadProd->below;
    pauxA = pHeadA;
    pauxB = pHeadB;

    // For each line of left matrix...
    for (i = 0; i < Nlinhas; i++) {

        // Go to their first or next line..
        pauxA = pauxA->below;

        // For each column of right matrix...
        for (j = 0; j < Ncolunas; j++) {

            // Go to their first or next column..
            pauxB = pauxB->right;
            // And start walking through elements..
            pauxB = pauxB->below;
            pauxA = pauxA->right;

            // Zero the line*column product result..
            produto = 0;

            // While neither one of matrices pointers go back to headers...
            while ((pauxA->column != -1) && (pauxB->line != -1)) {

                // Check for elements matching for multiplication..
                if (pauxA->column < pauxB->line) {
                    pauxA = pauxA->right;
                } else {
                    if (pauxA->column > pauxB->line) {
                        pauxB = pauxB->below;
                    } else {
                        produto += pauxA->info * pauxB->info;

                        pauxA = pauxA->right;
                        pauxB = pauxB->below;
                    }
                }
            }

            // If any multiplication occurred, allocate element for new matrix..
            if (produto) {
                pnew = (Matrix*) malloc(sizeof (Matrix));
                if (pnew == NULL) {
                    fprintf(stderr, "ERROR: Unable to allocate matrix.");
                    matrix_destroy(pHeadProd);
                    return NULL;
                }
                pnew->line = i + 1;
                pnew->column = j + 1;
                pnew->info = produto;
                pnew->right = pauxProd->right;
                pauxProd->right = pnew;
                pauxProd = pnew;

                ind = pauxProd->column - 1; // = j
                pColAux = &pColProd[ind];
                while (pColAux->below != &pColProd[ind])
                    pColAux = pColAux->below;

                pauxProd->below = pColAux->below;
                pColAux->below = pauxProd;
                pColAux = pColAux->below;
            }

            // Update auxiliar pointers positions to head nodes..
            while (pauxA->column != -1)
                pauxA = pauxA->right;
            while (pauxB->line != -1) {
                pauxB = pauxB->below;
            }
        }

        // Go to next Head node of right matrix and headers of new matrix..
        pauxB = pauxB->right;
        pauxProd = pauxProd->right;
        pauxProd = pauxProd->below;

    }


    // Return new matrix address..
    return pHeadProd;

}
/* -- */


/*********************************************************************/
/* MATRIX TRANSPOSE lines<->columns */
// - input: Matrix address to transpose;
// - output: Matrix transposed address;
// - dep:  matrix_create_HeadElems()

Matrix* matrix_transpose(Matrix* pHead) {

    int i, ind;
    int Nlinhas, Ncolunas;
    int NlinhasT, NcolunasT;
    Matrix *paux, *pnew;
    Matrix *pHeadT, *pauxT;
    Matrix *pLinT, *pLinAux;

    Nlinhas = -1 * pHead->line;
    Ncolunas = -1 * pHead->column;
    NlinhasT = Ncolunas;
    NcolunasT = Nlinhas;

    /* Create head nodes for new matrix. */
    pHeadT = matrix_create_HeadElems(NlinhasT, NcolunasT);
    if (pHeadT == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate matrix.\n");
        return NULL;
    }

    paux = pHead->below;
    pauxT = pHeadT->right;
    pLinT = pHeadT->below;

    /* Run through matrix lines reading the elements. */
    for (i = 0; i < Nlinhas; i++) {
        paux = paux->right;
        while (paux->column != -1) {
            pnew = (Matrix*) malloc(sizeof (Matrix));
            if (pnew == NULL) {
                fprintf(stderr, "ERROR: Unable to allocate matrix element.");
                matrix_destroy(pHeadT);
                return NULL;
            }
            pnew->line = paux->column;
            pnew->column = paux->line;
            pnew->info = paux->info;

            pnew->below = pauxT->below;
            pauxT->below = pnew;
            pauxT = pnew;

            ind = paux->column - 1;
            pLinAux = &pLinT[ind];

            while (pLinAux->right != &pLinT[ind]) {
                pLinAux = pLinAux->right;
            }

            pauxT->right = pLinAux->right;
            pLinAux->right = pauxT;
            pLinAux = pauxT->right;

            paux = paux->right;
        }

        pauxT = pauxT->below;
        paux = paux->below;
        pauxT = pauxT->right;
    }
    /* By now, both paux and pauxT are positioned over Head nodes of their matrices. */

    // Return transpose matrix address..
    return pHeadT;
}
/* -- */


/*********************************************************************/
/* DELETE MATRICES */
// - input: Matrix to be deleted;
// - output: ;
// - dep: ;

void matrix_destroy(Matrix* pHead) {

    Matrix* v;
    Matrix* p;
    Matrix* paux;
    int i;
    int M;

    if (pHead == NULL)
        return;

    M = -1 * pHead->line;
    v = pHead->below;

    /* Line-by-line delete  the elements. */
    for (i = 0; i < M; i++) {

        p = v[i].right;

        while (p != &v[i]) {
            paux = p;
            p = p->right;
            free(paux);
        }

    }

    /* And then, delete the Head nodes */
    free(pHead->below);
    free(pHead->right);
    free(pHead);

    return;
}
/* -- */


/*********************************************************************/
/* CREATE ELEMENT NODE; used in matrix_add function */
// - input: element(left) address, line column and data for new element;
// - output: new element address (in case data!=0)
// - dep: ;

Matrix* matrix_copy_Elem(Matrix* pauxSum, int i, int j, float info) {

    Matrix* pnew;

    // Allocate only non-zero elements.
    if ( info != 0 ){
        pnew = (Matrix*) malloc(sizeof (Matrix));
        if (pnew == NULL) {
            return NULL;
        }
        pnew->line = i;
        pnew->column = j;
        pnew->info = info;

        pnew->right = pauxSum->right;
        pauxSum->right = pnew;
        pauxSum = pauxSum->right;
    }
    return pauxSum;
}
/* -- */


/*********************************************************************/
/* CREATE MATRIX HEAD NODES */
// - input: Number of lines and column for new matrix;
// - output: Head node (matrix) address;
// - dep: ;

Matrix* matrix_create_HeadElems(int Nlinhas, int Ncolunas) {

    int i, j;
    Matrix *pHead;
    Matrix *vLHead, *vCHead;

    /* Create the Head node, where matrix information will be stored. */
    pHead = (Matrix*) malloc(sizeof (Matrix));
    if (pHead == NULL) {
        return NULL;
    }
    pHead->line = -1 * Nlinhas;
    pHead->column = -1 * Ncolunas;
    pHead->info = 0;
    pHead->below = pHead;
    pHead->right = pHead;

    vCHead = (Matrix*) malloc(Ncolunas * sizeof (Matrix));
    if (vCHead == NULL) {
        free(pHead);
        return NULL;
    }
    vLHead = (Matrix*) malloc(Nlinhas * sizeof (Matrix));
    if (vLHead == NULL) {
        free(vCHead);
        free(pHead);
        return NULL;
    }

    pHead->right = vCHead;
    pHead->below = vLHead;

    /* Initialize lines head nodes. */
    for (i = 0; i < Nlinhas; i++) {

        vLHead[i].line = i + 1;
        vLHead[i].column = -1;
        vLHead[i].info = 0;
        vLHead[i].below = &vLHead[i + 1];
        vLHead[i].right = &vLHead[i];

    }
    vLHead[Nlinhas - 1].below = pHead;

    /* Create columns head nodes. */
    for (j = 0; j < Ncolunas; j++) {

        vCHead[j].column = j + 1;
        vCHead[j].line = -1;
        vCHead[j].info = 0;
        vCHead[j].right = &vCHead[j + 1];
        vCHead[j].below = &vCHead[j];

    }
    vCHead[Ncolunas - 1].right = pHead;

    // Return the address of head element.
    return pHead;
}
/* -- */


/*********************************************************************/
/* DELETE ELEMENT (i,j) */
// - input: Matrix address, i and j of desired element;
// - output: ;
// - dep: ;

void matrix_removeelem(Matrix* pHead, int i, int j) {

    Matrix* plhead;
    Matrix* pchead;
    Matrix* paux;

    plhead = pHead->below;
    pchead = pHead->right;

    /* Go till head node of corresponding element line */
    while (plhead->line != i) {
        plhead = plhead->below;
    }
    /* Do the same for column. Searchs the corresponding one. */
    while (pchead->column != j) {
        pchead = pchead->right;
    }

    /* Now, walk (through) line till the element. */
    do {
        paux = plhead;
        paux = paux->right;
    } while ((paux->column != j) && (paux->column != -1));
    if (paux->column == -1) {
        return;
    }
    plhead->right = paux->right;


    /* Here, there is also a pointer pointing to corresponding column head node.  */
    do {
        paux = pchead;
        paux = paux->below;
    } while ((paux->line != i) && (paux->line != -1));
    /* Sanity check... :-P */
    if (paux->line == -1) {
        return;
    }
    pchead->below = paux->below;

    free(paux);

    return;
}
/* -- */
