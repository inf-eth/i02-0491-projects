#define PRECISION double

#ifdef cl_amd_fp64
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#else
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#endif

#define Matrix(i,j) Matrix_[Cols*(i)+(j)]
#define MatrixA(i,j) MatrixA_[Cols*(i)+(j)]
#define MatrixB(i,j) MatrixB_[Cols*(i)+(j)]
#define MatrixBt(i,j) MatrixBt_[Cols*(i)+(j)]
#define MatrixC(i,j) MatrixC_[Cols*(i)+(j)]

// Kernel functions.
__kernel void MatrixMultiplicationNaiveGPUKernel(__global PRECISION* MatrixA_, __global PRECISION* MatrixB_, __global PRECISION* MatrixC_, const unsigned int Rows, const unsigned int Cols)
{
	unsigned int i = get_global_id(0);
	unsigned int j = get_global_id(1);
	for (unsigned int k=0; k<Cols; k++)
		MatrixC(i,j) = MatrixC(i,j) + MatrixA(i,k) * MatrixB(k,j);
}
__kernel void MatrixMultiplicationNaiveGPUHeterogenousKernel(__global PRECISION* MatrixA_, __global PRECISION* MatrixB_, __global PRECISION* MatrixC_, const unsigned int Rows, const unsigned int Cols, const unsigned int ThreadStart, const unsigned int ThreadEnd)
{
	unsigned int i = get_global_id(0) + ThreadStart;
	unsigned int j = get_global_id(1);
	for (unsigned int k=0; k<Cols; k++)
		MatrixC(i,j) = MatrixC(i,j) + MatrixA(i,k) * MatrixB(k,j);
}
