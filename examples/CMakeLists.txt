# define GQE_STATIC if the build type is not set to 'shared'
if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DGQE_STATIC)
endif()

# add the examples subdirectories
add_subdirectory(demo)
