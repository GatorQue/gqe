# Locate the GQE library
#
# This module defines the following variables:
# - For each module XXX (CORE):
#   - GQE_XXX_LIBRARY_DEBUG, the name of the debug library of the xxx module (set to GQE_XXX_LIBRARY_RELEASE is no debug version is found)
#   - GQE_XXX_LIBRARY_RELEASE, the name of the release library of the xxx module (set to GQE_XXX_LIBRARY_DEBUG is no release version is found)
#   - GQE_XXX_LIBRARY, the name of the library to link to for the xxx module (includes both debug and optimized names if necessary)
#   - GQE_XXX_FOUND, true if either the debug or release library of the xxx module is found
# - GQE_LIBRARIES, the list of all libraries corresponding to the required modules
# - GQE_FOUND, true if all the required modules are found
# - GQE_INCLUDE_DIR, the path where GQE headers are located (the directory containing the GQE/Config.hpp file)
#
# By default, the dynamic libraries of GQE will be found. To find the static ones instead,
# you must set the GQE_STATIC_LIBRARIES variable to TRUE before calling find_package(GQE ...).
#
# If GQE is not installed in a standard path, you can use the GQEDIR CMake variable
# to tell CMake where GQE is.

# deduce the libraries suffix from the options
set(FIND_GQE_LIB_SUFFIX "")
if(GQE_STATIC_LIBRARIES)
  set(FIND_GQE_LIB_SUFFIX "${FIND_GQE_LIB_SUFFIX}-s")
endif()

# find the GQE include directory
find_path(GQE_INCLUDE_DIR GQE/Config.hpp
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/
  /usr/
  /sw          # Fink
  /opt/local/  # DarwinPorts
  /opt/csw/    # Blastwave
  /opt/
  ${GQEDIR}
  $ENV{GQEDIR})

# check the version number
set(GQE_VERSION_OK TRUE)
if(GQE_FIND_VERSION AND GQE_INCLUDE_DIR)
  # extract the major and minor version numbers from GQE/Config.hpp
  FILE(READ "${GQE_INCLUDE_DIR}/GQE/Config.hpp" GQE_CONFIG_HPP_CONTENTS)
  STRING(REGEX MATCH ".*#define GQE_VERSION_MAJOR ([0-9]+).*#define GQE_VERSION_MINOR ([0-9]+).*" GQE_CONFIG_HPP_CONTENTS "${GQE_CONFIG_HPP_CONTENTS}")
  STRING(REGEX REPLACE ".*#define GQE_VERSION_MAJOR ([0-9]+).*" "\\1" GQE_VERSION_MAJOR "${GQE_CONFIG_HPP_CONTENTS}")
  STRING(REGEX REPLACE ".*#define GQE_VERSION_MINOR ([0-9]+).*" "\\1" GQE_VERSION_MINOR "${GQE_CONFIG_HPP_CONTENTS}")
  math(EXPR GQE_REQUESTED_VERSION "${GQE_FIND_VERSION_MAJOR} * 10 + ${GQE_FIND_VERSION_MINOR}")

  # if we could extract them, compare with the requested version number
  if (GQE_VERSION_MAJOR)
    # transform version numbers to an integer
    math(EXPR GQE_VERSION "${GQE_VERSION_MAJOR} * 10 + ${GQE_VERSION_MINOR}")

    # compare them
    if(GQE_VERSION LESS GQE_REQUESTED_VERSION)
      set(GQE_VERSION_OK FALSE)
    endif()
  else()
    # GQE version is < 2.0
    if (GQE_REQUESTED_VERSION GREATER 19)
      set(GQE_VERSION_OK FALSE)
      set(GQE_VERSION_MAJOR 1)
      set(GQE_VERSION_MINOR x)
    endif()
  endif()
endif()

# find the requested components
set(GQE_FOUND TRUE) # will be set to false if one of the required modules is not found
set(FIND_GQE_LIB_PATHS ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  ${GQEDIR}
  $ENV{GQEDIR})

