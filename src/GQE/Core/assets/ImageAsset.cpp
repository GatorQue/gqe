/**
 * Provides the Image Asset type used by the AssetManager in the GQE namespace
 * which is responsible for providing the Asset management facilities for the
 * App base class used in the GQE core library.
 *
 * @file src/GQE/Core/assets/ImageAsset.cpp
 * @author Ryan Lindeman
 * @date 20100724 - Initial Release
 * @date 20110127 - Moved to GQE Core library and src directory
 * @date 20110218 - Change to system include style
 * @date 20110611 - Convert logging to new Log macros
 * @date 20110627 - Removed extra ; from namespace
 * @date 20110831 - Support new SFML2 snapshot changes
 */
 
#include <assert.h>
#include <stddef.h>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/assets/ImageAsset.hpp>
#include <GQE/Core/classes/App.hpp>
 
namespace GQE
{
  ImageAsset::ImageAsset(std::string theFilename, AssetLoadingStyle theStyle) :
#if (SFML_VERSION_MAJOR < 2)
    TAsset<sf::Image>(theFilename, theStyle)
#else
    TAsset<sf::Texture>(theFilename, theStyle)
#endif
  {
  }
 
  ImageAsset::~ImageAsset()
  {
    UnloadAsset();
  }
 
  void ImageAsset::LoadAsset(void)
  {
    // Only load the asset once if possible!
    if(false == mLoaded)
    {
      // Make sure memory is not already allocated
      assert(NULL == mAsset && "ImageAsset::LoadAsset() memory already allocated!");
 
      // Create the asset
#if (SFML_VERSION_MAJOR < 2)
      mAsset = new(std::nothrow) sf::Image;
#else
      mAsset = new(std::nothrow) sf::Texture;
#endif
      assert(NULL != mAsset && "ImageAsset::LoadAsset() unable to allocate memory");
 
      ILOG() << "ImageAsset::LoadAsset(" << mFilename << ") loading..." << std::endl;
 
      // Attempt to load the asset from a file
      mLoaded = mAsset->LoadFromFile(mFilename);
 
      // If the asset did not load successfully, delete the memory
      if(false == mLoaded)
      {
        FLOG(StatusAppMissingAsset) << "ImageAsset::LoadAsset(" << mFilename << ") is missing" << std::endl;
      }
    }
  }
 
  void ImageAsset::UnloadAsset(void)
  {
    // Delete the asset, forcing it to be removed from memory
    delete mAsset;
    mAsset = NULL;
    mLoaded = false;
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
