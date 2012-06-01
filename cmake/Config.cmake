
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
  # Add some include files missing in MSVC 2005
  include_directories(${PROJECT_SOURCE_DIR}/extlibs/msvc)
elseif(MSVC_VERSION EQUAL 1500)
  set(COMPILER_MSVC 2008)
  # Add some include files missing in MSVC 2008
  include_directories(${PROJECT_SOURCE_DIR}/extlibs/msvc)
elseif(MSVC_VERSION EQUAL 1600)
  set(COMPILER_MSVC 2010)
else()
  message(WARNING "Unsupported compiler")
  return()
endif()

# define SFML_STATIC if the SFML_STATIC_LIBRARIES type is set to 'On'
if(SFML_STATIC_LIBRARIES)
  add_definitions(-DSFML_STATIC)
else()
  # This flag is needed for SFML 1.6 to be able to import SFML values correctly
  add_definitions(-DSFML_DYNAMIC)
endif(SFML_STATIC_LIBRARIES)

# define GQE_STATIC if the build type is not set to 'shared'
if(NOT BUILD_SHARED_LIBS)
  add_definitions(-DGQE_STATIC)
endif()

# let CMake know about our additional libraries paths (on Windows and OS X)
include_directories(${PROJECT_SOURCE_DIR}/extlibs/headers)
if(WINDOWS)
  if(COMPILER_GCC)
    # Specify where to find DLL and LIB files according to 32bit or 64bit compiling
    if(ARCH_32BITS)
      set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/bin-mingw/x86")
      set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-mingw/x86")
    else()
      set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/bin-mingw/x64")
      set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-mingw/x64")
    endif()

    # require proper c++
    ADD_DEFINITIONS("-Wall -pedantic -lstdc++")
  elseif(COMPILER_MSVC)
    # Specify where to find DLL and LIB files according to 32bit or 64bit compiling
    if(ARCH_32BITS)
      set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/bin-msvc/x86")
      set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-msvc/x86")
    else()
      set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/bin-msvc/x64")
      set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-msvc/x64")
    endif()

    # remove SL security warnings with Visual C++
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    
    # for VC++, we can apply it globally by modifying the compiler flags
    if(BUILD_STATIC_STD_LIBS)
      foreach(flag
              CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
              CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag} MATCHES "/MD")
          string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
        endif()
      endforeach()
    endif()
  endif()
elseif(LINUX)
  set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-gcc")

  # require proper c++
  ADD_DEFINITIONS("-Wall -pedantic")
elseif(MACOSX)
  set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-osx/lib/")

  # require proper c++
  ADD_DEFINITIONS("-Wall -pedantic")
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
