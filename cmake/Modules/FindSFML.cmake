# Locate the SFML library
#
# This module defines
#  SFML_FOUND, if false, do not try to link to SFML
#  SFML_XXX_LIBRARY, library corresponding to the XXX component (release)
#  SFML_XXX_LIBRARY_DEBUG, library corresponding to the XXX component (debug)
#  SFML_LIBRARIES, list containing all the libraries corresponding to the requested components
#  SFML_INCLUDE_DIR, where to find SFML/Config.hpp
#
# By default, the dynamic libraries of SFML will be found. To find the static ones instead,
# you must set the SFML_STATIC_LIBRARIES variable to TRUE before calling find_package(SFML ...).
#
# If SFML is not installed in a standard path, you can use the SFMLDIR CMake variable
# to tell CMake where SFML is.

# link against dynamic SFML libraries
if(NOT SFML_STATIC_LIBRARIES)
  add_definitions(-DSFML_DYNAMIC)
endif()

# find the SFML include directory
find_path(SFML_INCLUDE_DIR SFML/Config.hpp
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
          ${CMAKE_SOURCE_DIR}/extlibs
          ${SFMLDIR})

# check the version number
set(SFML_VERSION_OK TRUE)
if(SFML_FIND_VERSION AND SFML_INCLUDE_DIR)
    # extract the major and minor version numbers from SFML/Config.hpp
    FILE(READ "${SFML_INCLUDE_DIR}/SFML/Config.hpp" SFML_CONFIG_HPP_CONTENTS)
    STRING(REGEX REPLACE ".*#define SFML_VERSION_MAJOR ([0-9]+).*" "\\1" SFML_VERSION_MAJOR "${SFML_CONFIG_HPP_CONTENTS}")
    STRING(REGEX REPLACE ".*#define SFML_VERSION_MINOR ([0-9]+).*" "\\1" SFML_VERSION_MINOR "${SFML_CONFIG_HPP_CONTENTS}")
    math(EXPR SFML_REQUESTED_VERSION "${SFML_FIND_VERSION_MAJOR} * 10 + ${SFML_FIND_VERSION_MINOR}")

    # if we could extract them, compare with the requested version number
    if (SFML_VERSION_MAJOR)
        # transform version numbers to an integer
        math(EXPR SFML_VERSION "${SFML_VERSION_MAJOR} * 10 + ${SFML_VERSION_MINOR}")

        # compare them
        if(SFML_VERSION LESS SFML_REQUESTED_VERSION)
            set(SFML_VERSION_OK FALSE)
        endif()
    else()
        # SFML version is < 2.0
        if (SFML_REQUESTED_VERSION GREATER 19)
            set(SFML_VERSION_OK FALSE)
        endif()
    endif()
endif()

# find the requested components
set(FIND_SFML_LIB_PATHS ~/Library/Frameworks
                        /Library/Frameworks
                        /usr/local
                        /usr
                        /sw
                        /opt/local
                        /opt/csw
                        /opt
                        ${SFMLDIR})

set(FIND_SFML_PATH_SUFFIXES lib64 lib)
if (WINDOWS AND COMPILER_GCC)
  set(FIND_SFML_PATH_SUFFIXES ${FIND_SFML_PATH_SUFFIXES} libs-mingw)
elseif(MSVC)
  if(SFML_STATIC_LIBRARIES)
    MESSAGE(STATUS "Using static SFML libraries...")
    set(FIND_SFML_PATH_SUFFIXES ${FIND_SFML_PATH_SUFFIXES} libs-msvc-s)
  else()
    MESSAGE(STATUS "Using dynamic SFML libraries...")
    set(FIND_SFML_PATH_SUFFIXES ${FIND_SFML_PATH_SUFFIXES} libs-msvc)
  endif()
elseif(MACOSX)
  set(FIND_SFML_PATH_SUFFIXES ${FIND_SFML_PATH_SUFFIXES} libs-osx)
else()
  set(FIND_SFML_PATH_SUFFIXES ${FIND_SFML_PATH_SUFFIXES} libs-gcc)
endif()
MESSAGE(STATUS "FIND_SFML_LIB_PATHS=${FIND_SFML_LIB_PATHS}")
MESSAGE(STATUS "FIND_SFML_PATH_SUFFIXES=${FIND_SFML_PATH_SUFFIXES}")

#unset(SFML_LIBRARIES_NAMES)
unset(SFML_LIBRARIES_NAMES CACHE)
#unset(SFML_LIBRARIES)
unset(SFML_LIBRARIES CACHE)
#unset(SFML_LIBRARIES_DEBUG)
unset(SFML_LIBRARIES_DEBUG CACHE)

MESSAGE(STATUS "SFML_LIBRARIES=${SFML_LIBRARIES}")
MESSAGE(STATUS "SFML_LIBRARIES_DEBUG=${SFML_LIBRARIES_DEBUG}")

foreach(FIND_SFML_COMPONENT ${SFML_FIND_COMPONENTS})
    string(TOLOWER ${FIND_SFML_COMPONENT} FIND_SFML_COMPONENT_LOWER)
    string(TOUPPER ${FIND_SFML_COMPONENT} FIND_SFML_COMPONENT_UPPER)

    #unset(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY)
    unset(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY CACHE)
    #unset(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG)
    unset(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG CACHE)
    set(FIND_SFML_COMPONENT_VAR SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY)
    set(FIND_SFML_COMPONENT_NAME sfml-${FIND_SFML_COMPONENT_LOWER})

    # release library
    find_library(${FIND_SFML_COMPONENT_VAR}
                 NAMES ${FIND_SFML_COMPONENT_NAME}
                 PATH_SUFFIXES ${FIND_SFML_PATH_SUFFIXES}
                 PATHS ${FIND_SFML_LIB_PATHS})

    # debug library
    find_library(${FIND_SFML_COMPONENT_VAR}_DEBUG
                 NAMES ${FIND_SFML_COMPONENT_NAME}-d
                 PATH_SUFFIXES ${FIND_SFML_PATH_SUFFIXES}
                 PATHS ${FIND_SFML_LIB_PATHS})

    set(SFML_LIBRARIES_NAMES ${SFML_LIBRARIES_NAMES} ${FIND_SFML_COMPONENT_VAR})
    set(SFML_LIBRARIES ${SFML_LIBRARIES} ${${FIND_SFML_COMPONENT_VAR}})
    set(SFML_LIBRARIES_DEBUG ${SFML_LIBRARIES_DEBUG} ${${FIND_SFML_COMPONENT_VAR}_DEBUG})
endforeach()

MESSAGE(STATUS "SFML_LIBRARIES=${SFML_LIBRARIES}")
MESSAGE(STATUS "SFML_LIBRARIES_DEBUG=${SFML_LIBRARIES_DEBUG}")

if(SFML_FIND_REQUIRED AND NOT SFML_VERSION_OK)
    message(SEND_ERROR "Bad SFML version (requested: ${SFML_FIND_VERSION}, found: ${SFML_VERSION_MAJOR}.${SFML_VERSION_MINOR})")
    set(SFML_FOUND FALSE)
else()
    # handle the QUIETLY and REQUIRED arguments and set SFML_FOUND to TRUE if all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(SFML DEFAULT_MSG SFML_INCLUDE_DIR ${SFML_LIBRARIES_NAMES})
    MARK_AS_ADVANCED(SFML_INCLUDE_DIR ${SFML_LIBRARIES_NAMES})
endif()
