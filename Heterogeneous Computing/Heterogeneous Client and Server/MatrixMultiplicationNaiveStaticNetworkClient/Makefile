DEPTH = .

include openclsdkdefs.mk 

####
#
#  Targets
#
####

OPENCL			= 1
SAMPLE_EXE		= 1
EXE_TARGET 		= MatrixMultiplicationNaiveStaticNetworkClient
EXE_TARGET_INSTALL   	= MatrixMultiplicationNaiveStaticNetworkClient

####
#
#  C/CPP files
#
####

FILES 	= MatrixMultiplicationNaiveStaticNetworkClientMain MatrixMultiplicationNaiveGPU Timer Client
CLFILES	= MatrixMultiplicationNaiveGPU_Kernels.cl

include openclsdkrules.mk 

