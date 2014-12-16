#ifndef _COMMON_H
#define _COMMON_H

#define GET_VALUE(MAT_T)\
val* MAT_T##_get_value(struct_##MAT_T *mat, row r, col c) { \
    if (r + 1 > mat->nrows || c + 1 > mat->ncols ) { \
        return 0; \
    } \
    cell cl; \
    memset(&cl, 0, sizeof(cell)); \
    cl.loc.r = r; \
    cl.loc.c = c; \
    cell* res; \
    HASH_FIND(hh, mat->cells, &cl.loc, sizeof(location), res); \
    if (res) { \
        return res->value; \
    } \
    return 0; \
}

#define SET_CHECK(MAT_T)\
    if (v == 0) { \
        return 0; \
    } \
    if (r + 1 > mat->nrows || c + 1 > mat->ncols ) { \
        return -1; \
    } \
    if (_##MAT_T##_increase(mat, mat->len + 1)) { \
        return -1; \
    }

#define IFNEW(MAT_T)\
    val *existing = MAT_T##_get_value(mat, r, c); \
    if (!existing) {

#define ENDIFNEW\
    } else { \
       *existing = v; \
    }

#define STOREHASH\
    cell *in = malloc(sizeof(cell)); \
    memset(in, 0, sizeof(cell)); \
    in->loc.r = r; \
    in->loc.c = c; \
    in->value = mat->values[mat->len]; \
    HASH_ADD(hh, mat->cells, loc, sizeof(location), in); \

#define SET_VALUE(MAT_T)\
int MAT_T##_set_value(struct_##MAT_T *mat, val v, row r, col c) { \
    SET_CHECK(MAT_T) \
    IFNEW(MAT_T) \
    _##MAT_T##_set_value(mat, v, r, c); \
    STOREHASH \
    mat->len++; \
    ENDIFNEW \
    return 0; \
}

#endif
