/**
 * Provides the Config Asset type used by the AssetManager in the GQE namespace
 * which is responsible for providing the Asset management facilities for the
 * App base class used in the GQE core library.
 *
 * @file GQE/Core/assets/ConfigAsset.cpp
 * @author Ryan Lindeman
 * @date 20110218 - Initial Release
 * @date 20110218 - Change to system include style
 * @date 20110611 - Convert logging to new Log macros
 * @date 20110627 - Removed extra ; from namespace
 * @date 20120512 - Use new RAII Asset and Asset Handler management style
 * @date 20120514 - Add default constructor for missing Asset ID at construction
 */

#include <assert.h>
#include <stddef.h>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/assets/ConfigAsset.hpp>
#include <GQE/Core/assets/ConfigHandler.hpp>
#include <GQE/Core/interfaces/IApp.hpp>

namespace GQE
{
  ConfigAsset::ConfigAsset() :
    TAsset<ConfigReader>(
      IApp::GetApp()->mAssetManager.GetHandler(ConfigHandler::DEFAULT_ID))
  {
  }

  ConfigAsset::ConfigAsset(std::string theFilename, bool theLoadFlag) :
    TAsset<ConfigReader>(
      IApp::GetApp()->mAssetManager.GetHandler(ConfigHandler::DEFAULT_ID),
      theFilename, theLoadFlag)
  {
  }

  ConfigAsset::~ConfigAsset()
  {
  }

} // namespace GQE

/**
 * Copyright (c) 2010-2011 Ryan Lindeman
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
