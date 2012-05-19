/**
 * Provides the IAssetHandler class in the GQE namespace which is
 * responsible for providing management of the IAsset derived classes.
 *
 * @file include/GQE/Core/interfaces/IAssetHandler.hpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */
#ifndef   CORE_TASSET_HANDLER_HPP_INCLUDED
#define   CORE_TASSET_HANDLER_HPP_INCLUDED

#include <map>
#include <typeinfo>
#include <GQE/Core/interfaces/IAssetHandler.hpp>
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
  /// Provides the base class interface for all game states
  template<class TYPE>
  class GQE_API TAssetHandler : public IAssetHandler
  {
    public:
      /**
       * TAssetHandler default constructor.
       */
      TAssetHandler() :
        IAssetHandler(typeid(TYPE).name())
      {
        ILOG() << "TAssetHandler::ctor(" << GetID() << ")" << std::endl;
      }

      /**
       * TAssetHandler deconstructor
       */
      virtual ~TAssetHandler()
      {
        ILOG() << "TAssetHandler::dtor(" << GetID() << ")" << std::endl;

        // Iterator to use while deleting all assets
        typename std::map<const typeAssetID, typeAssetData>::iterator iter;

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
            ELOG() << "TAssetHandler(" << GetID() << "):dtor("
              << iter->first << ") Non zero asset reference count("
              << iter->second.count << ")!" << std::endl;
          }
          const typeAssetID anAssetID = iter->first;
          TYPE* anAsset = iter->second.asset;

          // Remove this Asset Data structure from our map
          mAssets.erase(iter++);

          // Release the asset
          ReleaseAsset(anAssetID, anAsset);

          // Don't keep pointers to something that has been released
          anAsset = NULL;
        }
      }

      /**
       * DropReference will decrement the reference counter for theAssetID
       * specified and optionally call the ReleaseAsset virtual function to
       * perform any cleanup operations before the IAsset derived pointer is
       * deleted if theRemoveFlag is set to true (default).
       * @param[in] theAssetID to drop the reference for
       * @param[in] theDropTime indicates if asset is dropped when count = 0 or later
       */
      virtual void DropReference(const typeAssetID theAssetID,
        AssetDropTime theDropTime = AssetDropUnspecified)
      {
        // Iterator to the asset if found
        typename std::map<const typeAssetID, typeAssetData>::iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mAssets.find(theAssetID);

        // Found asset? decrement the count value
        if(iter != mAssets.end())
        {
          // Default to dropTime previously registered
          AssetDropTime anDropTime = iter->second.dropTime;

          // Decrement the reference count for this asset
          iter->second.count--;

          // Caller specified another dropTime value? use it instead
          if(AssetDropUnspecified != theDropTime &&
            theDropTime > AssetDropUnspecified &&
            theDropTime <= AssetDropAtExit)
          {
            anDropTime = theDropTime;
          }

          // Use anDropTime specified above
          switch(anDropTime)
          {
          default:
            ELOG() << "TAssetHandler(" << GetID() << ")::DropReference("
              << theAssetID << ") Unknown drop time specified!" << std::endl;
          case AssetDropUnspecified:
          case AssetDropAtZero:
            // See if the asset should be released and deleted
            if(iter->second.count == 0)
            {
              TYPE* anAsset = iter->second.asset;

              // Remove this Asset Data structure from our map
              mAssets.erase(iter++);

              // Release the asset
              ReleaseAsset(theAssetID, anAsset);

              // Don't keep pointers to something that has been released
              anAsset = NULL;
            }
            break;
          case AssetDropAtExit:
            /* Do nothing, destructor will release each asset */
            break;
          }
        }
        else
        {
          // Log an error for trying to drop a reference to an unknown ID
          ELOG() << "TAssetHandler(" << GetID() << "):DropReference("
            << theAssetID << ") Asset ID not found!"
            << std::endl;
        }
      }

      /**
       * GetReference will return the dummy asset reference address since no
       * ID was provided to find. This enables system stability since all
       * Assets will have valid addresses
       */
      TYPE* GetReference(void)
      {
        // Return the Dummy Asset reference
        return &mDummyAsset;
      }

      /**
       * GetReference will retrieve the asset registered under theAssetID and
       * increment the reference counter for this asset or call the
       * AcquireAsset pure virtual function to obtain it if it hasn't yet been
       * created.
       * @param[in] theAssetID to lookup for the reference
       * @param[in] theLoadTime (Now, Later) of when to load this asset
       * @param[in] theLoadStyle (File, Mem, Network) to use when loading this asset
       * @return the asset found or a newly acquired asset if not found
       */
      TYPE* GetReference(const typeAssetID theAssetID,
        AssetLoadTime theLoadTime = AssetLoadLater,
        AssetLoadStyle theLoadStyle = AssetLoadFromFile,
        AssetDropTime theDropTime = AssetDropAtZero)
      {
        // IAsset pointer that will be returned
        TYPE* anResult = NULL;

        // Iterator to the asset if found
        typename std::map<const typeAssetID, typeAssetData>::iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mAssets.find(theAssetID);

        // Found asset? increment the count and return the reference
        if(iter != mAssets.end())
        {
          // Increment the reference count for this asset
          iter->second.count++;

          // Return the IAsset address found
          anResult = iter->second.asset;
        }
        else
        {
          // First attempt to acquire the asset first
          anResult = AcquireAsset(theAssetID);

          // Map the newly acquired asset to theAssetID provided
          if(NULL != anResult)
          {
            // Create a new AssetData to hold our asset information
            typeAssetData anAssetData;

            // Acquire the asset for the first time
            anAssetData.asset = anResult;
            anAssetData.count = 1;
            anAssetData.loaded = false;
            anAssetData.loadStyle = theLoadStyle;
            anAssetData.loadTime = theLoadTime;
            anAssetData.dropTime = AssetDropAtZero;
            anAssetData.filename = theAssetID;

            // Check the Load Style range provided and force to LoadFromUnknown if out of range
            if(theLoadStyle < AssetLoadFromUnknown || theLoadStyle > AssetLoadFromNetwork)
            {
              // Force style to AssetLoadFromFile if out of enum range
              anAssetData.loadStyle = AssetLoadFromFile;
            }

            // Check the Load Time range provided and force LoadNow if out of range
            if(theLoadTime < AssetLoadNow || theLoadTime > AssetLoadLater)
            {
              // Force load time to AssetLoadLater if out of enum range
              anAssetData.loadTime = AssetLoadLater;
            }

            // Store the newly acquired asset pointer in our map for future reference
            mAssets.insert(std::pair<const typeAssetID, typeAssetData>(theAssetID, anAssetData));

            // Were we asked to load the asset now?
            if(AssetLoadNow == anAssetData.loadTime)
            {
              // Load the Asset now
              LoadAsset(theAssetID);
            }
          }
        }

        // If result is NULL by this point then return the Dummy Asset instead
        if(NULL == anResult)
        {
          // Get the DummyAsset pointer now
          anResult = &mDummyAsset;
        }

        // Return the asset pointer provided to the caller
        return anResult;
      }

      /**
       * IsLoaded will return true if the Resource specified by theResourceID
       * has been loaded.
       * @param[in] theAssetID to the Resource to determine loaded state
       * @return true if loaded, false otherwise
       */
      virtual bool IsLoaded(const typeAssetID theAssetID) const
      {
        // Result if asset was not found
        bool anResult = false;

        // Iterator to the asset if found
        typename std::map<const typeAssetID, typeAssetData>::const_iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mAssets.find(theAssetID);

        // Found asset? return the loaded indicator found
        if(iter != mAssets.end())
        {
          // Return the loaded value found
          anResult = iter->second.loaded;
        }
        else
        {
          // Log a warning for trying to determine loaded value of an unknown asset ID
          WLOG() << "TAssetHandler(" << GetID() << "):IsLoaded("
            << theAssetID << ") Asset ID provided not found!"
            << std::endl;
        }

        // Return the result found or the default result assigned above
        return anResult;
      }

      /**
       * GetFilename is responsible for retrieving the filename to use when
       * loading theAssetID specified.
       * @param[in] theAssetID to set filename for
       * @param[in] theFilename to use when loading this asset from a file
       */
      virtual const std::string GetFilename(const typeAssetID theAssetID) const
      {
        // Return empty string if no filename was found
        std::string anResult;

        // Iterator to the asset if found
        typename std::map<const typeAssetID, typeAssetData>::const_iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mAssets.find(theAssetID);

        // Found asset? return the filename found
        if(iter != mAssets.end())
        {
          // Retrieve the filename for this asset
          anResult = iter->second.filename;
        }
        else
        {
          // Log a warning for trying to determine loaded value of an unknown asset ID
          WLOG() << "TAssetHandler(" << GetID() << "):GetFilename("
            << theAssetID << ") Asset ID provided not found!"
            << std::endl;
        }

        // Return anResult found, empty string otherwise
        return anResult;
      }

      /**
       * SetFilename is responsible for noting the filename to use when loading
       * theAssetID specified.
       * @param[in] theAssetID to set filename for
       * @param[in] theFilename to use when loading this asset from a file
       */
      virtual void SetFilename(const typeAssetID theAssetID, std::string theFilename)
      {
        // Iterator to the asset if found
        typename std::map<const typeAssetID, typeAssetData>::iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mAssets.find(theAssetID);

        // Found asset? return the loaded indicator found
        if(iter != mAssets.end())
        {
          // Print warning if asset has already been loaded
          if(false == iter->second.loaded)
          {
            WLOG() << "TAssetHandler(" << GetID() << ")::SetFilename("
              << theAssetID << ") Asset is already loaded" << std::endl;
          }

          // Set the filename for this asset
          iter->second.filename = theFilename;
        }
        else
        {
          // Log a warning for trying to determine loaded value of an unknown asset ID
          WLOG() << "TAssetHandler(" << GetID() << "):SetFilename("
            << theAssetID << ") Asset ID provided not found!"
            << std::endl;
        }
      }

      /**
       * GetLoadStyle allows someone to find out the loading style of
       * theAssetID provided.
       * @param[in] theAssetID of the asset to find loading style for
       * @return the loading style for the asset or LoadFromUnknown otherwise
       */
      virtual AssetLoadStyle GetLoadStyle(const typeAssetID theAssetID) const
      {
        AssetLoadStyle anResult = AssetLoadFromUnknown;

        // Iterator to the asset if found
        typename std::map<const typeAssetID, typeAssetData>::const_iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mAssets.find(theAssetID);

        if(iter != mAssets.end())
        {
          // Retrieve our loading style from the asset found
          anResult = iter->second.loadStyle;
        }
        else
        {
          // Log a warning for trying to determine loaded value of an unknown asset ID
          WLOG() << "TAssetHandler(" << GetID() << "):GetLoadStyle("
            << theAssetID << ") Asset ID provided not found!"
            << std::endl;
        }

        // Return anResult found or LoadFromUnknown if none was found
        return anResult;
      }

      /**
       * SetLoadStyle allows someone to change the loading style of theAssetID
       * provided. A warning will be printed of the loading style is changed
       * after an asset has already been loaded.
       * @param[in] theAssetID of the asset to change loading style
       * @param[in] theLoadStyle (File,Mem,Network) to use when loading this asset
       */
      void SetLoadStyle(const typeAssetID theAssetID, AssetLoadStyle theLoadStyle)
      {
        // Iterator to the asset if found
        typename std::map<const typeAssetID, typeAssetData>::iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mAssets.find(theAssetID);

        // Found asset? return the loaded indicator found
        if(iter != mAssets.end())
        {
          // Check the range provided and force to Unknown if out of range
          if(theLoadStyle < AssetLoadFromUnknown || theLoadStyle > AssetLoadFromNetwork)
          {
            // Force style to AssetLoadFromFile if out of enum range
            iter->second.loadStyle = AssetLoadFromFile;
          }
          else
          {
            // Set the asset Load Style now
            iter->second.loadStyle = theLoadStyle;
          }

          // Are we changing the load style after it was loaded!?
          if(true == iter->second.loaded)
          {
            switch(theLoadStyle)
            {
            case AssetLoadFromFile:
              WLOG() << "TAssetHandler(" << GetID() << ")::SetLoadStyle("
                << theAssetID << ") Setting load style to load from a file" << std::endl;
              break;
            case AssetLoadFromMemory:
              WLOG() << "TAssetHandler(" << GetID() << ")::SetLoadStyle("
                << theAssetID << ") Setting load style to load from memory" << std::endl;
              break;
            case AssetLoadFromNetwork:
              WLOG() << "TAssetHandler(" << GetID() << ")::SetLoadStyle("
                << theAssetID << ") Setting load style to load from network" << std::endl;
              break;
            case AssetLoadFromUnknown:
            default:
              WLOG() << "TAssetHandler(" << GetID() << ")::SetLoadStyle("
                << theAssetID << ") Setting load style to load from unknown source!" << std::endl;
              break;
            }
          }
        }
        else
        {
          // Log a warning for trying to determine loaded value of an unknown asset ID
          WLOG() << "TAssetHandler(" << GetID() << "):SetLoadStyle("
            << theAssetID << ") Asset ID provided not found!"
            << std::endl;
        }
      }

      /**
       * GetLoadTime allows someone to find out the loading time of
       * theAssetID provided.
       * @param[in] theAssetID of the asset to find loading time for
       * @return the loading time for the asset or AssetLoadLater otherwise
       */
      virtual AssetLoadTime GetLoadTime(const typeAssetID theAssetID) const
      {
        AssetLoadTime anResult = AssetLoadLater;

        // Iterator to the asset if found
        typename std::map<const typeAssetID, typeAssetData>::const_iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mAssets.find(theAssetID);

        if(iter != mAssets.end())
        {
          // Retrieve our loading time from the asset found
          anResult = iter->second.loadTime;
        }
        else
        {
          // Log a warning for trying to determine loaded value of an unknown asset ID
          WLOG() << "TAssetHandler(" << GetID() << "):GetLoadTime("
            << theAssetID << ") Asset ID provided not found!"
            << std::endl;
        }

        // Return anResult found or AssetLoadLater if none was found
        return anResult;
      }

      /**
       * SetLoadTime allows someone to change the loading time of theAssetID
       * provided. A warning will be printed of the loading time is changed
       * after an asset has already been loaded.
       * @param[in] theAssetID of the asset to change loading time
       * @param[in] theLoadTime (Now,Later) of when to load this asset
       */
      virtual void SetLoadTime(const typeAssetID theAssetID, AssetLoadTime theLoadTime)
      {
        // Iterator to the asset if found
        typename std::map<const typeAssetID, typeAssetData>::iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mAssets.find(theAssetID);

        // Found asset? return the loaded indicator found
        if(iter != mAssets.end())
        {
          // Check the range provided and force to Unknown if out of range
          if(theLoadTime < AssetLoadNow || theLoadTime > AssetLoadLater)
          {
            // Force load time to AssetLoadLater if out of enum range
            iter->second.loadTime = AssetLoadLater;
          }
          else
          {
            // Set the asset Load Time now
            iter->second.loadTime = theLoadTime;
          }

          // Are we changing the load time after it was loaded!?
          if(true == iter->second.loaded)
          {
            switch(theLoadTime)
            {
            case AssetLoadNow:
              WLOG() << "TAssetHandler(" << GetID() << ")::SetLoadTime("
                << theAssetID << ") Setting load time to now" << std::endl;
              break;
            case AssetLoadLater:
              WLOG() << "TAssetHandler(" << GetID() << ")::SetLoadTime("
                << theAssetID << ") Setting load time to later" << std::endl;
              break;
            default:
              WLOG() << "TAssetHandler(" << GetID() << ")::SetLoadTime("
                << theAssetID << ") Setting load time to unknown!" << std::endl;
              break;
            }
          }
        }
        else
        {
          // Log a warning for trying to determine loaded value of an unknown asset ID
          WLOG() << "TAssetHandler(" << GetID() << "):SetLoadTime("
            << theAssetID << ") Asset ID provided not found!"
            << std::endl;
        }
      }

      /**
       * GetDopTime allows someone to find out the drop time of theAssetID
       * provided.
       * @param[in] theAssetID of the asset to find drop time for
       * @return the drop time for the asset or AssetDropUnspecified otherwise
       */
      virtual AssetDropTime GetDropTime(const typeAssetID theAssetID) const
      {
        AssetDropTime anResult = AssetDropUnspecified;

        // Iterator to the asset if found
        typename std::map<const typeAssetID, typeAssetData>::const_iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mAssets.find(theAssetID);

        if(iter != mAssets.end())
        {
          // Retrieve our loading time from the asset found
          anResult = iter->second.dropTime;
        }
        else
        {
          // Log a warning for trying to determine loaded value of an unknown asset ID
          WLOG() << "TAssetHandler(" << GetID() << "):GetDropTime("
            << theAssetID << ") Asset ID provided not found!"
            << std::endl;
        }

        // Return anResult found or AssetDropUnspecified if none was found
        return anResult;
      }

      /**
       * SetDropTime allows someone to change the drop time of theAssetID
       * provided.
       * @param[in] theAssetID of the asset to change loading time
       * @param[in] theDropTime (Zero, Exit) of when to drop this asset
       */
      virtual void SetDropTime(const typeAssetID theAssetID,
        AssetDropTime theDropTime)
      {
        // Iterator to the asset if found
        typename std::map<const typeAssetID, typeAssetData>::iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mAssets.find(theAssetID);

        // Found asset? return the loaded indicator found
        if(iter != mAssets.end())
        {
          // Check the range provided and force to Unknown if out of range
          if(theDropTime < AssetDropAtZero || theDropTime > AssetDropAtExit)
          {
            // Force drop time to AssetDropAtZero if out of enum range
            iter->second.dropTime = AssetDropAtZero;
          }
          else
          {
            // Set the asset drop time now
            iter->second.dropTime = theDropTime;
          }
        }
        else
        {
          // Log a warning for trying to determine loaded value of an unknown asset ID
          WLOG() << "TAssetHandler(" << GetID() << "):SetDropTime("
            << theAssetID << ") Asset ID provided not found!"
            << std::endl;
        }
      }

      /**
       * LoadAsset is responsible for loading the asset specified by theAssetID
       * provided according to the previously registered style (see
       * GetReference and SetLoadStyle).
       * @param[in] theAssetID of the asset to load
       */
      virtual bool LoadAsset(const typeAssetID theAssetID)
      {
        // Result if asset was not found
        bool anResult = false;

        // Iterator to the asset if found
        typename std::map<const typeAssetID, typeAssetData>::iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mAssets.find(theAssetID);

        // Found asset? return the loaded indicator found
        if(iter != mAssets.end())
        {
          if(false == iter->second.loaded)
          {
            // Attempt to load the asset now using the correct style
            switch(iter->second.loadStyle)
            {
            case AssetLoadFromFile:
              iter->second.loaded = LoadFromFile(theAssetID, *(iter->second.asset));
              break;
            case AssetLoadFromMemory:
              iter->second.loaded = LoadFromMemory(theAssetID, *(iter->second.asset));
              break;
            case AssetLoadFromNetwork:
              iter->second.loaded = LoadFromNetwork(theAssetID, *(iter->second.asset));
              break;
            case AssetLoadFromUnknown:
            default:
              ELOG() << "TAssetHandler(" << GetID() << ")::LoadAsset("
                << theAssetID << ") unknown loading style specified!" << std::endl;
              break;
            }
          }

          // Set our return result
          anResult = iter->second.loaded;
        }
        else
        {
          // Log a warning for trying to determine loaded value of an unknown asset ID
          WLOG() << "TAssetHandler(" << GetID() << "):LoadAsset("
            << theAssetID << ") Asset ID provided not found!"
            << std::endl;
        }

        // Return anResult which is true if asset was loaded, false otherwise
        return anResult;
      }

      /**
       * LoadAllAssets is responsible for loading all unloaded assets that are
       * currently registered with this IAssetHandler derived class.
       * @return true if all assets were successfully loaded, false otherwise
       */
      virtual bool LoadAllAssets(void)
      {
        // Return true if all assets load successfully
        bool anResult = true;
    
        // Iterator for each typeAssetData registered
        typename std::map<const typeAssetID, typeAssetData>::iterator iter;

        // Loop through each asset handler and tell it to load its assets
        iter = mAssets.begin();
        while(iter != mAssets.end())
        {
          // Is this an asset that hasn't been loaded yet? load it now
          if(false == iter->second.loaded)
          {
            // Attempt to load the asset now using the correct style
            switch(iter->second.loadStyle)
            {
            case AssetLoadFromFile:
              iter->second.loaded = LoadFromFile(iter->first, *(iter->second.asset));
              break;
            case AssetLoadFromMemory:
              iter->second.loaded = LoadFromMemory(iter->first, *(iter->second.asset));
              break;
            case AssetLoadFromNetwork:
              iter->second.loaded = LoadFromNetwork(iter->first, *(iter->second.asset));
              break;
            case AssetLoadFromUnknown:
            default:
              ELOG() << "TAssetHandler(" << GetID() << ")::LoadAllAssets("
                << iter->first << ") unknown loading style specified!" << std::endl;
              break;
            }
          }

          // Set our return result
          anResult &= iter->second.loaded;

          // Move to the next registered Assets value
          iter++;
        }

        // Return anResult which will still be true if all LoadAllAssets returned
        // true or there are no registered Assets, false otherwise
        return anResult;
      }

    protected:
      /**
       * AcquireAsset is responsible for creating an IAsset derived asset and
       * returning it to the caller.
       * @param[in] theAssetID of the asset to acquire
       * @return a pointer to the newly created asset
       */
      virtual TYPE* AcquireAsset(const typeAssetID theAssetID)
      {
        ILOG() << "TAssetHandler(" << GetID() << "):AcquireAsset("
          << theAssetID << ") Creating asset" << std::endl;
        return new(std::nothrow) TYPE();
      }
      
      /**
       * ReleaseAsset is responsible for unloading and removing theAsset
       * specified and deleting the memory used by the asset.
       * @param[in] theAssetID of the asset to be released
       * @param[in] theAsset to be released
       */
      virtual void ReleaseAsset(const typeAssetID theAssetID, TYPE* theAsset)
      {
        ILOG() << "TAssetHandler(" << GetID() << "):ReleaseAsset("
          << theAssetID << ") Releasing asset" << std::endl;

        // Just delete the memory used for the asset
        delete theAsset;
      }

      /**
       * LoadFromFile is responsible for loading theAsset from a file and must
       * be defined by the derived class since the interface for TYPE is
       * unknown at this stage.
       * @param[in] theAssetID of the asset to be loaded
       * @param[in] theAsset pointer to load
       * @return true if the asset was successfully loaded, false otherwise
       */
      virtual bool LoadFromFile(const typeAssetID theAssetID, TYPE& theAsset) = 0;

      /**
       * LoadFromMemory is responsible for loading theAsset from memory and
       * must be defined by the derived class since the interface for TYPE is
       * unknown at this stage.
       * @param[in] theAssetID of the asset to be loaded
       * @param[in] theAsset pointer to load
       * @return true if the asset was successfully loaded, false otherwise
       */
      virtual bool LoadFromMemory(const typeAssetID theAssetID, TYPE& theAsset) = 0;

      /**
       * LoadFromNetwork is responsible for loading theAsset from network and
       * must be defined by the derived class since the interface for TYPE is
       * unknown at this stage.
       * @param[in] theAssetID of the asset to be loaded
       * @param[in] theAsset pointer to load
       * @return true if the asset was successfully loaded, false otherwise
       */
      virtual bool LoadFromNetwork(const typeAssetID theAssetID, TYPE& theAsset) = 0;

    private:
      // Structures
      ///////////////////////////////////////////////////////////////////////////
      /// Structure holding information about each Resource
      struct typeAssetData {
        TYPE*          asset;     ///< The asset being shared
        Uint32         count;     ///< Number of people referencing this Asset
        bool           loaded;    ///< Is the Asset currently loaded?
        AssetLoadStyle loadStyle; ///< Load type (File, Memory, Network, etc)
        AssetLoadTime  loadTime;  ///< Load time (Now, later)
        AssetDropTime  dropTime;  ///< Drop time at (Zero, Exit)
        std::string    filename;  ///< Filename to use when loading this asset
      };

      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Map that associates asset ID's with their appropriate TAssetData
      std::map<const typeAssetID, typeAssetData> mAssets;
      /// Dummy asset that will be returned if an asset can't be Acquired
      TYPE mDummyAsset;
  }; // class TAssetHandler
} // namespace GQE

#endif // CORE_TASSET_HANDLER_HPP_INCLUDED

/**
 * @class GQE::TAssetHandler
 * @ingroup Core
 * The TAssetHandler template class is used to quickly provide a IAssetHandler
 * derived class for handling the Asset type specified.
 *
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