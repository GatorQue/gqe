/**
 * Provides the AssetManager class in the GQE namespace which is responsible
 * for providing the Asset management facilities to the App base class used in
 * the GQE core library.
 *
 * @file include/GQE/Core/classes/AssetManager.hpp
 * @author Ryan Lindeman
 * @date 20100723 - Initial Release
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Added new Config asset type
 */
#ifndef   CORE_ASSET_MANAGER_HPP_INCLUDED
#define   CORE_ASSET_MANAGER_HPP_INCLUDED
 
#include <map>
#include <string>
#include "GQE/Core/Core_types.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
 
namespace GQE
{
  /// Provides centralized game asset manager class for managing game assets.
  class GQE_API AssetManager
  {
  public:
    /// Enumeration for all Asset Type values
    enum AssetType {
      FirstStandardAsset  = 0,  ///< First Standard Asset Type Value
      AssetConfig         = 1,  ///< Config file Asset Type
      AssetFont           = 2,  ///< Font Asset Type
      AssetImage          = 3,  ///< Image/Texture Asset Type
      AssetMusic          = 4,  ///< Background Music Asset Type
      AssetScript         = 5,  ///< Script Asset Type
      AssetSound          = 6,  ///< Sound Effect Asset Type
      AssetLevel          = 7,  ///< Level/Map Asset Type
      LastStandardAsset,        ///< Last Standard Asset Type Value
 
      // The following can be used for custom assets
      FirstCustomAsset    = 10, ///< First Custom Asset Type value
      AssetCustom1        = 11, ///< Custom Asset Type 1
      AssetCustom2        = 12, ///< Custom Asset Type 2
      AssetCustom3        = 13, ///< Custom Asset Type 3
      AssetCustom4        = 14, ///< Custom Asset Type 4
      AssetCustom5        = 15, ///< Custom Asset Type 5
      LastCustomAsset,          ///< Last Custom Asset Type Value
    };
 
    /**
     * AssetManager constructor
     */
    AssetManager();
 
    /**
     * AssetManager deconstructor
     */
    virtual ~AssetManager();
 
    /**
     * RegisterApp will register a pointer to the App class so it can be used
     * by the AssetManager for error handling and log reporting.
     * @param[in] theApp is a pointer to the App (or App derived) class
     */
    void RegisterApp(App* theApp);
 
    /**
     * LoadAssets will attempt to load all of the assets that have not been
     * loaded in either the foreground or background depending on
     * theBackgroundFlag provided.
     * @param[in] theBackgroundFlag means load assets in the background
     */
    void LoadAssets(bool theBackgroundFlag = false);
 
    /**
     * IsLoading() will return true if the background thread is currently
     * loading the Background loading style assets.
     * @return true if background thread is still running
     */
    bool IsLoading(void);

    /**
     * AddConfig will add a ConfigAsset object if the ConfigAsset object does
     * not yet exist, otherwise it will return a pointer to the existing
     * ConfigAsset.
     * @param[in] theAssetID is the ID for the asset to be added
     * @param[in] theFilename to use for loading this asset
     * @param[in] theStyle is the Loading style to use for this asset
     * @return pointer to the ConfigAsset that was added
     */
    ConfigAsset* AddConfig(
      const typeAssetID theAssetID,
      const std::string theFilename = "",
      AssetLoadingStyle theStyle = AssetLoadStyleBackground);

    /**
     * UnloadConfig will unload the Config asset specified by theAssetID.
     * @param[in] theAssetID is the ID for the ConfigAsset to unload
     */
    void UnloadConfig(const typeAssetID theAssetID);

    /**
     * GetConfig will retrieve the Config asset specified by theAssetID.
     * @param[in] theAssetID is the ID for the ConfigAsset to be retrieved
     * @return pointer to ConfigAsset or NULL if it doesn't yet exist
     */
    ConfigAsset* GetConfig(const typeAssetID theAssetID);

    /**
     * AddFont will add a FontAsset object if the FontAsset object does not
     * yet exist, otherwise it will return a pointer to the existing
     * FontAsset.
     * @param[in] theAssetID is the ID for the asset to be added
     * @param[in] theFilename to use for loading this asset
     * @param[in] theStyle is the Loading style to use for this asset
     * @return pointer to the FontAsset that was added
     */
    FontAsset* AddFont(
      const typeAssetID theAssetID,
      const std::string theFilename = "",
      AssetLoadingStyle theStyle = AssetLoadStyleBackground);
 
    /**
     * UnloadFont will unload the font asset specified by theAssetID.
     * @param[in] theAssetID is the ID for the FontAsset to unload
     */
    void UnloadFont(const typeAssetID theAssetID);
 
    /**
     * GetFont will retrieve the font asset specified by theAssetID.
     * @param[in] theAssetID is the ID for the FontAsset to be retrieved
     * @return pointer to FontAsset or NULL if it doesn't yet exist
     */
    FontAsset* GetFont(const typeAssetID theAssetID);
 
    /**
     * AddImage will add a ImageAsset object if the ImageAsset object does not
     * yet exist, otherwise it will return a pointer to the existing
     * ImageAsset.
     * @param[in] theAssetID is the ID for the asset to be added
     * @param[in] theFilename to use for loading this asset
     * @param[in] theStyle is the Loading style to use for this asset
     * @return pointer to the ImageAsset that was added
     */
    ImageAsset* AddImage(
      const typeAssetID theAssetID,
      const std::string theFilename = "",
      AssetLoadingStyle theStyle = AssetLoadStyleBackground);
 
    /**
     * GetImage will retrieve the image asset specified by theAssetID.
     * @param[in] theAssetID is the ID for the ImageAsset to be retrieved
     * @return pointer to ImageAsset or NULL if it doesn't yet exist
     */
    ImageAsset* GetImage(const typeAssetID theAssetID);
 
