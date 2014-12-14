#include "struct.h"
#include "common.h"

/*
 * Matrix initialization and memory management
 */

struct_coo* new_coo(uint rowlen, uint collen, uint maxsize) {
    struct_coo* res = malloc(sizeof(struct_coo));
    res->values = malloc(sizeof(val*) * maxsize);
    res->locations = malloc(sizeof(location*) * maxsize);
    res->cells = NULL;
    res->len = 0;
    res->cap = maxsize;
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

/*
 * Getting and setting values
 */

GET_VALUE(coo)

int coo_set_value(struct_coo* mat, val v, row r, col c) {
    SET_CHECK(coo)
    IFNEW(coo)
    // Add value and location to the data structure
    mat->values[mat->len] = malloc(sizeof(val));
    *(mat->values[mat->len]) = v;
    location loc = {r, c};
    mat->locations[mat->len] = malloc(sizeof(location));
    *(mat->locations[mat->len]) = loc;
    STOREHASH
    mat->len++;
    ENDIFNEW
    return 0;
}

/*
 * Matrix algebra
 */

struct_coo* coo_matrix_add(struct_coo* mat1, struct_coo* mat2) {
    if (mat1->nrows != mat2->nrows || mat1->ncols != mat2->ncols ) {
        return 0;
    }
    // Strategy: copy all values from first matrix. Then add values from second
    // matrix. 
    //
    // Complexity analysis: We iterate over all nonzero values of both cells.
    // During each loop we do a simple constant-time value retrieval and
    // assignment. Thus the complexity is O(M1 + M2).
    struct_coo* res = new_coo(mat1->nrows, mat1->ncols, mat1->len);
    cell *newitem, *item, *tmp, *tmp2, *c = NULL;
    val* v;
    HASH_ITER(hh, mat1->cells, item, tmp) {
        coo_set_value(res, *item->value, item->loc.r, item->loc.c);
    }
    tmp = NULL;
    item = NULL;
    HASH_ITER(hh, mat2->cells, item, tmp) {
        v = coo_get_value(res, item->loc.r, item->loc.c);
        coo_set_value(res, (v?*v:0) + *item->value, item->loc.r, item->loc.c);
    }
    return res;
}

struct_coo* coo_matrix_mult(struct_coo* mat1, struct_coo* mat2) {
    // Strategy: Take the Cartesian product of the values-locations and add the
    // value to the location if the product of the values should be in the sum
    // of the cell.
    //
    // Complexity analysis: We iterate over all nonzero entries in both
    // matrices. This Takes M1*M2 loop executions. In each loop, we do a
    // comparison, value retrieval (constant time because of hash tables) and
    // assignment (also constant time because we add the value to the end of
    // the list and hash table addition is constant time). Thus, the total
    // complexity is O(M1 * M2).
    if (mat1->ncols != mat2->nrows) {
        return 0;
    }
    struct_coo* res = new_coo(mat1->nrows, mat2->ncols, mat1->len);
    int i, j;
    for (i = 0; i < mat1->len; i++) {
        location* loc1 = mat1->locations[i];        
        val* val1 = mat1->values[i];
        for (j = 0; j < mat2->len; j++) {
            location* loc2 = mat2->locations[j];
            if (loc1->c == loc2->r) {
                val* val2 = mat2->values[j];
                val *current = coo_get_value(res, loc1->r, loc2->c);
                coo_set_value(res, (current?*current:0) + *val1 * *val2, loc1->r, loc2->c);
            }
        }
    }
    return res;
}
