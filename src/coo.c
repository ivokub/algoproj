#include "struct.h"

struct_coo* new_coo(uint rowlen, uint collen, uint maxsize) {
    struct_coo* res = malloc(sizeof(struct_coo) * maxsize);
    res->values = malloc(sizeof(val));
    res->locations = malloc(sizeof(location));
    res->cells = NULL;
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
    // If the user requests too small amount of memory, then allocate
    // some prematurely.
    if (mat->cap * 2 < newsize) {
        mat->values = realloc(mat->values, sizeof(val*) * newsize);
        mat->locations = realloc(mat->locations, sizeof(location*) * newsize);
        mat->cap = newsize;
    } else {
        mat->values = realloc(mat->values, sizeof(val*) * 2 * mat->cap);
        mat->locations = realloc(mat->locations, sizeof(location*) * 2 * mat->cap);
        mat->cap = 2 * mat->cap;
    }
    if (mat->values == NULL || mat->locations == NULL) {
        return -1;
    }
    return 0;
}

int coo_set_value(struct_coo* mat, val v, row r, col c) {
    // Boundaries
    if (r + 1 > mat->nrows || c + 1 > mat->ncols ) {
        return -1;
    }

    if (_coo_increase(mat, mat->len + 1)) {
        return -1;
    }

    // If the value is already in the table, then change it.
    val *existing = coo_get_value(mat, r, c);
    if (!existing) {
        // Add value and location to the data structure
        mat->values[mat->len] = malloc(sizeof(val));
        *(mat->values[mat->len]) = v;
        location loc = {r, c};
        mat->locations[mat->len] = malloc(sizeof(location));
        *(mat->locations[mat->len]) = loc;

        // Record the location in hashtable
        cell *in = malloc(sizeof(cell));
        memset(in, 0, sizeof(cell));
        in->loc.r = r;
        in->loc.c = c;
        in->value = mat->values[mat->len];
        HASH_ADD(hh, mat->cells, loc, sizeof(location), in);
        mat->len++;
    } else {
       *existing = v;
    }
    return 0;
}

val* coo_get_value(struct_coo* mat, row r, col c) {
    // Boundaries
    if (r + 1 > mat->nrows || c + 1 > mat->ncols ) {
        return 0;
    }
    // Find the value from hash table
    cell cl;
    memset(&cl, 0, sizeof(cell));
    cl.loc.r = r;
    cl.loc.c = c;
    cell* res;
    HASH_FIND(hh, mat->cells, &cl.loc, sizeof(location), res);
    if (res) {
        return res->value;
    }
    return 0;
}

struct_coo* coo_matrix_mult(struct_coo* mat1, struct_coo* mat2) {
    return 0;
}
