/**
 * Defines the GQE engine version numbers and provides OS identification and
 * other OS and OS independent values for all GQE engines.
 *
 * @file include/GQE/Config.hpp
 * @author Ryan Lindeman
 * @date 20110127 - Initial Release
 * @date 20110131 - Added define documentation
 * @date 20110218 - Next minor release of GQE
 * @date 20110318 - Next minor release of GQE
 * @date 20110611 - Next minor release of GQE
 * @date 20110618 - Next minor release of GQE
 * @date 20110627 - Next minor release of GQE
 * @date 20110831 - Next minor release of GQE
 * @date 20110906 - Remove use of GQE_STRICMP
 * @date 20120211 - Next minor release of GQE
 * @date 20120211 - Next minor release of GQE
 * @date 20120330 - Next minor release of GQE
 */
#ifndef GQE_CONFIG_HPP
#define GQE_CONFIG_HPP


////////////////////////////////////////////////////////////
// Define the GQE version
////////////////////////////////////////////////////////////
/// Define the GQE Major version number
#define GQE_VERSION_MAJOR 0
/// Define the GQE Minor version number
#define GQE_VERSION_MINOR 22


////////////////////////////////////////////////////////////
// Identify the operating system
////////////////////////////////////////////////////////////
#if defined(_WIN32) || defined(__WIN32__)

/// Define a GQE Windows OS flag
#define GQE_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#elif defined(linux) || defined(__linux)

/// Define a GQE Linux OS flag
#define GQE_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)

/// Define a GQE Mac OS flag
#define GQE_MACOS

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

/// Define a GQE FreeBSD OS flag
#define GQE_FREEBSD

#else

// Unsupported system
#error This operating system is not supported by GQE library

#endif


////////////////////////////////////////////////////////////
// Identify the endianess
////////////////////////////////////////////////////////////
#if defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || (defined(__MIPS__) && defined(__MISPEB__)) || \
  defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || defined(__sparc__) || defined(__hppa__)

/// Define a GQE Big endian flag
#define GQE_ENDIAN_BIG

#else

/// Define a GQE Little endian flag
#define GQE_ENDIAN_LITTLE

#endif


////////////////////////////////////////////////////////////
// Define a portable debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG)

/// Define a GQE Debug build flag
#define GQE_DEBUG

#endif


////////////////////////////////////////////////////////////
// Define portable import / export macros
////////////////////////////////////////////////////////////
#if defined(GQE_WINDOWS) && !defined(GQE_STATIC)

#ifdef GQE_EXPORTS

// From DLL side, we must export
/// Define the GQE DLL API for exporting classes and methods
#define GQE_API __declspec(dllexport)

#else

// From client application side, we must import
/// Define the GQE DLL API for importing classes and methods
#define GQE_API __declspec(dllimport)

#endif

// For Visual C++ compilers, we also need to turn off this annoying C4251 warning.
// You can read lots ot different things about it, but the point is the code will
// just work fine, and so the simplest way to get rid of this warning is to disable it
#ifdef _MSC_VER

#pragma warning(disable : 4251)

#endif

#else

// Other platforms and static build don't need these export macros
/// Define the GQE DLL API for non DLL OS platforms as nothing
#define GQE_API

#endif


////////////////////////////////////////////////////////////
// Define portable fixed-size types
////////////////////////////////////////////////////////////
#include <stdint.h>
namespace GQE
{
  // All "common" platforms use the same size for char, short and int
  // (basically there are 3 types for 3 sizes, so no other match is possible),
  // we can use them without doing any kind of check

  // 8 bits integer types
  typedef signed   char Int8;     ///< Standard 8 bit signed integer
  typedef unsigned char Uint8;    ///< Standard 8 bit unsigned integer

  // 16 bits integer types
  typedef signed   short Int16;   ///< Standard 16 bit signed integer
  typedef unsigned short Uint16;  ///< Standard 16 bit unsigned integer

  // 32 bits integer types
  typedef signed   int Int32;     ///< Standard 32 bit signed integer
  typedef unsigned int Uint32;    ///< Standard 32 bit unsigned integer

  // 64 bits integer types
#if defined(_MSC_VER)
  typedef signed   __int64 Int64;  ///< Standard 64 bit signed integer
  typedef unsigned __int64 Uint64; ///< Standard 64 bit unsigned integer
#else
  typedef int64_t          Int64;  ///< Standard 64 bit signed integer
  typedef uint64_t         Uint64; ///< Standard 64 bit unsigned integer
#endif

} // namespace GQE


#endif // GQE_CONFIG_HPP
