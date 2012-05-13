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

    // Iterator to use while deleting all assets
    std::map<const typeAssetID, typeAssetData>::iterator iter;

    // Loop through each asset and try to remove each one
    iter = mAssets.begin();
    while(iter != mAssets.end())
    {
      // Decrement the reference count for this asset
      iter->second.count--;

      // See if the asset should be released and deleted
      if(iter->second.count != 0)
      {
        // Log an error for trying to drop a reference to an unknown ID
        ELOG() << "IAssetHandler(" << mAssetHandlerID << "):dtor("
          << iter->first << ") Non zero asset reference count("
          << iter->second.count << ")!" << std::endl;
      }

      void* anAsset = iter->second.asset;

      // Release or unload the asset before deleting it
      ReleaseAsset(iter->first, anAsset);

      // Remove this Asset Data structure from our map
      mAssets.erase(iter++);

      // Delete the Asset
      delete anAsset;
    }
  }

  void IAssetHandler::DropReference(const typeAssetID theAssetID, bool theRemoveFlag)
  {
    // Iterator to the asset if found
    std::map<const typeAssetID, typeAssetData>::iterator iter;

    // Try to find the asset using theAssetID as the key
    iter = mAssets.find(theAssetID);

    // Found asset? decrement the count value
    if(iter != mAssets.end())
    {
      // Decrement the reference count for this asset
      iter->second.count--;

      // See if the asset should be released and deleted
      if(theRemoveFlag && iter->second.count == 0)
      {
        void* anAsset = iter->second.asset;

        // Release or unload the asset before deleting it
        ReleaseAsset(iter->first, anAsset);

        // Remove this Asset Data structure from our map
        mAssets.erase(iter);

        // Delete the Asset
        delete anAsset;
      }
    }
    else
    {
      // Log an error for trying to drop a reference to an unknown ID
      ELOG() << "IAssetHandler(" << mAssetHandlerID << "):DropReference("
        << theAssetID << ") Asset ID not found!"
        << std::endl;
    }
  }

  const typeAssetHandlerID IAssetHandler::GetID(void) const
  {
    return mAssetHandlerID;
  }

  void* IAssetHandler::GetReference(void)
  {
    // Return the Dummy Asset reference
    return GetDummyAsset();
  }

  void* IAssetHandler::GetReference(const typeAssetID theAssetID, bool theLoadFlag)
  {
    // IAsset pointer that will be returned
    void* result = NULL;

    // Iterator to the asset if found
    std::map<const typeAssetID, typeAssetData>::iterator iter;

    // Try to find the asset using theAssetID as the key
    iter = mAssets.find(theAssetID);

    // Found asset? increment the count and return the reference
    if(iter != mAssets.end())
    {
      // Increment the reference count for this asset
      iter->second.count++;

      // Return the IAsset address found
      result = iter->second.asset;
    }
    else
    {
      // First attempt to acquire the asset first
      result = AcquireAsset(theAssetID);

      // Map the newly acquired asset to theAssetID provided
      if(NULL != result)
      {
        // Create a new AssetData to hold our asset information
        typeAssetData anAssetData;

        // Acquire the asset for the first time
        anAssetData.asset = result;
        anAssetData.count = 1;
        if(theLoadFlag)
        {
          // Attempt to load the asset now
          anAssetData.loaded = LoadAsset(theAssetID, anAssetData.asset);
        }
        else
        {
          // Asset is not yet loaded
          anAssetData.loaded = false;
        }

        // Store the newly acquired asset pointer in our map for future reference
        mAssets.insert(std::pair<const typeAssetID, typeAssetData>(theAssetID, anAssetData));
      }
    }

    // If result is NULL by this point then return the Dummy Asset instead
    if(NULL == result)
    {
      // Get the DummyAsset pointer now
      result = GetDummyAsset();
    }

    // Return the asset pointer provided to the caller
    return result;
  }

  bool IAssetHandler::IsLoaded(const typeAssetID theAssetID) const
  {
    // Result if asset was not found
    bool result = false;

    // Iterator to the asset if found
    std::map<const typeAssetID, typeAssetData>::const_iterator iter;

    // Try to find the asset using theAssetID as the key
    iter = mAssets.find(theAssetID);

    // Found asset? return the loaded indicator found
    if(iter != mAssets.end())
    {
      // Return the loaded value found
      result = iter->second.loaded;
    }
    else
    {
      // Log a warning for trying to determine loaded value of an unknown asset ID
      WLOG() << "IAssetHandler(" << mAssetHandlerID << "):IsLoaded("
        << theAssetID << ") Asset ID provided not found!"
        << std::endl;
    }

    // Return the result found or the default result assigned above
    return result;
  }

  bool IAssetHandler::LoadAsset(const typeAssetID theAssetID)
  {
    bool result = false;

    // Iterator to the asset if found
    std::map<const typeAssetID, typeAssetData>::iterator iter;

    // Try to find the asset using theAssetID as the key
    iter = mAssets.find(theAssetID);

    // Found asset? load the asset if not yet loaded
    if(iter != mAssets.end())
    {
      if(false == iter->second.loaded)
      {
        iter->second.loaded = LoadAsset(theAssetID, iter->second.asset);
      }
      else
      {
        // Log an error for trying to drop a reference to an unknown ID
        WLOG() << "IAssetHandler(" << mAssetHandlerID << "):LoadAsset("
          << theAssetID << ") Asset already loaded!"
          << std::endl;
      }

      // Return the result (true if already loaded or successful above)
      result = iter->second.loaded;
    }
    else
    {
      // Log an error for trying to drop a reference to an unknown ID
      ELOG() << "IAssetHandler(" << mAssetHandlerID << "):LoadAsset("
        << theAssetID << ") Asset ID not found!"
        << std::endl;
    }

    // Return the result, true if successfully or previously loaded, false otherwise
    return result;
  }

  void IAssetHandler::ReleaseAsset(const typeAssetID theAssetID, void* theAsset)
  {
    // Log that we are releasing this asset now
    ILOG() << "IAssetHandler(" << mAssetHandlerID << "):ReleaseAsset("
      << theAssetID << ") asset released."
      << std::endl;
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
