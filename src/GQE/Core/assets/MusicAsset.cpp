/**
 * Provides the Music Asset type used by the AssetManager in the GQE namespace
 * which is responsible for providing the Asset management facilities for the
 * App base class used in the GQE core library.
 *
 * @file src/GQE/Core/assets/MusicAsset.cpp
 * @author Ryan Lindeman
 * @date 20100724 - Initial Release
 * @date 20110127 - Moved to GQE Core library and src directory
 */
 
#include <assert.h>
#include <stddef.h>
#include "GQE/Core/assets/MusicAsset.hpp"
#include "GQE/Core/classes/App.hpp"
 
namespace GQE
{
  MusicAsset::MusicAsset(std::string theFilename, AssetLoadingStyle theStyle) :
    TAsset<sf::Music>(theFilename, theStyle)
  {
  }
 
  MusicAsset::~MusicAsset()
  {
    UnloadAsset();
  }
 
  void MusicAsset::LoadAsset(void)
  {
    // Only load the asset once if possible!
    if(false == mLoaded)
    {
      // Make sure memory is not already allocated
      assert(NULL == mAsset && "MusicAsset::LoadAsset() memory already allocated!");
 
      // Create the asset
      mAsset = new(std::nothrow) sf::Music;
      assert(NULL != mAsset && "MusicAsset::LoadAsset() unable to allocate memory");
 
      // Output to log file
      if(NULL != mApp)
      {
        mApp->mLog << "MusicAsset::LoadAsset() loading asset with filename=" << mFilename << std::endl;
      }
 
      // Attempt to load the asset from a file
      mLoaded = mAsset->OpenFromFile(mFilename);
 
      // If the asset did not load successfully, delete the memory
      if(false == mLoaded)
      {
        // Output to log file
        if(NULL != mApp)
        {
          mApp->mLog << "MusicAsset::LoadAsset() asset with filename=" << mFilename << " is missing" << std::endl;
          mApp->Quit(StatusAppMissingAsset);
        }
      }
    }
  }
 
  void MusicAsset::UnloadAsset(void)
  {
    // If the music asset is currently loaded, make sure it is stopped
    if(true == mLoaded && NULL != mAsset)
    {
       // Always stop music from playing before removing from memory
       mAsset->Stop();
    }
 
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
