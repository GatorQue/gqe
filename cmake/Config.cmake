
# detect the OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX 1)
else()
    message(WARNING "Unsupported operating system")
    return()
endif()

# detect the architecture
# note: this test won't work for cross-compilation
include(CheckTypeSize)
check_type_size(void* SIZEOF_VOID_PTR)
if(${SIZEOF_VOID_PTR} MATCHES "^4$")
    set(ARCH_32BITS 1)
else()
    set(ARCH_64BITS 1)
endif()

# detect the compiler and its version
if(CMAKE_COMPILER_IS_GNUCXX)
    set(COMPILER_GCC 1)
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpversion" OUTPUT_VARIABLE GCC_VERSION_OUTPUT)
    string(REGEX REPLACE "([0-9]+\\.[0-9]+).*" "\\1" GCC_VERSION "${GCC_VERSION_OUTPUT}")
elseif(MSVC_VERSION EQUAL 1400)
    set(COMPILER_MSVC 2005)
elseif(MSVC_VERSION EQUAL 1500)
    set(COMPILER_MSVC 2008)
elseif(MSVC_VERSION EQUAL 1600)
    set(COMPILER_MSVC 2010)
else()
    message(WARNING "Unsupported compiler")
    return()
endif()

# let CMake know about our additional libraries paths (on Windows and OS X)
if(WINDOWS)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers")
    if(COMPILER_GCC)
        set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-mingw")

        # require proper c++
        ADD_DEFINITIONS("-Wall -ansi -pedantic")
    elseif(COMPILER_MSVC)
        if(ARCH_32BITS)
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/extlibs/libs-msvc/x86")
        else()
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/extlibs/libs-msvc/x64")
        endif()

        # remove SL security warnings with Visual C++
        add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    endif()
elseif(LINUX)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-gcc")

    # require proper c++
    ADD_DEFINITIONS("-Wall -ansi -pedantic")
elseif(MACOSX)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-osx/lib/")

    # require proper c++
    ADD_DEFINITIONS("-Wall -ansi -pedantic")
endif()

# offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if(WINDOWS)
    set(INSTALL_DATA_DIR . CACHE PATH "Installation directory for data files")
else()
    set(INSTALL_DATA_DIR share CACHE PATH "Installation directory for data files")
endif(WINDOWS)

# make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE DATA)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()
