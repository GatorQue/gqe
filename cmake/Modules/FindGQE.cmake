# Locate the GQE library
#
# This module defines
#  GQE_FOUND, if false, do not try to link to GQE
#  GQE_XXX_LIBRARY, library corresponding to the XXX component (release)
#  GQE_XXX_LIBRARY_DEBUG, library corresponding to the XXX component (debug)
#  GQE_LIBRARIES, list containing all the libraries corresponding to the requested components
#  GQE_INCLUDE_DIR, where to find GQE/Config.hpp
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
          ${GQEDIR})

# check the version number
set(GQE_VERSION_OK TRUE)
if(GQE_FIND_VERSION AND GQE_INCLUDE_DIR)
    # extract the major and minor version numbers from GQE/Config.hpp
    FILE(READ "${GQE_INCLUDE_DIR}/GQE/Config.hpp" GQE_CONFIG_HPP_CONTENTS)
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
        endif()
    endif()
endif()

# find the requested components
set(FIND_GQE_LIB_PATHS ~/Library/Frameworks
                        /Library/Frameworks
                        /usr/local
                        /usr
                        /sw
                        /opt/local
                        /opt/csw
                        /opt
                        ${GQEDIR}
)

set(FIND_SFML_PATH_SUFFIXES lib64 lib)
if (WINDOWS AND COMPILER_GCC)
	set(FIND_GQE_PATH_SUFFIXES ${FIND_GQE_PATH_SUFFIXES} libs-mingw)
elseif(MSVC)
	set(FIND_GQE_PATH_SUFFIXES ${FIND_GQE_PATH_SUFFIXES} libs-msvc)
elseif(MACOSX)
	set(FIND_GQE_PATH_SUFFIXES ${FIND_GQE_PATH_SUFFIXES} libs-osx)
else()
	set(FIND_GQE_PATH_SUFFIXES ${FIND_GQE_PATH_SUFFIXES} libs-gcc)
endif()
						
foreach(FIND_GQE_COMPONENT ${GQE_FIND_COMPONENTS})
    string(TOLOWER ${FIND_GQE_COMPONENT} FIND_GQE_COMPONENT_LOWER)
    string(TOUPPER ${FIND_GQE_COMPONENT} FIND_GQE_COMPONENT_UPPER)
    set(FIND_GQE_COMPONENT_VAR GQE_${FIND_GQE_COMPONENT_UPPER}_LIBRARY)
    set(FIND_GQE_COMPONENT_NAME gqe-${FIND_GQE_COMPONENT_LOWER}${FIND_GQE_LIB_SUFFIX})

    # release library
    find_library(${FIND_GQE_COMPONENT_VAR}
                 NAMES ${FIND_GQE_COMPONENT_NAME}
                 PATH_SUFFIXES ${FIND_GQE_PATH_SUFFIXES}
                 PATHS ${FIND_GQE_LIB_PATHS})

    # debug library
    find_library(${FIND_GQE_COMPONENT_VAR}_DEBUG
                 NAMES ${FIND_GQE_COMPONENT_NAME}-d
                 PATH_SUFFIXES ${FIND_GQE_PATH_SUFFIXES}
                 PATHS ${FIND_GQE_LIB_PATHS})

    set(GQE_LIBRARIES_NAMES ${GQE_LIBRARIES_NAMES} ${FIND_GQE_COMPONENT_VAR})
    set(GQE_LIBRARIES ${GQE_LIBRARIES} ${${FIND_GQE_COMPONENT_VAR}})
endforeach()

if(GQE_FIND_REQUIRED AND NOT GQE_VERSION_OK)
    message(SEND_ERROR "Bad GQE version (requested: ${GQE_FIND_VERSION}, found: ${GQE_VERSION_MAJOR}.${GQE_VERSION_MINOR})")
    set(GQE_FOUND FALSE)
else()
    # handle the QUIETLY and REQUIRED arguments and set GQE_FOUND to TRUE if all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(GQE DEFAULT_MSG GQE_INCLUDE_DIR ${GQE_LIBRARIES_NAMES})
    MARK_AS_ADVANCED(GQE_INCLUDE_DIR ${GQE_LIBRARIES_NAMES})
endif()
