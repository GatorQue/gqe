# Project master CMakeList.txt file
# 
# Reference(s):
# http://www.itk.org/Wiki/CMake/Tutorials/How_to_create_a_ProjectConfig.cmake_file
# http://www.sfml-dev.org/
# @date 20110421 RQL Modified to match example in reference(s) above

# Make sure they are using a recent version of CMake
cmake_minimum_required(VERSION 2.6)

# set a default build type if none was provided
# this has to be done before the project() instruction!
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# project name
project(GQE)

# project options
OPTION(BUILD_SHARED_LIBS "Set to OFF to build static libraries" OFF)
OPTION(BUILD_EXAMPLES "Set to OFF to skip building examples" ON) 
OPTION(INSTALL_DOC "Set to OFF to skip build/install Documentation" ON)

# setup version numbers
set(GQE_VERSION_MAJOR 0)
set(GQE_VERSION_MINOR 7)
set(GQE_VERSION_PATCH 0)

# define the path of our additional CMake modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/")

# include the configuration file after the project options are defined
include(${PROJECT_SOURCE_DIR}/cmake/Config.cmake)

# set up include-directories
include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/extlibs/headers")

# disable the rpath stuff
set(CMAKE_SKIP_BUILD_RPATH TRUE)

# add the subdirectories
add_subdirectory(src/GQE)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(INSTALL_DOC)
    add_subdirectory(doc)
endif()

# Add all targets to the build-tree export set
export(TARGETS gqe-core
  FILE "${PROJECT_BINARY_DIR}/GQE_LibraryDepends.cmake")
 
# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE GQE)
 
# Create a GQE_Config.cmake file for the use from the build tree
set(GQE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
set(GQE_LIB_DIR "${PROJECT_BINARY_DIR}")
set(GQE_CMAKE_DIR "${PROJECT_BINARY_DIR}")
configure_file(gqe-config.cmake.in
  "${PROJECT_BINARY_DIR}/gqe-config.cmake" @ONLY)
configure_file(gqe-configversion.cmake.in
  "${PROJECT_BINARY_DIR}/gqe-configversion.cmake" @ONLY)

# Install the export set for use with the install-tree
install(EXPORT GQE_LibraryDepends DESTINATION
  "${INSTALL_DATA_DIR}/CMake"
  COMPONENT dev)
 
# Create a GQE_Config.cmake file for the use from the install tree
# and install it
set(GQE_INCLUDE_DIRS "${INSTALL_INCLUDE_DIR}")
set(GQE_LIB_DIR "${INSTALL_LIB_DIR}")
set(GQE_CMAKE_DIR "${INSTALL_DATA_DIR}/CMake")
configure_file(gqe-config.cmake.in
  "${PROJECT_BINARY_DIR}/InstallFiles/gqe-config.cmake" @ONLY)
configure_file(gqe-configversion.cmake.in
  "${PROJECT_BINARY_DIR}/InstallFiles/gqe-configversion.cmake" @ONLY)

# install our config and config version files
install(FILES
  "${PROJECT_BINARY_DIR}/InstallFiles/gqe-config.cmake"
  "${PROJECT_BINARY_DIR}/InstallFiles/gqe-configversion.cmake"
  DESTINATION "${GQE_CMAKE_DIR}" COMPONENT dev)

# install our FindGQE cmake module into the Cmake Modules directory
install(FILES ${PROJECT_SOURCE_DIR}/cmake/Modules/FindGQE.cmake DESTINATION ${CMAKE_ROOT}/Modules)

# install our license file into our INSTALL_DATA_DIR
install(FILES license.txt DESTINATION ${INSTALL_DATA_DIR})

if(WINDOWS)
    install(FILES extlibs/bin/libsndfile-1.dll DESTINATION bin)
    install(FILES extlibs/bin/openal32.dll DESTINATION bin)
elseif(MACOSX)
    install(DIRECTORY extlibs/libs-osx/Frameworks/sndfile.framework DESTINATION /Library/Frameworks PATTERN "hg" EXCLUDE)
endif()
