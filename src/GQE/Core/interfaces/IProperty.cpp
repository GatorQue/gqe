/**
 * Provides the IProperty interface class for storing IEntity wide values.
 *
 * @file src/GQE/Core/interfaces/IProperty.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#include <GQE/Core/interfaces/IProperty.hpp>

namespace GQE
{
  IProperty::IProperty(std::string theType, const typePropertyID thePropertyID) :
    mType(theType),
    mPropertyID(thePropertyID)
  {
  }

  IProperty::~IProperty()
  {
  }

  IProperty::Type_t* IProperty::GetType(void)
  {
    return &mType;
  }

  const typePropertyID IProperty::GetID(void) const
  {
    return mPropertyID;
  }

  void IProperty::SetType(std::string theType)
  {
    mType = Type_t(theType);
  }
} // namespace GQE

/**
 * Copyright (c) 2010-2012 Jacob Dix
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
