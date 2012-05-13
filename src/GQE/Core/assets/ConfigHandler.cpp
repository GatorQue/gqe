/**
 * Provides the ConfigHandler class used by the AssetManager to manage all
 * ConfigReader assets for the application.
 *
 * @file src/GQE/Core/assets/ConfigHandler.cpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */
 
#include <GQE/Core/assets/ConfigHandler.hpp>
#include <GQE/Core/interfaces/IApp.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
 
namespace GQE
{
  /// Default Asset Handler ID for this Asset Handler
  const char* ConfigHandler::DEFAULT_ID = "configs";

  ConfigHandler::ConfigHandler() :
    IAssetHandler(ConfigHandler::DEFAULT_ID)
  {
    ILOG() << "ConfigHandler::ctor()" << std::endl;
  }
 
  ConfigHandler::~ConfigHandler()
  {
    ILOG() << "ConfigHandler::dtor()" << std::endl;
  }
 
  void* ConfigHandler::AcquireAsset(const typeAssetID theAssetID)
  {
    ILOG() << "ConfigHandler(" << GetID() << "):AcquireAsset("
      << theAssetID << ") Creating asset" << std::endl;
    return new ConfigReader();
  }
      
  void* ConfigHandler::GetDummyAsset(void)
  {
    ILOG() << "ConfigHandler(" << GetID()
      << "):GetDummyAsset() Returning Dummy Asset." << std::endl;
    return &mDummyAsset;
  }

  bool ConfigHandler::LoadAsset(const typeAssetID theAssetID, void* theAsset)
  {
    // Default to NOT loaded
    bool anResult = false;
    ConfigReader* anAsset = static_cast<ConfigReader*>(theAsset);

    if(NULL != anAsset)
    {
      // Get filename for this asset from AppSettings file in App
      std::string anFilename = theAssetID; // gApp->mAppSettings.GetString("Images", theAssetID);

      ILOG() << "ConfigHandler::LoadAsset(" << theAssetID
        << ") Loading asset from file(" << anFilename << ") ..." << std::endl;
 
      // Attempt to load the asset from a file
      anResult = anAsset->LoadFromFile(anFilename);
 
      // If the asset did not load successfully, log a fatal error
      if(false == anResult)
      {
        FLOG(StatusAppMissingAsset) << "ConfigHandler::LoadAsset(" << theAssetID
          << ") Unable to load from file(" << anFilename << ")!" << std::endl;
      }
    }
    else
    {
      // Log fatal error if our static cast failed!
      FLOG(StatusAppMissingAsset) << "ConfigHandler::LoadAsset(" << theAssetID
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
