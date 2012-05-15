/**
 * Provides the TAsset template class for managing a single Asset and is used
 * by the IAssetHandler in the GQE namespace which is responsible for providing
 * the Asset management facilities for each asset type.
 *
 * @file include/GQE/Core/interfaces/TAsset.hpp
 * @author Ryan Lindeman
 * @date 20100723 - Initial Release
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110127 - Use new OS independent Uint/Int types
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Change to system include style
 * @date 20110627 - Removed extra ; from namespace
 * @date 20110721 - Make GetAsset method return TYPE& because that is usually
 *                  what SFML is expecting.
 * @date 20120503 - Rewrote TAsset to use RAII technique and more flexible
 *                  handling of Assets using the new IAssetHandler classes.
 * @date 20120514 - Fix comment whitespace and added GetID method call
 */
#ifndef   CORE_TASSET_HPP_INCLUDED
#define   CORE_TASSET_HPP_INCLUDED

#include <string>
#include <GQE/Core/interfaces/IAssetHandler.hpp>
#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// Provides base template class for all game assets
  template<class TYPE>
  class GQE_API TAsset
  {
    protected:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Asset Handler class that will manage this asset
      IAssetHandler& mAssetHandler;
      /// Pointer to the loaded asset
      TYPE*          mAsset;
      /// Asset ID specified for this asset
      typeAssetID    mAssetID;

    public:
      /**
        * TAsset default constructor is used for TAsset classes whose ID's are
        * not available at construction time.  ID's can be specified after
        * construction time using the SetID method.  A dummy Asset will be
        * provided by the AssetManager using theAssetHandlerID provided until
        * a proper ID can be obtained.
        * @param[in] theAssetHandler to use for retrieving asset references
        */
      TAsset(IAssetHandler& theAssetHandler) :
        mAssetHandler(theAssetHandler),
        mAsset(static_cast<TYPE*>(mAssetHandler.GetReference()))
      {
      }

      /**
        * TAsset constructor that should be used most frequently since it
        * allows theAssetManager to provide the asset at construction time.
        * @param[in] theAssetHandler to use for retrieving asset references
        * @param[in] theAssetID to use for this asset
        * @param[in] theLoadFlag indicating immediate loading of asset
        */
      TAsset(IAssetHandler& theAssetHandler, const typeAssetID theAssetID,
        bool theLoadFlag = false) :
        mAssetHandler(theAssetHandler),
        mAsset(static_cast<TYPE*>(mAssetHandler.GetReference(theAssetID, theLoadFlag))),
        mAssetID(theAssetID)
      {
      }

      /**
        * TAsset copy constructor will allow for copying of assets by
        * incrementing the reference counter for this asset through the
        * Asset Handler provided at construction time.
        */
      TAsset(const TAsset<TYPE>& theCopy) :
        mAssetHandler(theCopy.mAssetHandler),
        mAssetID(theCopy.mAssetID)
      {
        // Increment reference count to this asset
        mAsset = static_cast<TYPE*>(mAssetHandler.GetReference(mAssetID));
      }

      /**
       * TAsset deconstructor
       */
      virtual ~TAsset()
      {
        // Drop reference to this asset
        mAssetHandler.DropReference(mAssetID);
      }

      /**
       * IsLoaded will return true if the Asset has been loaded.
       * @return true if loaded, false otherwise
       */
      bool IsLoaded(void) const
      {
        return mAssetHandler.IsLoaded(mAssetID);
      }

      /**
       * GetID will return the ID being used for this asset.
       * @return the Asset ID assigned to this asset
       */
      const typeAssetID GetID(void) const
      {
        return mAssetID;
      }

      /**
       * SetID will set the ID for this asset and get a reference to the
       * asset.
       * @param[in] theAssetID to use for this asset
       * @param[in] theLoadFlag indicating immediate loading of asset
       */
      void SetID(const typeAssetID theAssetID, bool theLoadFlag = false)
      {
        // Make note of the new Asset ID
        mAssetID = theAssetID;

        // Try to obtain a reference to the new Asset from Handler
        mAsset = static_cast<TYPE*>(mAssetHandler.GetReference(mAssetID, theLoadFlag));
      }

      /**
       * GetAsset will return the Asset if it is available.
       * @return pointer to the Asset or NULL if not available yet.
       */
      TYPE& GetAsset(void) const
      {
        return *mAsset;
      }

      /**
       * TAsset assignment operator will allow for copying of assets by
       * incrementing the reference count for this asset.
       * @param[in] theRight hand side of the = operation
       */
      TAsset<TYPE>& operator=(const TAsset<TYPE>& theRight)
      {
        // Use copy constructor to duplicate theRight side
        //TAsset anCopy(theRight);

        // Now swap my local copy with anCopy made above
        //std::swap(mAssetHandler, anCopy.mAssetHandler);
        //std::swap(mAsset, anCopy.mAsset);
        //std::swap(mAssetID, anCopy.mAssetID);

        // Increment the reference count to this Asset
        //mAssetHandler.GetReference(mAssetID);

        // Return my pointer
        return *this;
      }
  }; // class TAsset
} // namespace GQE

#endif // CORE_TASSET_HPP_INCLUDED

/**
 * @class GQE::TAsset
 * @ingroup Core
 * The TAsset template class is the template used to create a new asset type
 * reference.  It provides indirect reference counting and dummy asset
 * references if no Asset ID is provided (see IAssetHandler).
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
