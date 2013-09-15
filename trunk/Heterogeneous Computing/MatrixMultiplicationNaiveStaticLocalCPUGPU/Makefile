DEPTH = .

include openclsdkdefs.mk 

####
#
#  Targets
#
####

OPENCL			= 1
SAMPLE_EXE		= 1
EXE_TARGET 		= MatrixMultiplicationNaiveStaticLocalCPUGPU
EXE_TARGET_INSTALL   	= MatrixMultiplicationNaiveStaticLocalCPUGPU

####
#
#  C/CPP files
#
####

FILES 	= MatrixMultiplicationNaiveStaticLocalCPUGPUMain MatrixMultiplicationNaiveGPU Timer
CLFILES	= MatrixMultiplicationNaiveGPU_Kernels.cl

include openclsdkrules.mk 

