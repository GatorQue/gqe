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
       * GetID will return the Asset Handler ID used to identify this
       * IAssetHandler object.
       * @return GQE::typeAssetHandlerID is the ID for this IAssetHandler object
       */
      const GQE::typeAssetHandlerID GetID(void) const;

      /**
       * DropReference will decrement the reference counter for theAssetID
       * specified and optionally call the ReleaseAsset virtual function to
       * perform any cleanup operations before the IAsset derived pointer is
       * deleted if theRemoveFlag is set to true (default).
       * @param[in] theAssetID to drop the reference for
       * @param[in] theDropTime indicates if asset is dropped when count = 0 or exit
       */
      virtual void DropReference(const typeAssetID theAssetID,
        AssetDropTime theDropTime = AssetDropUnspecified) = 0;

      /**
       * IsLoaded will return true if the Resource specified by theResourceID
       * has been loaded.
       * @param[in] theAssetID to the Resource to determine loaded state
       * @return true if loaded, false otherwise
       */
      virtual bool IsLoaded(const typeAssetID theAssetID) const = 0;

      /**
       * GetFilename is responsible for retrieving the filename to use when
       * loading theAssetID specified.
       * @param[in] theAssetID to get filename for
       */
      virtual const std::string GetFilename(const typeAssetID theAssetID) const = 0;

      /**
       * SetFilename is responsible for noting the filename to use when loading
       * theAssetID specified.
       * @param[in] theAssetID to set filename for
       * @param[in] theFilename to use when loading this asset from a file
       */
      virtual void SetFilename(const typeAssetID theAssetID, std::string theFilename) = 0;

      /**
       * GetLoadStyle allows someone to find out the loading style of
       * theAssetID provided.
       * @param[in] theAssetID of the asset to find loading style for
       * @return the loading style for the asset or LoadFromUnknown otherwise
       */
      virtual AssetLoadStyle GetLoadStyle(const typeAssetID theAssetID) const = 0;

      /**
       * SetLoadStyle allows someone to change the loading style of theAssetID
       * provided. A warning will be printed of the loading style is changed
       * after an asset has already been loaded.
       * @param[in] theAssetID of the asset to change loading style
       * @param[in] theLoadStyle (File,Mem,Network) to use when loading this asset
       */
      virtual void SetLoadStyle(const typeAssetID theAssetID, AssetLoadStyle theLoadStyle) = 0;

      /**
       * GetLoadTime allows someone to find out the loading time of
       * theAssetID provided.
       * @param[in] theAssetID of the asset to find loading time for
       * @return the loading style for the asset or LoadFromUnknown otherwise
       */
      virtual AssetLoadTime GetLoadTime(const typeAssetID theAssetID) const = 0;

      /**
       * SetLoadTime allows someone to change the loading time of theAssetID
       * provided. A warning will be printed of the loading time is changed
       * after an asset has already been loaded.
       * @param[in] theAssetID of the asset to change loading time
       * @param[in] theLoadTime (Now,Later) of when to load this asset
       */
      virtual void SetLoadTime(const typeAssetID theAssetID, AssetLoadTime theLoadTime) = 0;

      /**
       * LoadAsset is responsible for loading the asset specified by theAssetID
       * provided according to the previously registered style (see
       * GetReference and SetLoadStyle).
       * @param[in] theAssetID of the asset to load
       */
      virtual bool LoadAsset(const typeAssetID theAssetID) = 0;

      /**
       * LoadAllAssets is responsible for loading all unloaded assets that are
       * currently registered with this IAssetHandler derived class.
       * @return true if all assets were successfully loaded, false otherwise
       */
      virtual bool LoadAllAssets(void) = 0;

    protected:

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// ID specified for this IAssetHandler
      const typeAssetHandlerID mAssetHandlerID;

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
