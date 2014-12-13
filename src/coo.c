#include "struct.h"

struct_coo* new_coo(uint rowlen, uint collen) {
    struct_coo* res = malloc(sizeof(struct_coo));
    res->values = malloc(sizeof(val));
    res->rows = malloc(sizeof(row));
    res->cols = malloc(sizeof(col));
    res->len = 0;
    res->cap = 1;
    res->nrows = rowlen;
    res->ncols = collen;
    return res;
}

int _coo_increase(struct_coo* mat, int newsize) {
    if (newsize < mat->cap) {
        // New size should always be larger than current size.
        // Otherwise, we wouldn't know what to do with the residue.
        return 0;
    }
    mat->values = realloc(mat->values, sizeof(val) * newsize);
    mat->rows = realloc(mat->rows, sizeof(val) * newsize);
    mat->cols = realloc(mat->cols, sizeof(val) * newsize);
    mat->cap = newsize;
    if (!(mat->values) || !(mat->rows) || !(mat->cols)) {
        return -1;
    }
    return 0;
}

int coo_set_value(struct_coo* mat, val v, row r, col c) {
    if (r + 1 > mat->nrows || c + 1 > mat->ncols ) {
        return -1;
    }
    int e = _coo_increase(mat, mat->len + 1);
    if (e) {
        return e;
    }
    mat->values[mat->len] = v;
    mat->rows[mat->len] = r;
    mat->cols[mat->len] = c;
    mat->len += 1;
    return 0;
}

val* coo_get_value(struct_coo* mat, row r, col c) {
    if (r + 1 > mat->nrows || c + 1 > mat->ncols ) {
        return 0;
    }
    int i;
    for (i = 0; i < mat->len; i++) {
        if (mat->rows[i] == r && mat->cols[i] == c) {
            return mat->values + i;
        }
    }
    return 0;
}
