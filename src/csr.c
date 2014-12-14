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

int csr_set_value(struct_csr* mat, val v, row r, col c) {
    SET_CHECK(csr)
    IFNEW(csr)
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
    STOREHASH
    mat->len++;
    ENDIFNEW
    return 0;
}

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
