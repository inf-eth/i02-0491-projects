#define PRECISION double

#ifdef cl_amd_fp64
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#else
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#endif

#define Matrix(i,j) Matrix_[Cols*(i)+(j)]
#define MatrixA(i,j) MatrixA_[Cols*(i)+(j)]
#define MatrixB(i,j) MatrixB_[Cols*(i)+(j)]
#define MatrixC(i,j) MatrixC_[Cols*(i)+(j)]


// Kernel function for CoreiX and nVidia GPUs. This kernel will be optimised by compiler. Use as is.
__kernel void MatrixMultiplicationNaiveGPUKernel(__global PRECISION* MatrixA_, __global PRECISION* MatrixB_, __global PRECISION* MatrixC_, const unsigned int Rows, const unsigned int Cols)
{
	unsigned int i = get_global_id(0);
	unsigned int j = get_global_id(1);
	for (unsigned int k=0; k<Cols; k++)
		MatrixC(i,j) = MatrixC(i,j) + MatrixA(i,k) * MatrixB(k,j);
}

// Kernel function for Core 2 Duo and AMD GPUs. MatrixC is in global memory and repeated accesses slows down computation for these devices.
__kernel void MatrixMultiplicationNaiveGPUOptimisedKernel(__global PRECISION* MatrixA_, __global PRECISION* MatrixB_, __global PRECISION* MatrixC_, const unsigned int Rows, const unsigned int Cols)
{
	unsigned int i = get_global_id(0);
	unsigned int j = get_global_id(1);
	PRECISION Sum = 0.;
	for (unsigned int k=0; k<Cols; k++)
		Sum = Sum + MatrixA(i,k) * MatrixB(k,j);
	MatrixC(i,j) = Sum;
}
