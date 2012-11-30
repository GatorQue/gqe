/**
 * Provides the AssetManager class in the GQE namespace which is responsible
 * for managing all IAssetHandler classes and providing application wide
 * Asset management facilities.
 *
 * @file include/GQE/Core/classes/AssetManager.hpp
 * @author Ryan Lindeman
 * @date 20100723 - Initial Release
 * @date 20110110 - Added ability to get sf::Sound from SoundAsset
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Added new Config asset type
 * @date 20110218 - Change to system include style
 * @date 20110627 - Remove extra , from enum and extra ; from namespace
 * @date 20120503 - Redo AssetManager to be more flexible and use RAII techniques
 */
#ifndef   CORE_ASSET_MANAGER_HPP_INCLUDED
#define   CORE_ASSET_MANAGER_HPP_INCLUDED

#include <map>
#include <typeinfo>
#include <GQE/Core/interfaces/TAssetHandler.hpp>
#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// Provides centralized game asset manager class for managing game assets.
  class GQE_API AssetManager
  {
    public:
      /**
       * AssetManager constructor
       */
      AssetManager();

      /**
       * AssetManager deconstructor
       */
      virtual ~AssetManager();

      /**
       * GetHandler is responsible for returning an TAssetHandler derived
       * class that was previously registered under typeid(TYPE).name() of the
       * TYPE provided which can then be used to obtain Asset references by
       * Asset ID.
       */
      template<class TYPE>
      TAssetHandler<TYPE>& GetHandler() const
      {
        // The TAssetHandler<TYPE> derived class that will be returned
        TAssetHandler<TYPE>* anResult = NULL;

        // Iterator to the asset if found
        std::map<const typeAssetHandlerID, IAssetHandler*>::const_iterator iter;

        // Try to find the asset using theAssetID as the key
        iter = mHandlers.find(typeid(TYPE).name());

        // Found asset? increment the count and return the reference
        if(iter != mHandlers.end())
        {
          // Cast the IAssetHandler address found into TAssetHandler
          anResult = static_cast<TAssetHandler<TYPE>*>(iter->second);
        }

        // Make sure we aren't returning NULL at this point
        if(anResult == NULL)
        {
          FLOG(StatusAppMissingAsset) << "AssetManager::GetHandler("
            << typeid(TYPE).name() << ") not found!" << std::endl;
        }

        // Return the TAssetHandler addres or Null if something went wrong
        return *anResult;
      }

      /**
       * GetHandler is responsible for returning an IAssetHandler derived
       * class that was previously registered under theAssetHandlerID provided
       * which can then be used to obtain Asset references by Asset ID.
       * @param[in] theAssetHandlerID to retrieve
       */
      IAssetHandler& GetHandler(const typeAssetHandlerID theAssetHandlerID) const;

      /**
       * RegisterHandler is responsible for registering an IAssetHandler
       * derived class with the AssetManager. These handlers are used to manage
       * various asset types used by game states and other entities.
       * @param[in] theAssetHandler pointer to register
       */
      void RegisterHandler(IAssetHandler* theAssetHandler);

      /**
       * LoadAllAssets is responsible for loading all unloaded assets for every
       * IAssetHandler derived class registered, typically from the
       * IState::DoInit() method.
       * @return true if all assets load successfully, false otherwise
       */
      bool LoadAllAssets(void);

    private:
      // Constants
      ///////////////////////////////////////////////////////////////////////////

      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Map to hold all IAssetHandler derived classes that manage assets
      std::map<const typeAssetHandlerID, IAssetHandler*> mHandlers;

      /**
       * AssetManager copy constructor is private because we do not allow copies
       * of our class
       */
      AssetManager(const AssetManager&); // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our class
       */
      AssetManager& operator=(const AssetManager&); // Intentionally undefined
  }; // class AssetManager
} // namespace GQE

#endif // CORE_ASSET_MANAGER_HPP_INCLUDED

/**
 * @class GQE::AssetManager
 * @ingroup Core
 * The AssetManager class provides a central class for managing game
 * application assets.  Assets might include but are not limited to:
 * Fonts, Images, Music, Sounds, etc.  The AssetManager collects a
 * list of requested game assets and allows the background loading of
 * these assets as necessary.  The AssetManager can also load these
 * assets in the foreground if specified.  The AssetManager also
 * manages the removal of shared assets as soon as it becomes clear that
 * the game asset is no longer in use by using internal reference counts
 * for each game asset requested.
 *
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
