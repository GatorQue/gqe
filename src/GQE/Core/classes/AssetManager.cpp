/**
 * Provides the AssetManager class in the GQE namespace which is responsible
 * for managing all IAssetHandler classes and providing application wide
 * Asset management facilities.
 *
 * @file src/GQE/Core/classes/AssetManager.cpp
 * @author Ryan Lindeman
 * @date 20100723 - Initial Release
 * @date 20110110 - Added ability to get sf::Sound from SoundAsset
 * @date 20110127 - Moved to GQE Core library and src directory
 * @date 20110128 - Fixed erase call in the DeleteXYZ methods.
 * @date 20110218 - Added ConfigAsset to AssetManager
 * @date 20110218 - Change to system include style
 * @date 20110611 - Convert logging to new Log macros
 * @date 20110627 - Removed extra ; from namespace
 * @date 20110721 - Remove * from GetAsset() calls since it now returns TYPE&
 * @date 20110831 - Support new SFML2 snapshot changes
 * @date 20120322 - Support new SFML2 snapshot changes
 * @date 20120503 - Redo AssetManager to be more flexible and use RAII techniques
 */

#include <GQE/Core/classes/AssetManager.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
  AssetManager::AssetManager()
  {
    ILOGM("AssetManager::ctor()");
  }

  AssetManager::~AssetManager()
  {
    ILOGM("AssetManager::dtor()");

    // Iterator to use while deleting all assets
    std::map<const typeAssetHandlerID, IAssetHandler*>::iterator iter;

    // Loop through each asset handler and remove each one
    iter = mHandlers.begin();
    while(iter != mHandlers.end())
    {
      IAssetHandler* anAssetHandler = iter->second;

      // Remove this Asset Handler from our map
      mHandlers.erase(iter++);

      // Delete the Asset Handler
      delete anAssetHandler;
    }
  }

  IAssetHandler& AssetManager::GetHandler(const typeAssetHandlerID theAssetHandlerID)
  {
    // The IAssetHandler derived class that will be returned
    IAssetHandler* result = NULL;

    // Iterator to the asset if found
    std::map<const typeAssetHandlerID, IAssetHandler*>::iterator iter;

    // Try to find the asset using theAssetID as the key
    iter = mHandlers.find(theAssetHandlerID);

    // Found asset? increment the count and return the reference
    if(iter != mHandlers.end())
    {
      // Return the IAsset address found
      result = iter->second;
    }

    // If result is NULL by this point then return the Dummy Handler instead
    if(NULL == result)
    {
      // Return the DummyHandler pointer instead
      result = &mDummyHandler;
    }

    // Return the addres to some IAssetHandler class or the DummyHandler instead
    return *result;
  }

  void AssetManager::RegisterHandler(IAssetHandler* theAssetHandler)
  {
    // Iterator to the asset if found
    std::map<const typeAssetHandlerID, IAssetHandler*>::iterator iter;

    // Make sure the Handler provided is not NULL
    if(NULL != theAssetHandler)
    {
      // Try to find the asset using theAssetID as the key
      iter = mHandlers.find(theAssetHandler->GetID());

      // Found asset? increment the count and return the reference
      if(iter == mHandlers.end())
      {
        // Store the provided IAssetHandler derived class for futurn reference
        mHandlers.insert(
          std::pair<const typeAssetHandlerID, IAssetHandler*>(
          theAssetHandler->GetID(), theAssetHandler));
      }
      else
      {
        // Log an error for trying to register two handlers with the same Handler ID.
        ELOG() << "AssetManager:RegisterHandler(" << theAssetHandler->GetID()
          << ") Handler already registered for this ID!" << std::endl;
      }
    }
    else
    {
      // Log an error for trying to register a NULL handler
      FLOG(StatusError)
        << "AssetManager:RegisterHandler() Handler pointer provided was NULL!"
        << std::endl;
    }
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
