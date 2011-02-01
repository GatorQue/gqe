/**
 * Defines the GQE engine version numbers and provides OS identification and
 * other OS and OS independent values for all GQE engines.
 *
 * @file include/GQE/Config.hpp
 * @author Ryan Lindeman
 * @date 20110127 - Initial Release
 * @date 20110131 - Added define documentation
 */
#ifndef GQE_CONFIG_HPP
#define GQE_CONFIG_HPP


////////////////////////////////////////////////////////////
// Define the GQE version
////////////////////////////////////////////////////////////
/// Define the GQE Major version number
#define GQE_VERSION_MAJOR 0
/// Define the GQE Minor version number
#define GQE_VERSION_MINOR 4


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
// Define portable string compare function
////////////////////////////////////////////////////////////
// The following defines help with OS/Compiler specific calls
#ifdef _MSC_VER
  #include <string.h>
  /// Define the GQE compiler independent string compare method
  #define GQE_STRICMP _stricmp
#else
  #include <string.h>
  /// Define the GQE compiler independent string compare method
  #define GQE_STRICMP strcasecmp
#endif

////////////////////////////////////////////////////////////
// Define portable fixed-size types
////////////////////////////////////////////////////////////
#include <climits>

namespace GQE
{
    // 8 bits integer types
    #if UCHAR_MAX == 0xFF
        typedef signed   char Int8;
        typedef unsigned char Uint8;
    #else
        #error No 8 bits integer type for this platform
    #endif

    // 16 bits integer types
    #if USHRT_MAX == 0xFFFF
        typedef signed   short Int16;
        typedef unsigned short Uint16;
    #elif UINT_MAX == 0xFFFF
        typedef signed   int Int16;
        typedef unsigned int Uint16;
    #elif ULONG_MAX == 0xFFFF
        typedef signed   long Int16;
        typedef unsigned long Uint16;
    #else
        #error No 16 bits integer type for this platform
    #endif

    // 32 bits integer types
    #if USHRT_MAX == 0xFFFFFFFF
        typedef signed   short Int32;
        typedef unsigned short Uint32;
    #elif UINT_MAX == 0xFFFFFFFF
        typedef signed   int Int32;
        typedef unsigned int Uint32;
    #elif ULONG_MAX == 0xFFFFFFFF
        typedef signed   long Int32;
        typedef unsigned long Uint32;
    #else
        #error No 32 bits integer type for this platform
    #endif

} // namespace GQE


#endif // GQE_CONFIG_HPP
