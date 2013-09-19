/**
 * Provides a simple class to hold and compare version information.
 *
 * @file src/GQE/Core/classes/VersionInfo.cpp
 * @author Ryan Lindeman
 * @date 20130202 - Initial Release
 */

#include <sstream>
#include <GQE/Core/classes/VersionInfo.hpp>

namespace GQE
{
  VersionInfo::VersionInfo(Uint8 theMajor, Uint8 theMinor, Uint16 thePatchBuild) :
    mMajor(theMajor),
    mMinor(theMinor),
    mPatchBuild(thePatchBuild)
  {
  }

  VersionInfo::~VersionInfo()
  {
  }

  Uint32 VersionInfo::ToInteger(void) const
  {
    // Compute version as a Uint32 integer
    Uint32 anResult = (mMajor << 24) | (mMinor << 16) | mPatchBuild;

    // Return anResult computed above
    return anResult;
  }

  std::string VersionInfo::ToString(void) const
  {
    // Use StringStream class to convert values to a string
    std::stringstream anResult;

    // Add major version
    anResult << (Uint32)mMajor << ".";
    anResult << (Uint32)mMinor << ".";
    anResult << (Uint16)mPatchBuild;

    // Return anResult as a string created above
    return anResult.str();
  }
} // namespace GQE

/**
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

