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
 */

#include <assert.h>
#include <stddef.h>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/assets/ConfigAsset.hpp>
#include <GQE/Core/classes/App.hpp>

namespace GQE
{
  ConfigAsset::ConfigAsset(std::string theFilename, AssetLoadingStyle theStyle) :
    TAsset<ConfigReader>(theFilename, theStyle)
  {
  }

  ConfigAsset::~ConfigAsset()
  {
    UnloadAsset();
  }

  void ConfigAsset::LoadAsset(void)
  {
    // Only load the asset once if possible!
    if(false == mLoaded)
    {
      // Make sure memory is not already allocated
      assert(NULL == mAsset && "ConfigAsset::LoadAsset() memory already allocated!");
    
      // Create the asset
      mAsset = new (std::nothrow) ConfigReader;
      assert(NULL != mAsset && "ConfigAsset::LoadAsset() unable to allocate memory");

      ILOG() << "ConfigAsset::LoadAsset(" << mFilename << ") loading..." << std::endl;

      // Attempt to load the asset from a file
      mLoaded = mAsset->Read(mFilename);

      // Register the App pointer with this asset
      mAsset->RegisterApp(mApp);

      // If the asset did not load successfully, delete the memory
      if(false == mLoaded)
      {
        FLOG(StatusAppMissingAsset) << "ConfigAsset::LoadAsset(" << mFilename << ") is missing" << std::endl;
      }
    }
  }

  void ConfigAsset::UnloadAsset(void)
  {
    // Delete the asset, forcing it to be removed from memory
    delete mAsset;
    mAsset = NULL;
    mLoaded = false;
  }

}; // namespace GQE

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
