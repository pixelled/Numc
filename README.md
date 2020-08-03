# numc
Numc is a C extension to Python that supports matrix multiplication of addition, subtraction, 
multiplication, exponential, negativity and absolute value. 

## Optimization Overview

### Simple Matrix operations
Matrix operations of addition, subtraction, negation and absolute value simply use 
OpenMP to parallelize computations. By adding `#pragma omp parallel for`, the program
can have an approximately x4 speedup. Using SIMD here doesn't speed up at all.

### Matrix Multiplication
Matrix multiplication uses unrolling, SIMD, OpenMP and some code optimizations to speed up computations. 
Instead of fetching each element in a specific column of the second matrix, I fetch four elements each time 
and store them in a `__m256d` vector to avoid unnecessary accesses to the memory. These vectors are stored
in a `__m256d` array to reduce unnecessary cache misses. In this case, I can directly retrieve a vector from 
this array and construct another vector from the first matrix, doing multiplication with SIMD in a very efficient 
way. To parallelize computations, I add `#pragma omp parallel for private(...)` before the outer for loop so that 
it can parallelize correctly. At this moment, the program can have a x80 speedup.

To further speed up multiplication, I store the results of some repeated computations into variables and gain a 
speedup of 90 in this case. Finally, I unroll the outer loop twice and get a surprising x200 speedup. It seems that
unrolling the outer loop utilizes cache to access continuous memory and thus have much fewer cache misses. 

### Matrix Exponential
Matrix exponential uses an algorithm to speed up the computation by keeping track of the power and an exponential of 
the original matrix. If the power is an even number, then the program divides power by two and stores 
the square of the matrix into a variable. If the power is an odd number, then it multiplies the 
current matrix with the matrix in the variable. This basically has a time complexity of `O(log(n))`, which is very 
efficient. With a speedup of x80 in matrix multiplication, this algorithm can achieve a speedup of x2500 on the 
autograder. In the final version, with a speedup of x200 in matrix multiplication, it can achieve a speedup of x3800, 
which is quite astonishing on my first glance.