#set(FIND_GQE_PATH_SUFFIXES lib64 lib)
#if (WINDOWS AND COMPILER_GCC)
#	set(FIND_GQE_PATH_SUFFIXES ${FIND_GQE_PATH_SUFFIXES} libs-mingw)
#elseif(MSVC)
#	set(FIND_GQE_PATH_SUFFIXES ${FIND_GQE_PATH_SUFFIXES} libs-msvc)
#elseif(MACOSX)
#	set(FIND_GQE_PATH_SUFFIXES ${FIND_GQE_PATH_SUFFIXES} libs-osx)
#else()
#	set(FIND_GQE_PATH_SUFFIXES ${FIND_GQE_PATH_SUFFIXES} libs-gcc)
#endif()

foreach(FIND_GQE_COMPONENT ${GQE_FIND_COMPONENTS})
  string(TOLOWER ${FIND_GQE_COMPONENT} FIND_GQE_COMPONENT_LOWER)
  string(TOUPPER ${FIND_GQE_COMPONENT} FIND_GQE_COMPONENT_UPPER)

  # Clear out previously set values
  unset(GQE_${FIND_GQE_COMPONENT_UPPER}_FOUND CACHE)
  unset(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY CACHE)
  unset(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_DEBUG CACHE)
  unset(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_RELEASE CACHE)

  # Define the GQE component name
  set(FIND_GQE_COMPONENT_NAME gqe-${FIND_GQE_COMPONENT_LOWER}${FIND_GQE_LIB_SUFFIX})

  # debug library
  find_library(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_DEBUG
    ${FIND_GQE_COMPONENT_NAME}-d
    PATH_SUFFIXES lib64 lib
    PATHS ${FIND_GQE_LIB_PATHS})

  # release library
  find_library(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_RELEASE
    ${FIND_GQE_COMPONENT_NAME}
    PATH_SUFFIXES lib64 lib
    PATHS ${FIND_GQE_LIB_PATHS})

  if (GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_DEBUG OR GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_RELEASE)
    # library found
    set(GQE_${FIND_GQE_COMPONENT_UPPER}_FOUND TRUE)

    # if both are found, set GQE_XXX_LIBRARY to contain both
    if (GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_DEBUG AND GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_RELEASE)
      # This causes problems with building under NMake Makefiles (command line Visual Studio)
      set(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY debug     ${GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_DEBUG}
        optimized ${GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_RELEASE})

      # Use this instead if your using NMake and you have problems compiling/linking
      #set(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY        ${GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_RELEASE})
    endif()

    # if only one debug/release variant is found, set the other to be equal to the found one
    if (GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_DEBUG AND NOT GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_RELEASE)
      # debug and not release
      set(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_RELEASE ${GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_DEBUG})
      set(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY         ${GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_DEBUG})
    endif()
    if (GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_RELEASE AND NOT GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_DEBUG)
      # release and not debug
      set(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_DEBUG ${GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_RELEASE})
      set(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY       ${GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_RELEASE})
    endif()
  else()
    # library not found
    set(GQE_FOUND FALSE)
    set(GQE_${FIND_GQE_COMPONENT_UPPER}_FOUND FALSE)
    set(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY "")
    set(FIND_GQE_MISSING "${FIND_GQE_MISSING} GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY")
  endif()

  # mark as advanced
  MARK_AS_ADVANCED(GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY
    GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_RELEASE
    GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY_DEBUG)

  # add to the global list of libraries
  set(GQE_LIBRARIES ${GQE_LIBRARIES} "${GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY}")
endforeach()

# handle errors
if(NOT GQE_VERSION_OK)
  # GQE version not ok
  set(FIND_GQE_ERROR "GQE found but version too low (requested: ${GQE_FIND_VERSION}, found: ${GQE_VERSION_MAJOR}.${GQE_VERSION_MINOR})")
  set(GQE_FOUND FALSE)
elseif(NOT GQE_FOUND)
  # include directory or library not found
  set(FIND_GQE_ERROR "Could NOT find GQE (missing: ${FIND_GQE_MISSING})")
endif()
if (NOT GQE_FOUND)
  if(GQE_FIND_REQUIRED)
    # fatal error
    message(FATAL_ERROR ${FIND_GQE_ERROR})
  elseif(NOT GQE_FIND_QUIETLY)
    # error but continue
    message(STATUS "${FIND_GQE_ERROR}")
  endif()
endif()

# handle success
if(GQE_FOUND)
  message(STATUS "Found GQE: ${GQE_INCLUDE_DIR}")
endif()
