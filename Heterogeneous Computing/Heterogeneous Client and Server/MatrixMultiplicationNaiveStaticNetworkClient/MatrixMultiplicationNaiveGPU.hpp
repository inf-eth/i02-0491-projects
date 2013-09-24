#ifndef MatrixMultiplicationNaiveGPU_H_
#define MatrixMultiplicationNaiveGPU_H_

#define PRECISION double

#include <Timer.h>
#include <string>
#include <CL/cl.h>

class CMatrixMultiplicationNaiveGPU
{
private:
	// Size of problem domain
	const unsigned int Rows;
	const unsigned int Cols;

	// Host data arrays.
	PRECISION* MatrixA_;
	PRECISION* MatrixB_;
	PRECISION* MatrixBt_;
	PRECISION* MatrixC_;

	// Device data arrays.
	cl_mem d_MatrixA_;
	cl_mem d_MatrixB_;
	//cl_mem d_MatrixBt_;
	cl_mem d_MatrixC_;

	// Heterogeneous simulation parameters.
	const unsigned int ThreadStart;
	const unsigned int ThreadEnd;

	// ============ OPENCL related parameters ===========
	// OPENCL context/device/program
	cl_context context;
	cl_device_id *devices;
	cl_command_queue commandQueue;
	cl_program program;
	cl_kernel kernel;
	cl_kernel Heterogenouskernel;
	// ==================================================

	// Timer variables.
	__int64 tStart;
	__int64 tEnd;
	__int64 tDelta;
	bool tPaused;

public:
	CMatrixMultiplicationNaiveGPU(unsigned int=1024U, unsigned int=1024U);	// Default size of matrices is 1024x1024.
	// Rows, Cols, Mat A, Mat B, Mat C, Starting Row Thread, Ending Row Thread.
	CMatrixMultiplicationNaiveGPU(unsigned int, unsigned int, PRECISION*, PRECISION*, PRECISION*, unsigned int, unsigned int);

	// Memory allocation and initialisation.
	int AllocateMemoryCPU();
	int InitialiseCPU();
	// Search and allocate a device. Platform (0. User choice, 1. AMD, 2. nVidia), Emulation (0. User choice, 1. CPU, 2. GPU).
	int InitialiseCL(unsigned int=0U, unsigned int=0U);
	int AllocateMemoryGPU();
	int InitialiseCLKernelsGPU(); // Build/attach kernels to respective kernel functions and set arguments.
	int RunCLKernels();
	int RunHeterogeneousKernels(unsigned int=1U);

	// Complete run encapsulating all the sub-functions.
	int CompleteRun();
	int CompleteRunHeterogeneous(unsigned int=1U);

	// Matrix input/output and initialisation.
	void InputRandom(PRECISION*);
	void Display(PRECISION*);
	void Initialise(PRECISION*);

	// Timing.
	void StartTimer();
	void StopTimer();
	void ResetTimer();
	PRECISION GetElapsedTime();

	std::string convertToString(const char * filename);

	int SafeCall(cl_int, const char []=NULL);

	int CleanupCPU();
	int CleanupCL();
	int CleanupGPU();
	~CMatrixMultiplicationNaiveGPU ();
};
#endif // #ifndef MatrixMultiplicationNaiveGPU_H_
