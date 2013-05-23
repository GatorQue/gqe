/**
 * Provides the Reference count class for handling reference counts in smart pointers, etc
 *
 * @file include/GQE/Core/interfaces/ReferenceCount.hpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#ifndef REFERENCE_COUNT_HPP_INCLUDED
#define REFERENCE_COUNT_HPP_INCLUDED
#include <GQE/Core/Core_types.hpp>
namespace GQE
{
  class GQE_API ReferenceCount
  {
      private:
      int mCount; // Reference count

      public:
      ReferenceCount();

      void AddRef();

      int Release();
  };
}
#endif