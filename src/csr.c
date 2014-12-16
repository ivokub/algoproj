#include "struct.h"
#include "common.h"

/*
 * Matrix initialization and memory management
 */

struct_csr* new_csr(uint rowlen, uint collen, uint maxsize) {
    struct_csr *res = malloc(sizeof(struct_csr));
    if (!res) {
        return NULL;
    }
    res->values = malloc(sizeof(val*) * maxsize);
    res->colind = malloc(sizeof(col*) * maxsize);
    res->rowptr = malloc(sizeof(val**) * rowlen + 1);
    res->cells = NULL;
    res->len = 0;
    res->cap = maxsize;
    res->nrows = rowlen;
    res->ncols = collen;
    int i;
    for (i = 0; i <= res->nrows; i++) {
        res->rowptr[i] = 0;
    }
    return res;
}

int _csr_increase(struct_csr* mat, int newsize) {
    if (newsize < mat->cap) {
        // New size should always be larger than current size.
        // Otherwise, we wouldn't know what to do with the residue.
        return 0;
    }
    // If the user requests too small amount of memory, then allocate
    // some prematurely.
    if (mat->cap * 2 < newsize) {
        mat->values = realloc(mat->values, sizeof(val*) * newsize);
        mat->colind = realloc(mat->colind, sizeof(col*) * newsize);
        mat->cap = newsize;
    } else {
        mat->values = realloc(mat->values, sizeof(val*) * 2 * mat->cap);
        mat->colind = realloc(mat->colind, sizeof(col*) * 2 * mat->cap);
        mat->cap = 2 * mat->cap;
    }
    if (mat->values == NULL || mat->colind == NULL) {
        return -1;
    }
    return 0;
}

/*
 * Getting and setting values
 */

GET_VALUE(csr)

void _csr_set_value(struct_csr* mat, val v, row r, col c) {
    // We find the correct row first. Then we do a linear search for the
    // right column.
    //
    // TODO: Binary search for finding the right column? Can get the
    // complexity down to logarithmic.
    val* valptr = malloc(sizeof(val));
    col* colind = malloc(sizeof(col));
    *valptr = v;
    *colind = c;
    int i, k, kk;
    i = mat->rowptr[r];
    while (i < mat->rowptr[r+1] && mat->colind[i] && c > *mat->colind[i]) {
        i++;
    }
    for (kk = mat->len; kk > i; kk--) {
        mat->values[kk] = mat->values[kk-1];
        mat->colind[kk] = mat->colind[kk-1];
    }
    mat->colind[kk] = colind;
    mat->values[kk] = valptr;

    for (kk = r+1; kk <= mat->nrows; kk++) {
        mat->rowptr[kk]++;
    }
}

SET_VALUE(csr)

/*
 * Conversion from/to COO structure
 */

struct_coo* convert_csr_coo(struct_csr* mat) {
    struct_coo* res = new_coo(mat->nrows, mat->ncols, mat->len);
    int i, j, k = 0;
    for (i = 0; i < mat->nrows; i++) {
        for (j = mat->rowptr[i]; j < mat->rowptr[i+1]; j++) {
            coo_set_value(res, *mat->values[j], i, *mat->colind[k]);
            k++;
        }
    }
    return res;
}

struct_csr* convert_coo_csr(struct_coo* mat) {
    int i;
    struct_csr* res = new_csr(mat->nrows, mat->ncols, mat->len);
    for (i = 0; i < mat->len; i++) {
        csr_set_value(res, *mat->values[i], mat->locations[i]->r, mat->locations[i]->c);
    }
    return res;
}

/*
 * Matrix algebra
 */

struct_csr* csr_matrix_add(struct_csr* mat1, struct_csr* mat2) {
    if (mat1->nrows != mat2->nrows || mat1->ncols != mat2->ncols ) {
        return 0;
    }
    struct_csr* res = new_csr(mat1->nrows, mat1->ncols, mat1->len + mat2->len);
    int i, j, k, r;
    val *ind1, *ind2;
    uint* row0 = res->rowptr;
    uint* row1 = mat1->rowptr;
    uint* row2 = mat2->rowptr;
    k = 0;
    i = 0;
    j = 0;
    for (r = 0; r < res->nrows; r++) {
        while (i < mat1->rowptr[r+1] || j < mat2->rowptr[r+1] ) {
            val* resv = malloc(sizeof(val));
            col* resc = malloc(sizeof(col));
            res->values[k] = resv;
            res->colind[k] = resc;
            ind1 = mat1->colind[i];
            ind2 = mat2->colind[j];
            if (ind1 && ind2) {
                if (*ind1 == *ind2
                        && i != mat1->rowptr[r+1]
                        && j != mat2->rowptr[r+1]) {
                    *resv = *mat1->values[i] + *mat2->values[j];
                    *resc = *mat1->colind[i];
                    res->values[k] = resv;
                    res->colind[k] = resc;
                    i++;
                    j++;
                } else if ((*ind1 < *ind2
                            && i != mat1->rowptr[r+1])
                        || j == mat2->rowptr[r+1]) {
                    *resv = *mat1->values[i];
                    *resc = *mat1->colind[i];
                    res->values[k] = resv;
                    res->colind[k] = resc;
                    i++;
                } else if ((*ind1 > *ind2
                            && j != mat2->rowptr[r+1])
                        || i == mat1->rowptr[r+1]) {
                    *resv = *mat2->values[j];
                    *resc = *mat2->colind[j];
                    res->values[k] = resv;
                    res->colind[k] = resc;
                    j++;
                }
            } else if (ind1) {
                *resv = *mat1->values[i];
                *resc = *mat1->colind[i];
                res->values[k] = resv;
                res->colind[k] = resc;
                i++;
            } else if (ind2) {
                *resv = *mat2->values[j];
                *resc = *mat2->colind[j];
                res->values[k] = resv;
                res->colind[k] = resc;
                j++;
            }
            k++;
        }
        res->rowptr[r+1] = k;
    }
    res->len = k;
    return res;
}

struct_csr* csr_matrix_mult(struct_csr* mat1, struct_csr* mat2) {
    if (mat1->ncols != mat2->nrows) {
        return 0;
    }
    struct_csr* res = new_csr(mat1->nrows, mat2->ncols, mat1->len);
    int b, i, j, k, r, x, y;
    int rp, cp;
    rp = 0;
    cp = 0;
    k = 0;
    uint* currentcol = malloc(sizeof(uint) * mat2->nrows);
    val* currentrow = malloc(sizeof(val) * mat2->ncols);  // actually min of lengths
    uint* ci = malloc(sizeof(uint) * mat2->ncols);


    for (r = 0; r < mat1->nrows; r++) {
        memset(currentrow, 0, sizeof(val) * mat2->ncols);
        for (x = mat1->rowptr[r]; x < mat1->rowptr[r+1]; x++) {
            for (k = 0; k < mat2->nrows; k++) {
                currentcol[k] = mat2->rowptr[k];
            }
            j = 0;
            while (j < mat2->nrows) {
                if (currentcol[j] != mat2->rowptr[j+1] 
                        && x == *mat2->colind[currentcol[j]]) {
                    currentrow[currentcol[j]] += 
                        *mat1->values[x] * *mat2->values[currentcol[j]];
                    currentcol[j]++;
                } else {
                    j++;
                }
            }
        }
        for (j = 0; j < mat2->nrows; j++) {
            if (currentrow[j]) {
                csr_set_value(res, currentrow[j], r, j);
            }
        }
    }

    return res;
}
