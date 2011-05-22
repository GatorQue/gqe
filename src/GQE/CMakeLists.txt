# include the GQE specific macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# add the GQE sources path
include_directories(${CPROJECT_SOURCE_DIR}/src)

# set the output directory for GQE libraries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# set the source directory for include and source files
set(INCROOT ${PROJECT_SOURCE_DIR}/include/GQE)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/GQE)

# define the export symbol
add_definitions(-DGQE_EXPORTS)

# define GQE_STATIC if the build type is not set to 'shared'
if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DGQE_STATIC)
endif()

# add the modules subdirectories
add_subdirectory(Core)

# install Config header include file
install(FILES ${INCROOT}/Config.hpp
        DESTINATION ${INSTALL_INCLUDE_DIR}/GQE
        COMPONENT dev)
