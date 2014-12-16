#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

struct_coo* test_coo_rand(uint, uint, uint);
struct_csr* test_csr_rand(uint, uint, uint);
void print_values(struct_csr*);

int main(char** argc, char** argv) {
    int n, nnz;
    test_coo_rand(1000000, 0, 50000);
    test_coo_rand(1000000, 0, 60000);
    test_coo_rand(1000000, 0, 70000);
    test_coo_rand(1000000, 0, 80000);
    test_coo_rand(1000000, 0, 90000);
    test_coo_rand(1000000, 0, 100000);
    test_coo_rand(1000000, 0, 110000);
    test_coo_rand(1000000, 0, 120000);
    test_coo_rand(1000000, 0, 130000);
    test_coo_rand(1000000, 0, 140000);
}

void print_values(struct_csr* mat1) {
    int i;
    printf("values: ");
    for (i = 0; i < mat1->len; i++) {
        printf("%lld ", *mat1->values[i]);
    }
    printf("\ncolind: ");
    for (i = 0; i < mat1->len; i++) {
        printf("%lld ", *mat1->colind[i]);
    }
    printf("\nrowptr: ");
    for (i = 0; i < mat1->nrows + 1; i++) {
        printf("%d ", mat1->rowptr[i]);
    }
    printf("\n");

}

struct_coo* test_coo_rand(uint size, uint seed, uint nnz) {
    srand(seed);
    int i;
    struct_coo* mat1 = new_coo(size, size, nnz);
    for (i = 0; i < nnz; i++) {
        int x = rand() % size;
        int y = rand() % size;
        int v = rand();
        coo_set_value(mat1, v, y, x);
    }

    struct_coo* mat2 = new_coo(size, size, nnz);
    for (i = 0; i < nnz; i++) {
        int x = rand() % size;
        int y = rand() % size;
        int v = rand();
        coo_set_value(mat2, v, y, x);
    }
    struct timeb start;
    struct timeb end;
    ftime(&start);
    struct_coo* res = coo_matrix_add(mat1, mat2);
    ftime(&end);
    printf("%lld,%d.%d,%d.%d\n", nnz, start.time, start.millitm, end.time, end.millitm);
    return res;
}

struct_csr* test_csr_rand(uint size, uint seed, uint nnz) {
    srand(seed);
    int i;
    struct_coo* mat1 = new_coo(size, size, nnz);
    for (i = 0; i < nnz; i++) {
        int x = rand() % size;
        int y = rand() % size;
        int v = rand();
        coo_set_value(mat1, v, y, x);
    }

    struct_coo* mat2 = new_coo(size, size, nnz);
    for (i = 0; i < nnz; i++) {
        int x = rand() % size;
        int y = rand() % size;
        int v = rand();
        coo_set_value(mat2, v, y, x);
    }
    struct_csr* mat1s = convert_coo_csr(mat1);
    struct_csr* mat2s = convert_coo_csr(mat2);
    struct_csr* res = csr_matrix_mult(mat1s, mat2s);
    return res;
}
