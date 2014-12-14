# Succinct representation of sparse matrices

A project work in Algorithms course.

## Project goal

The aim of this project is to study several data structures for representing
sparse matrices. We look at the space complexity of the structures. We also
study the possible algorithms for basic arithmetic operations multiplication and
addition. We will also implement the determinant computation.

We also implement a number of selected structures and the corresponding
operations. We will give an experimental comparison on the speed of the
implementations.

## Materials

There is some related literature considering the topic (available upon request):

  * Bell, N. & Garland, M.: *Implementing sparse matrix-vector multiplication on throughput-oriented processors*
  * Bell, N. & Garland, M.: *Efficient sparse matrix-vector multiplication on CUDA*
  * Dongarraxz, J.; Lumsdaine, A.; Niu, X.; Pozoz, R. & Remingtonx, K.: *A sparse matrix library in C++ for high performance architectures*
  * Duff, I. S.; Erisman, A. M. & Reid, J. K.: *Direct methods for sparse matrices*
  * ...

However, most of them are rather specific for some certain architectures and
cover mostly matrix-vector multiplication. We are more interested in
matrix-matrix multiplication. Thus we have come up with our own algorithm for
linear algebra.

## Implemented data structures

  * Coordinate list (abbreviated COO). Actually, this implementation uses hash
    tables extensively to speed up computation.
  * Compressed row storage (abbreviated CRS)

One of the goals was also to implement ELLPACK structure, but this was discarded
because of time constraints.

## Improvements

We have implemented hash table search for values for COO representation. This
allows to decrease the search complexity of certain values. There are some
attempts to represent the matrices as hash tables but there have not been
reasonable algorithms for linear algebra.

## License

The software is licensed under MIT license (see `LICENSE`).

This software uses hash table implementation by Troy D. Hanson. This software is
licensed under BSD license (see `LICENSE.BSD`).
