#################################################################################
# bladeRF Spectrogram

set(EXECUTABLE_NAME ${PROJECT_NAME})
set(SOURCE_FILES bladeRFSpectrogram.cpp)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_BUILD_TYPE MATCHES "[Tt][Bb][Bb]")
	return( )
endif()

# Auto-select bitness based on platform
if(NOT BITNESS)
	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(BITNESS 64)
	else()
		set(BITNESS 32)
	endif()
endif()

# Select bitness for non-msvc platform. Can be specified as -DBITNESS=32/64 at command-line
if( NOT MSVC )
	set(BITNESS ${BITNESS} CACHE STRING "Specify bitness")
	set_property(CACHE BITNESS PROPERTY STRINGS "64" "32")
endif()

if( BITNESS EQUAL 64 )
	set(BITNESS_SUFFIX x86_64)
elseif( BITNESS EQUAL 32 )
	set(BITNESS_SUFFIX x86)
else()
	message( FATAL_ERROR "Bitness specified is invalid" )
endif()

# Set CMAKE_BUILD_TYPE (default = Release)
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
	set(CMAKE_BUILD_TYPE Release)
endif()

# Set platform
if( NOT UNIX )
	set(PLATFORM win)
else()
	set(PLATFORM lnx)
endif()

# Find libbladeRF
find_path(BLADERF_INCLUDE_DIR libbladeRF.h HINTS $ENV{BLADERF_INCLUDE_DIR} "C:\\Program Files\\bladeRF\\include")
find_library(BLADERF_LIBRARY NAMES bladeRF libbladeRF HINTS $ENV{BLADERF_LIBRARY} "C:\\Program Files\\bladeRF\\x64")

if(NOT BLADERF_INCLUDE_DIR OR NOT BLADERF_LIBRARY)
	message(FATAL_ERROR "Could not find libbladeRF. Set BLADERF_INCLUDE_DIR and BLADERF_LIBRARY manually.")
endif()
set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} ${BLADERF_LIBRARY})

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})

############################ OpenMP Support ############################
# Place this after add_executable()
# Define an option for enabling/disabling OpenMP
option(ENABLE_OPENMP "Enable OpenMP support" ON)
# Check for OpenMP support
if(ENABLE_OPENMP)
	find_package(OpenMP REQUIRED)
endif()
# Link OpenMP if enabled
if(ENABLE_OPENMP AND OpenMP_CXX_FOUND)
	#target_link_libraries(${EXECUTABLE_NAME} PUBLIC OpenMP::OpenMP_CXX)
	set(OPENMP_SUPPORT OpenMP::OpenMP_CXX)
	set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} ${OPENMP_SUPPORT} )
endif()
########################################################################

# gcc/g++ specific compile options
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(COMPILER_FLAGS "${COMPILER_FLAGS} -msse2 ")
	
	# Note: "rt" is not present on mingw
	if(UNIX)
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			set(COMPILER_FLAGS " -g ")
		endif( )
		set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} "rt")
	endif()
	
	if(BITNESS EQUAL 32)
		set(COMPILER_FLAGS "${COMPILER_FLAGS} -m32 ")
		set(LINKER_FLAGS "${LINKER_FLAGS} -m32 ")
	else()
		set(COMPILER_FLAGS "${COMPILER_FLAGS} -m64 ")
		set(LINKER_FLAGS "${LINKER_FLAGS} -m64 ")
	endif()
	
	set(COMPILER_FLAGS "${COMPILER_FLAGS} ${EXTRA_COMPILER_FLAGS_GXX} ")
	set(LINKER_FLAGS "${LINKER_FLAGS} ${EXTRA_LINKER_FLAGS_GXX} " )
	set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} ${EXTRA_LIBRARIES_GXX})
elseif( MSVC )
	# Samples can specify additional libs/flags using EXTRA* defines
	add_definitions("/W3 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo")

	set(COMPILER_FLAGS "${COMPILER_FLAGS} ${EXTRA_COMPILER_FLAGS_MSVC} ")
	set(LINKER_FLAGS "${LINKER_FLAGS} ${EXTRA_LINKER_FLAGS_MSVC}  /SAFESEH:NO ")
	set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} ${EXTRA_LIBRARIES_MSVC})
endif()

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
						COMPILE_FLAGS ${COMPILER_FLAGS}
						LINK_FLAGS ${LINKER_FLAGS}
					 )

target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${ADDITIONAL_LIBRARIES})
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${BLADERF_INCLUDE_DIR})

# target_link_libraries(bladeRFApp PRIVATE ${BLADERF_LIBRARY})
# Set output directory to bin
if( MSVC )
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/${BITNESS_SUFFIX})
else()
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/${BITNESS_SUFFIX}/${CMAKE_BUILD_TYPE})
endif()

# Copy extra files to binary directory
foreach(extra_file ${EXTRA_FILES})
	add_custom_command(
		TARGET ${EXECUTABLE_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/${extra_file}  ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/${extra_file}  ./
		)
endforeach(extra_file)

if(NOT MSVC)
	add_custom_target(
		run
		COMMAND ${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE_NAME}
		DEPENDS ${EXECUTABLE_NAME}
		COMMENT "Running..."
	)
endif()
# Group sample based on FOLDER_GROUP defined in parent folder
if(FOLDER_GROUP)
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES FOLDER ${FOLDER_GROUP})
endif( )
