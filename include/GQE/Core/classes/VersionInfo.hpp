/**
 * Provides a simple class to hold and compare version information.
 *
 * @file include/GQE/Core/classes/VersionInfo.hpp
 * @author Ryan Lindeman
 * @date 20130202 - Initial Release
 */
#ifndef VERSION_INFO_HPP_INCLUDED
#define VERSION_INFO_HPP_INCLUDED

#include <GQE/Config.hpp>

namespace GQE
{
  /// Provides the VersionInfo class for handling version operations
  class GQE_API VersionInfo
  {
    public:
      // Variables
      ////////////////////////////////////////////////////////////////////////
      /// Major version
      Uint8 mMajor;
      /// Minor version
      Uint8 mMinor;
      /// Patch/Build version
      Uint16 mPatchBuild;

      /**
       * VersionInfo default constructor
       * @param[in] theMajor version number
       * @param[in] theMinor version number
       * @param[in] thePatchBuild version number
       */
      VersionInfo(Uint8 theMajor = 1, Uint8 theMinor = 0, Uint16 thePatchBuild = 0);

      /**
       * VersionInfo deconstructor
       */
      virtual ~VersionInfo();

      /**
       * ToInteger will return the version information as a single Uint32
       * integer which can be used for comparing two versions together.
       * @returns Uint32 format of the version information
       */
      Uint32 ToInteger(void) const;

      /**
       * ToString will return the version information as a string for
       * outputing to a log or file.
       * @returns string version as major.minor.patch
       */
      std::string ToString(void) const;
    protected:

    private:
  }; // VersionInfo class

  /**
   * operator== as a non-member method for comparing two VersionInfo objects
   * to each other.
   * @return true if they match, false otherwise
   */
  inline bool operator==(const VersionInfo& theLeft, const VersionInfo& theRight)
  {
    // Compare integer version of left and right
    bool anResult = theLeft.ToInteger() == theRight.ToInteger();

    // Return anResult produced above
    return anResult;
  }

  /**
   * operator!= as a non-member method for comparing two VersionInfo objects
   * to each other.
   * @return true if they don't match, false otherwise
   */
  inline bool operator!=(const VersionInfo& theLeft, const VersionInfo& theRight)
  {
    return !operator==(theLeft,theRight);
  }

  /**
   * operator< as a non-member method for comparing two VersionInfo objects to
   * each other.
   * @return true if left is less than right, false otherwise
   */
  inline bool operator< (const VersionInfo& theLeft, const VersionInfo& theRight)
  {
    // Determine if left is smaller than right
    bool anResult = theLeft.ToInteger() < theRight.ToInteger();

    // Return anResult computed above
    return anResult;
  }

  /**
   * operator> as a non-member method for comparing two VersionInfo objects to
   * each other.
   * @return true if right is less than left, false otherwise
   */
  inline bool operator> (const VersionInfo& theLeft, const VersionInfo& theRight)
  {
    return  operator< (theRight,theLeft);
  }

  /**
   * operator<= as a non-member method for comparing two VersionInfo objects
   * to each other.
   * @return true if left is less than or equal to right, false otherwise
   */
  inline bool operator<=(const VersionInfo& theLeft, const VersionInfo& theRight)
  {
    return !operator> (theLeft,theRight);
  }

  /**
   * operator<= as a non-member method for comparing two VersionInfo objects
   * to each other.
   * @return true if right is less than or equal to left, false otherwise
   */
  inline bool operator>=(const VersionInfo& theLeft, const VersionInfo& theRight)
  {
    return !operator< (theLeft,theRight);
  }
} // namespace GQE
#endif

/**
 * @class GQE::VersionInfo
 * @ingroup Core
 * The VersionInfo class is responsible for providing simple version handling
 * for any class that needs it. It easily allows for comparing two VersionInfo
 * classes to each other or comparing a version number in a string to a
 * VersionInfo object.
 *
 * Copyright (c) 2010-2013 Ryan Lindeman
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

