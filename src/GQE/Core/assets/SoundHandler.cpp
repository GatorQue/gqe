/**
 * Provides the SoundHandler class used by the AssetManager to manage all
 * sf::SoundBuffer assets for the application.
 *
 * @file src/GQE/Core/assets/SoundHandler.cpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */
 
#include <GQE/Core/assets/SoundHandler.hpp>
#include <GQE/Core/interfaces/IApp.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
 
namespace GQE
{
  /// Default Asset Handler ID for this Asset Handler
  const char* SoundHandler::DEFAULT_ID = "sounds";

  SoundHandler::SoundHandler() :
    IAssetHandler(SoundHandler::DEFAULT_ID)
  {
    ILOG() << "SoundHandler::ctor()" << std::endl;
  }
 
  SoundHandler::~SoundHandler()
  {
    ILOG() << "SoundHandler::dtor()" << std::endl;
  }
 
  void* SoundHandler::AcquireAsset(const typeAssetID theAssetID)
  {
    ILOG() << "SoundHandler(" << GetID() << "):AcquireAsset("
      << theAssetID << ") Creating asset" << std::endl;
    return new sf::SoundBuffer();
  }
      
  void* SoundHandler::GetDummyAsset(void)
  {
    ILOG() << "SoundHandler(" << GetID()
      << "):GetDummyAsset() Returning Dummy Asset." << std::endl;
    return &mDummyAsset;
  }

  bool SoundHandler::LoadAsset(const typeAssetID theAssetID, void* theAsset)
  {
    // Default to NOT loaded
    bool anResult = false;
    sf::SoundBuffer* anAsset = static_cast<sf::SoundBuffer*>(theAsset);

    if(NULL != anAsset)
    {
      // Get filename for this asset from AppSettings file in App
      std::string anFilename = theAssetID; // gApp->mAppSettings.GetString("Images", theAssetID);

      ILOG() << "SoundHandler::LoadAsset(" << theAssetID
        << ") Loading asset from file(" << anFilename << ") ..." << std::endl;
 
      // Attempt to load the asset from a file
      anResult = anAsset->LoadFromFile(anFilename);
 
      // If the asset did not load successfully, log a fatal error
      if(false == anResult)
      {
        FLOG(StatusAppMissingAsset) << "SoundHandler::LoadAsset(" << theAssetID
          << ") Unable to load from file(" << anFilename << ")!" << std::endl;
      }
    }
    else
    {
      // Log fatal error if our static cast failed!
      FLOG(StatusAppMissingAsset) << "SoundHandler::LoadAsset(" << theAssetID
        << ") Bad static cast!" << std::endl;
    }

    // Return anResult determined above or false for NOT loaded
    return anResult;
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
