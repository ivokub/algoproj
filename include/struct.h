#include <stdlib.h>
#include "uthash.h"

#ifndef _TYPE_H
#define _TYPE_H
#define val unsigned long long
#define row unsigned long long
#define col unsigned long long
#define uint unsigned int

#define ERR_MALLOC -1
#endif

#ifndef _HTABLE_H
#define _HTABLE_H

typedef struct {
    row r;
    col c;
} location;

typedef struct {
    location loc;
    val* value;
    UT_hash_handle hh;
} cell;

int htable_insert(row, col, uint);
int htable_destroy();
uint htable_get(row, col);

#endif

#ifndef _STRUCT_COO_H
#define _STRUCT_COO_H
typedef struct {
    uint len;
    uint cap;
    uint nrows;
    uint ncols;
    val** values;
    location** locations;
    cell *cells;
} struct_coo;
struct_coo* new_coo(uint, uint, uint);
int coo_set_value(struct_coo*, val, row, col);
val* coo_get_value(struct_coo*, row, col);
struct_coo* coo_matrix_add(struct_coo*, struct_coo*);
#endif

#ifndef _STRUCT_CSR_H
#define _STRUCT_CSR_H
typedef struct {
    uint len;
    uint cap;
    val* values;
    row* rows;
    col* cols;
} struct_csr;
struct_coo* convert_csr_coo(struct_csr*);
struct_csr* convert_coo_csr(struct_csr*);
#endif

#ifndef _STRUCT_ELL_H
#define _STRUCT_ELL_H
typedef struct {
    uint len;
    uint cap;
    val** values;
    col* cols;
} struct_ell;
struct_coo* convert_ell_coo(struct_ell*);
struct_ell* convert_coo_ell(struct_coo*);
#endif
