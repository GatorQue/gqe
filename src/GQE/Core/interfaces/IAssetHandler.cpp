/**
 * Provides the IAssetHandler class in the GQE namespace which is
 * responsible for providing management of the IAsset derived classes.
 *
 * @file src/GQE/Core/interfaces/IAssetHandler.cpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */

#include <assert.h>
#include <GQE/Core/interfaces/IAssetHandler.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
  IAssetHandler::IAssetHandler(const typeAssetHandlerID theAssetHandlerID) :
    mAssetHandlerID(theAssetHandlerID)
  {
    ILOG() << "IAssetHandler::ctor(" << mAssetHandlerID << ")" << std::endl;
  }

  IAssetHandler::~IAssetHandler()
  {
    ILOG() << "IAssetHandler::dtor(" << mAssetHandlerID << ")" << std::endl;
  }

  const typeAssetHandlerID IAssetHandler::GetID(void) const
  {
    return mAssetHandlerID;
  }
} // namespace GQE

/**
 * Copyright (c) 2010-2012 Ryan Lindeman
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
