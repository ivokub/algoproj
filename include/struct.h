#include <stdlib.h>

#ifndef _TYPE_H
#define _TYPE_H
#define val unsigned long long
#define row unsigned long long
#define col unsigned long long
#define uint unsigned int

#define ERR_MALLOC -1
#endif

#ifndef _STRUCT_COO_H
#define _STRUCT_COO_H
typedef struct STRUCT_COO {
    uint len;
    uint cap;
    uint nrows;
    uint ncols;
    val* values;
    row* rows;
    col* cols;
} struct_coo;
struct_coo* new_coo(uint, uint);
int coo_set_value(struct_coo*, val, row, col);
val* coo_get_value(struct_coo*, row, col);
#endif

#ifndef _STRUCT_CSR_H
#define _STRUCT_CSR_H
typedef struct STRUCT_CSR {
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
typedef struct STRUCT_ELL {
    uint len;
    uint cap;
    val** values;
    col* cols;
} struct_ell;
struct_coo* convert_ell_coo(struct_ell*);
struct_ell* convert_coo_ell(struct_coo*);
#endif
