/**
 * Provides the IAssetHandler class in the GQE namespace which is
 * responsible for providing management of the IAsset derived classes.
 *
 * @file include/GQE/Core/interfaces/IAssetHandler.hpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */
#ifndef   CORE_IASSET_HANDLER_HPP_INCLUDED
#define   CORE_IASSET_HANDLER_HPP_INCLUDED

#include <map>
#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// Provides the base class interface for all game states
  class GQE_API IAssetHandler
  {
    public:
      /**
       * IAssetHandler default constructor.
       * @param[in] theAssetHandlerID to use for this Resource Handler
       */
      IAssetHandler(const typeAssetHandlerID theAssetHandlerID);

      /**
       * IAssetHandler deconstructor
       */
      virtual ~IAssetHandler();

      /**
       * DropReference will decrement the reference counter for theAssetID
       * specified and optionally call the ReleaseAsset virtual function to
       * perform any cleanup operations before the IAsset derived pointer is
       * deleted if theRemoveFlag is set to true (default).
       * @param[in] theAssetID to drop the reference for
       * @param[in] theRemoveFlag indicates the asset should be removed when count=0
       */
      void DropReference(const typeAssetID theAssetID, bool theRemoveFlag = true);

      /**
       * GetID will return the Asset Handler ID used to identify this
       * IAssetHandler object.
       * @return GQE::typeAssetHandlerID is the ID for this IAssetHandler object
       */
      const GQE::typeAssetHandlerID GetID(void) const;

      /**
       * GetReference will return the dummy asset reference address since no
       * ID was provided to find. This enables system stability since all
       * Assets will have valid addresses
       */
      void* GetReference(void);

      /**
       * GetReference will retrieve the asset registered under theAssetID and
       * increment the reference counter for this asset or call the
       * AcquireAsset pure virtual function to obtain it if it hasn't yet been
       * created.
       * @param[in] theAssetID to lookup for the reference
       * @param[in] theLoadFlag indicates the asset should be loaded immediately
       * @return the asset found or a newly acquired asset if not found
       */
      void* GetReference(const typeAssetID theAssetID, bool theLoadFlag = false);

      /**
       * IsLoaded will return true if the Resource specified by theResourceID
       * has been loaded.
       * @param[in] theAssetID to the Resource to determine loaded state
       * @return true if loaded, false otherwise
       */
      bool IsLoaded(const typeAssetID theAssetID) const;

      /**
       * LoadAsset will find and load theAssetID provided if it exists and has
       * not been previously loaded.
       * @param[in] theAssetID to lookup and load
       * @return true if the asset was loaded successfully, false otherwise
       */
      bool LoadAsset(const typeAssetID theAssetID);

    protected:
      /// Structure holding information about each Resource
      struct typeAssetData
      {
        void*  asset;  ///< The asset being shared
        Uint32 count;  ///< Number of people referencing this Asset
        bool   loaded; ///< Is the Asset currently loaded?
      };

      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// ID specified for this IAssetHandler
      const typeAssetHandlerID mAssetHandlerID;
      /// Map that associates asset ID's with their appropriate TAssetData
      std::map<const typeAssetID, typeAssetData> mAssets;

      /**
       * AcquireAsset is responsible for creating an IAsset derived asset and
       * returning it to the caller.
       * @param[in] theAssetID of the asset to acquire
       * @return a pointer to the newly created asset
       */
      virtual void* AcquireAsset(const typeAssetID theAssetID) = 0;
      
      /**
       * GetDummyAsset is responsible for returning a pointer to a Dummy asset
       * which will be returned if AcquireAsset fails to create an asset or
       * GetReference() is called. This makes the system more stable since all
       * assets will have a valid reference pointer
       * @return pointer to a dummy asset
       */
      virtual void* GetDummyAsset(void) = 0;

      /**
       * LoadAsset is responsible for loading theAsset returned by AcquireAsset
       * and must provide a default implementation for all loading style types.
       * @param[in] theAsset pointer to load
       * @return true if the asset was successfully loaded, false otherwise
       */
      virtual bool LoadAsset(const typeAssetID theAssetID, void* theAsset) = 0;

      /**
       * ReleaseAsset is responsible for doing any cleanup on the IAsset
       * derived asset before the IAsset pointer is deleted from memory.
       * @param[in] theAsset pointer release/unload before it is deleted
       */
      virtual void ReleaseAsset(const typeAssetID theAssetID, void* theAsset);

    private:
      /**
       * Our copy constructor is private because we do not allow copies of our
       * class
       */
      IAssetHandler(const IAssetHandler&);  // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies of our
       * class
       */
      IAssetHandler& operator=(const IAssetHandler&); // Intentionally undefined
  }; // class IAssetHandler
} // namespace GQE

#endif // CORE_IASSET_HANDLER_HPP_INCLUDED

/**
 * @class GQE::IAssetHandler
 * @ingroup Core
 * The IAssetHandler interface class is what defines the minimum methods
 * to be defined to manage a specific asset/resource type. By implementing this
 * interface you can create your own custom asset handler for your own custom
 * game asset.
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