    /**
     * GetSprite will return a sprite object for the image asset specified by
     * theAssetID.  The caller is responsible for this object and must delete
     * this object when he is through with it.
     * @param[in] theAssetID is the ID for the ImageAsset to be retrieved
     * @return pointer to sf::Sprite or NULL if image was not found
     */
    sf::Sprite* GetSprite(const typeAssetID theAssetID);
 
    /**
     * UnloadImage will unload the image asset specified by theAssetID.
     * @param[in] theAssetID is the ID for the ImageAsset to unload
     */
    void UnloadImage(const typeAssetID theAssetID);
 
    /**
     * AddMusic will add a MusicAsset object if the MusicAsset object does not
     * yet exist, otherwise it will return a pointer to the existing
     * MusicAsset.
     * @param[in] theAssetID is the ID for the asset to be added
     * @param[in] theFilename to use for loading this asset
     * @param[in] theStyle is the Loading style to use for this asset
     * @return pointer to the MusicAsset that was added
     */
    MusicAsset* AddMusic(
      const typeAssetID theAssetID,
      const std::string theFilename = "",
      AssetLoadingStyle theStyle = AssetLoadStyleBackground);
 
    /**
     * GetMusic will retrieve the music asset specified by theAssetID.
     * @param[in] theAssetID is the ID for the MusicAsset to be retrieved
     * @return pointer to MusicAsset or NULL if it doesn't yet exist
     */
    MusicAsset* GetMusic(const typeAssetID theAssetID);
 
    /**
     * UnloadMusic will unload the music asset specified by theAssetID.
     * @param[in] theAssetID is the ID for the MusicAsset to unload
     */
    void UnloadMusic(const typeAssetID theAssetID);
 
    /**
     * AddSound will add a SoundAsset object if the SoundAsset object does not
     * yet exist, otherwise it will return a pointer to the existing
     * SoundAsset.
     * @param[in] theAssetID is the ID for the asset to be added
     * @param[in] theFilename to use for loading this asset
     * @param[in] theStyle is the Loading style to use for this asset
     * @return pointer to the FontAsset that was added
     */
    SoundAsset* AddSound(
      const typeAssetID theAssetID,
      const std::string theFilename = "",
      AssetLoadingStyle theStyle = AssetLoadStyleBackground);
 
    /**
     * GetSound will retrieve the sound asset specified by theAssetID.
     * @param[in] theAssetID is the ID for the SoundAsset to be retrieved
     * @return pointer to SoundAsset or NULL if it doesn't yet exist
     */
    SoundAsset* GetSound(const typeAssetID theAssetID);
 
    /**
     * UnloadSound will unload the sound asset specified by theAssetID.
     * @param[in] theAssetID is the ID for the SoundAsset to unload
     */
    void UnloadSound(const typeAssetID theAssetID);
 
  private:
    // Constants
    ///////////////////////////////////////////////////////////////////////////
 
    // Variables
    ///////////////////////////////////////////////////////////////////////////
    /// Pointer to the App class for error handling and logging
    App*                                      mApp;
    /// Boolean indicating that the Background loading thread is running
    bool                                      mBackgroundLoading;
    /// Background loading thread
    sf::Thread*                               mBackgroundThread;
    /// Background loading thread mutex
    sf::Mutex                                 mBackgroundMutex;
    /// Map to store all the Config assets
    std::map<const typeAssetID, ConfigAsset*> mConfigs;
    /// Map to store all the Font assets
    std::map<const typeAssetID, FontAsset*>   mFonts;
    /// Map to store all the Image/Texture assets
    std::map<const typeAssetID, ImageAsset*>  mImages;
    /// Map to store all the Sound assets
    std::map<const typeAssetID, SoundAsset*>  mSounds;
    /// Map to store all the Music assets
    std::map<const typeAssetID, MusicAsset*>  mMusic;
 
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
 
    /**
     * BackgroundLoop is the method that will be called when the background
     * thread is currently running.
     * @param[in] theAssetManager is a pointer to the AssetManager class
     */
    static void BackgroundLoop(void* theAssetManager);

    /**
     * DeleteConfigs will delete all added Config assets.
     */
    void DeleteConfigs(void);

    /**
     * LoadConfigs will load all the config that match theStyle specified.
     * @param[in] theStyle that equals the loading style of the unloaded assets
     */
    void LoadConfigs(AssetLoadingStyle theStyle);

    /**
     * DeleteFonts will delete all added font assets.
     */
    void DeleteFonts(void);
 
    /**
     * LoadFonts will load all the fonts that match theStyle specified.
     * @param[in] theStyle that equals the loading style of the unloaded assets
     */
    void LoadFonts(AssetLoadingStyle theStyle);
 
    /**
     * DeleteImages will delete all added image assets.
     */
    void DeleteImages(void);
 
    /**
     * LoadImages will load all the images that match theStyle specified.
     * @param[in] theStyle that equals the loading style of the unloaded assets
     */
    void LoadImages(AssetLoadingStyle theStyle);
 
    /**
     * DeleteMusic will delete all added music assets.
     */
    void DeleteMusic(void);
 
    /**
     * LoadMusic will load all the music that match theStyle specified.
     * @param[in] theStyle that equals the loading style of the unloaded assets
     */
    void LoadMusic(AssetLoadingStyle theStyle);
 
    /**
     * DeleteSound will delete all added sound assets.
     */
    void DeleteSounds(void);
 
    /**
     * LoadSounds will load all the sounds that match theStyle specified.
     * @param[in] theStyle that equals the loading style of the unloaded assets
     */
    void LoadSounds(AssetLoadingStyle theStyle);
 
  }; // class AssetManager
}; // namespace GQE
 
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
