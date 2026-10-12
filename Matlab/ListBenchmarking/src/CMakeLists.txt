# Array and linked list benchmarks
set(EXECUTABLE_NAME ${PROJECT_NAME})

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20 REQUIRED)

find_package(Matlab REQUIRED)
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_NAME}.cpp ${PROJECT_SOURCE_DIR}/include/${EXECUTABLE_NAME}.h)

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${Matlab_INCLUDE_DIRS} PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${Matlab_LIBRARIES})

# Group sample based on FOLDER_GROUP defined in parent folder
if(FOLDER_GROUP)
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES FOLDER ${FOLDER_GROUP})
endif( )
