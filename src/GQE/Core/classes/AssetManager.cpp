/**
 * Provides the AssetManager class in the GQE namespace which is responsible
 * for providing the Asset management facilities to the App base class used in
 * the GQE core library.
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
 */
 
#include <assert.h>
#include <stddef.h>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/classes/AssetManager.hpp>
#include <GQE/Core/classes/App.hpp>
#include <GQE/Core/assets/ConfigAsset.hpp>
#include <GQE/Core/assets/FontAsset.hpp>
#include <GQE/Core/assets/ImageAsset.hpp>
#include <GQE/Core/assets/MusicAsset.hpp>
#include <GQE/Core/assets/SoundAsset.hpp>
 
namespace GQE
{
  AssetManager::AssetManager() :
    mApp(NULL),
    mBackgroundLoading(false),
    mBackgroundThread(NULL)
  {
    ILOGM("AssetManager::ctor()");

    // Create our background loading thread for use later
    mBackgroundThread = new(std::nothrow) sf::Thread(&AssetManager::BackgroundLoop, this);
  }
 
  AssetManager::~AssetManager()
  {
    ILOGM("AssetManager::dtor()");
 
    if(true == mBackgroundLoading)
    {
      // Wait for thread to finish first
      mBackgroundThread->Wait();
    }
 
    // Cleanup after ourselves
    DeleteConfigs();
    DeleteFonts();
    DeleteImages();
    DeleteMusic();
    DeleteSounds();
 
    // Clear pointers we don't need anymore
    mApp = NULL;
  }
 
  void AssetManager::RegisterApp(App* theApp)
  {
    // Check that our pointer is good
    assert(NULL != theApp && "AssetManager::RegisterApp() theApp pointer provided is bad");
 
    // Make a note of the pointer
    assert(NULL == mApp && "AssetManager::RegisterApp() theApp pointer was already registered");
    mApp = theApp;
  }
 
  void AssetManager::LoadAssets(bool theBackgroundFlag)
  {
    // Background loading thread
    if(true == theBackgroundFlag && false == mBackgroundLoading)
    {
      ILOGM("AssetManager::LoadAssets() starting background loading thread");
 
      // Launch the background loading thread
      mBackgroundThread->Launch();
    }
    // Foreground loading thread
    else
    {
      ILOGM("AssetManager::LoadAssets() starting foreground loading thread");

      // Load all the configs
      LoadConfigs(AssetLoadStyleForeground);

      // Load all the fonts
      LoadFonts(AssetLoadStyleForeground);

      // Load all the images
      LoadImages(AssetLoadStyleForeground);

      // Load all the music
      LoadMusic(AssetLoadStyleForeground);

      // Load all the sounds
      LoadSounds(AssetLoadStyleForeground);
    }
  }

  bool AssetManager::IsLoading(void)
  {
    return mBackgroundLoading;
  }

  ConfigAsset* AssetManager::AddConfig(
    const typeAssetID theAssetID,
    const std::string theFilename,
    AssetLoadingStyle theStyle)
  {
    // ConfigAsset result
    ConfigAsset* result = NULL;

    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();

    // Check for a duplicate asset using theAssetID as the key
    std::map<const typeAssetID, ConfigAsset*>::iterator iter;
    iter = mConfigs.find(theAssetID);
    if(iter == mConfigs.end())
    {
      // Create the asset since it wasn't found
      result = new (std::nothrow) ConfigAsset(theFilename, theStyle);
      assert(NULL != result && "AssetManager::AddConfig() unable to allocate memory");

      // Register our App pointer with this asset
      result->RegisterApp(mApp);

      // Handle Immediate loading style
      if(AssetLoadStyleImmediate == theStyle)
      {
        result->LoadAsset();
      }

      // Add the asset to the map of available assets
      mConfigs.insert(std::pair<const typeAssetID, ConfigAsset*>(theAssetID, result));
      
      ILOG() << "AssetManager::AddConfig(" << theAssetID << ") added" << std::endl;
    }
    else
    {
      // Return the previous asset that was added instead of creating a duplicate
      result = iter->second;

      ILOG() << "AssetManager::AddConfig(" << theAssetID << ") returning existing" << std::endl;
    }

    // Make sure we are actually returning a good pointer
    assert(NULL != result && "AssetManager::AddConfig() bad return result");

    // Increment our references to this asset
    result->AddReference();

    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();

    // Return our results
    return result;
  }

  void AssetManager::DeleteConfigs(void)
  {
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();

    // Delete all configs, since they are no longer needed
    std::map<const typeAssetID, ConfigAsset*>::iterator iter;
    iter = mConfigs.begin();
    while(iter != mConfigs.end())
    {
      ConfigAsset* anAsset = iter->second;
      mConfigs.erase(iter++);
      delete anAsset;
    }

    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }

  ConfigAsset* AssetManager::GetConfig(const typeAssetID theAssetID)
  {
    ConfigAsset* result = NULL;
    
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();

    // Locate the asset using theAssetID as the key
    std::map<const typeAssetID, ConfigAsset*>::iterator iter;
    iter = mConfigs.find(theAssetID);
    if(iter != mConfigs.end())
    {
      // Get the asset found
      result = iter->second;
    }

    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();

    // Return the result found
    return result;
  }

  void AssetManager::LoadConfigs(AssetLoadingStyle theStyle)
  {
    assert(AssetLoadStyleFirst < theStyle && AssetLoadStyleLast > theStyle &&
           "AssetManager::LoadConfigs() invalid style provided");

    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();

    // Load all unloaded configs with theStyle specified
    std::map<const typeAssetID, ConfigAsset*>::iterator iter;
    iter = mConfigs.begin();
    while(iter != mConfigs.end())
    {
      if(false == iter->second->IsLoaded() &&
         theStyle == iter->second->GetLoadingStyle())
      {
        iter->second->LoadAsset();
      }
      iter++;
    }

    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }

  void AssetManager::UnloadConfig(const typeAssetID theAssetID)
  {
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();

    // Locate the asset using theAssetID as the key
    std::map<const typeAssetID, ConfigAsset*>::iterator iter;
    iter = mConfigs.find(theAssetID);
    if(iter != mConfigs.end())
    {
      // Drop our reference to this asset and unload if needed
      iter->second->DropReference();
    }

    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }

  FontAsset* AssetManager::AddFont(
    const typeAssetID theAssetID,
    const std::string theFilename,
    AssetLoadingStyle theStyle)
  {
    // FontAsset result
    FontAsset* result = NULL;

    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();

    // Check for a duplicate asset using theAssetID as the key
    std::map<const typeAssetID, FontAsset*>::iterator iter;
    iter = mFonts.find(theAssetID);
    if(iter == mFonts.end())
    {
      // Create the asset since it wasn't found
      result = new(std::nothrow) FontAsset(theFilename, theStyle);
      assert(NULL != result && "AssetManager::AddFont() unable to allocate memory");
 
      // Register our App pointer with this asset
      result->RegisterApp(mApp);
 
      // Handle Immediate loading style
      if(AssetLoadStyleImmediate == theStyle)
      {
        result->LoadAsset();
      }
 
      // Add the asset to the map of available assets
      mFonts.insert(std::pair<const typeAssetID, FontAsset*>(theAssetID, result));
 
      ILOG() << "AssetManager::AddFont(" << theAssetID << ") added" << std::endl;
    }
    else
    {
      // Return the previous asset that was added instead of creating a duplicate
      result = iter->second;
 
      ILOG() << "AssetManager::AddFont(" << theAssetID << ") returning existing" << std::endl;
    }
 
    // Make sure we are actually returning a good pointer
    assert(NULL != result && "AssetManager::AddFont() bad return result");
 
    // Increment our references to this asset
    result->AddReference();
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
 
    // Return our results
    return result;
  }
 
  void AssetManager::DeleteFonts(void)
  {
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Delete all fonts, since they are no longer needed
    std::map<const typeAssetID, FontAsset*>::iterator iter;
    iter = mFonts.begin();
    while(iter != mFonts.end())
    {
      FontAsset* anAsset = iter->second;
      mFonts.erase(iter++);
      delete anAsset;
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }
 
  FontAsset* AssetManager::GetFont(const typeAssetID theAssetID)
  {
    FontAsset* result = NULL;
 
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Locate the asset using theAssetID as the key
    std::map<const typeAssetID, FontAsset*>::iterator iter;
    iter = mFonts.find(theAssetID);
    if(iter != mFonts.end())
    {
      // Get the asset found
      result = iter->second;
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
 
    // Return the result found
    return result;
  }
 
  void AssetManager::LoadFonts(AssetLoadingStyle theStyle)
  {
    assert(AssetLoadStyleFirst < theStyle && AssetLoadStyleLast > theStyle &&
           "AssetManager::LoadFonts() invalid style provided");
 
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Load all unloaded fonts with theStyle specified
    std::map<const typeAssetID, FontAsset*>::iterator iter;
    iter = mFonts.begin();
    while(iter != mFonts.end())
    {
      if(false == iter->second->IsLoaded() &&
         theStyle == iter->second->GetLoadingStyle())
      {
        iter->second->LoadAsset();
      }
      iter++;
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }
 
  void AssetManager::UnloadFont(const typeAssetID theAssetID)
  {
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Locate the asset using theAssetID as the key
    std::map<const typeAssetID, FontAsset*>::iterator iter;
    iter = mFonts.find(theAssetID);
    if(iter != mFonts.end())
    {
      // Drop our reference to this asset and unload if needed
      iter->second->DropReference();
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }
 
  ImageAsset* AssetManager::AddImage(
    const typeAssetID theAssetID,
    const std::string theFilename,
    AssetLoadingStyle theStyle)
  {
    // Our return result
    ImageAsset* result = NULL;
 
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Check for a duplicate asset using theAssetID as the key
    std::map<const typeAssetID, ImageAsset*>::iterator iter;
    iter = mImages.find(theAssetID);
    if(iter == mImages.end())
    {
      // Create the asset since it wasn't found
      result = new(std::nothrow) ImageAsset(theFilename, theStyle);
      assert(NULL != result && "AssetManager::AddImage() unable to allocate memory");
 
      // Register our App pointer with this asset
      result->RegisterApp(mApp);
 
      // Handle Immediate loading style
      if(AssetLoadStyleImmediate == theStyle)
      {
        result->LoadAsset();
      }
 
      // Add the asset to the map of available assets
      mImages.insert(std::pair<const typeAssetID, ImageAsset*>(theAssetID, result));
 
      ILOG() << "AssetManager::AddImage(" << theAssetID << ") added" << std::endl;
    }
    else
    {
      // Return the previous asset that was added instead of creating a duplicate
      result = iter->second;
 
      ILOG() << "AssetManager::AddImage(" << theAssetID << ") returning existing" << std::endl;
    }
 
    // Make sure we are actually returning a good pointer
    assert(NULL != result && "AssetManager::AddImage() bad return result");
 
    // Increment our references to this asset
    result->AddReference();
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
 
    // Return our results
    return result;
  }
 
  void AssetManager::DeleteImages(void)
  {
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Delete all images, since they are no longer needed
    std::map<const typeAssetID, ImageAsset*>::iterator iter;
    iter = mImages.begin();
    while(iter != mImages.end())
    {
      ImageAsset* anAsset = iter->second;
      mImages.erase(iter++);
      delete anAsset;
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }
 
  ImageAsset* AssetManager::GetImage(const typeAssetID theAssetID)
  {
    ImageAsset* result = NULL;
 
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Locate the asset using theAssetID as the key
    std::map<const typeAssetID, ImageAsset*>::iterator iter;
    iter = mImages.find(theAssetID);
    if(iter != mImages.end())
    {
      // Get the asset found
      result = iter->second;
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
 
    // Return the result found
    return result;
  }
 
  sf::Sprite* AssetManager::GetSprite(const typeAssetID theAssetID)
  {
    sf::Sprite* result = NULL;
    ImageAsset* anImageAsset = GetImage(theAssetID);
    if(NULL != anImageAsset)
    {
      sf::Image* anImage = anImageAsset->GetAsset();
      assert(NULL != anImage && "AssetManager::GetSprite() failed to obtain image asset");
      result = new(std::nothrow) sf::Sprite(*anImage);
      assert(NULL != result && "AssetManager::GetSprite() unable to allocate memory");
    }
    else
    {
      FLOG(StatusAppMissingAsset) << "AssetManager::GetSprite(" << theAssetID << ") can't find image" << std::endl;
    }
 
    // Return our result
    return result;
  }
 
  void AssetManager::LoadImages(AssetLoadingStyle theStyle)
  {
    assert(AssetLoadStyleFirst < theStyle && AssetLoadStyleLast > theStyle &&
           "AssetManager::LoadImages() invalid style provided");
 
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Load all unloaded images with theStyle specified
    std::map<const typeAssetID, ImageAsset*>::iterator iter;
    iter = mImages.begin();
    while(iter != mImages.end())
    {
      if(false == iter->second->IsLoaded() &&
         theStyle == iter->second->GetLoadingStyle())
      {
        iter->second->LoadAsset();
      }
      iter++;
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }
 
  void AssetManager::UnloadImage(const typeAssetID theAssetID)
  {
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Locate the asset using theAssetID as the key
    std::map<const typeAssetID, ImageAsset*>::iterator iter;
    iter = mImages.find(theAssetID);
    if(iter != mImages.end())
    {
      // Drop our reference to this asset and unload if needed
      iter->second->DropReference();
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }
 
  MusicAsset* AssetManager::AddMusic(
    const typeAssetID theAssetID,
    const std::string theFilename,
    AssetLoadingStyle theStyle)
  {
    // Our return result
    MusicAsset* result = NULL;
 
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Check for a duplicate asset using theAssetID as the key
    std::map<const typeAssetID, MusicAsset*>::iterator iter;
    iter = mMusic.find(theAssetID);
    if(iter == mMusic.end())
    {
      // Create the asset since it wasn't found
      result = new(std::nothrow) MusicAsset(theFilename, theStyle);
      assert(NULL != result && "AssetManager::AddMusic() unable to allocate memory");
 
      // Register our App pointer with this asset
      result->RegisterApp(mApp);
 
      // Handle Immediate loading style
      if(AssetLoadStyleImmediate == theStyle)
      {
        result->LoadAsset();
      }
 
      // Add the asset to the map of available assets
      mMusic.insert(std::pair<const typeAssetID, MusicAsset*>(theAssetID, result));
 
      ILOG() << "AssetManager::AddMusic(" << theAssetID << ") added" << std::endl;
    }
    else
    {
      // Return the previous asset that was added instead of creating a duplicate
      result = iter->second;
 
      ILOG() << "AssetManager::AddMusic(" << theAssetID << ") returning existing" << std::endl;
    }
 
    // Make sure we are actually returning a good pointer
    assert(NULL != result && "AssetManager::AddMusic() bad return result");
 
    // Increment our references to this asset
    result->AddReference();
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
 
    // Return our results
    return result;
  }
 
  void AssetManager::DeleteMusic(void)
  {
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Delete all music, since they are no longer needed
    std::map<const typeAssetID, MusicAsset*>::iterator iter;
    iter = mMusic.begin();
    while(iter != mMusic.end())
    {
      MusicAsset* anAsset = iter->second;
      mMusic.erase(iter++);
      delete anAsset;
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }
 
  MusicAsset* AssetManager::GetMusic(const typeAssetID theAssetID)
  {
    MusicAsset* result = NULL;
 
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Locate the asset using theAssetID as the key
    std::map<const typeAssetID, MusicAsset*>::iterator iter;
    iter = mMusic.find(theAssetID);
    if(iter != mMusic.end())
    {
      // Get the asset found
      result = iter->second;
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
 
    // Return the result found
    return result;
  }
 
  void AssetManager::LoadMusic(AssetLoadingStyle theStyle)
  {
    assert(AssetLoadStyleFirst < theStyle && AssetLoadStyleLast > theStyle &&
           "AssetManager::LoadMusic() invalid style provided");
 
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Load all unloaded music with theStyle specified
    std::map<const typeAssetID, MusicAsset*>::iterator iter;
    iter = mMusic.begin();
    while(iter != mMusic.end())
    {
      if(false == iter->second->IsLoaded() &&
         theStyle == iter->second->GetLoadingStyle())
      {
        iter->second->LoadAsset();
      }
      iter++;
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }
 
  void AssetManager::UnloadMusic(const typeAssetID theAssetID)
  {
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Locate the asset using theAssetID as the key
    std::map<const typeAssetID, MusicAsset*>::iterator iter;
    iter = mMusic.find(theAssetID);
    if(iter != mMusic.end())
    {
      // Drop our reference to this asset and unload if needed
      iter->second->DropReference();
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }
 
  SoundAsset* AssetManager::AddSound(
    const typeAssetID theAssetID,
    const std::string theFilename,
    AssetLoadingStyle theStyle)
  {
    // Our return result
    SoundAsset* result = NULL;
 
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Check for a duplicate asset using theAssetID as the key
    std::map<const typeAssetID, SoundAsset*>::iterator iter;
    iter = mSounds.find(theAssetID);
    if(iter == mSounds.end())
    {
      // Create the asset since it wasn't found
      result = new(std::nothrow) SoundAsset(theFilename, theStyle);
      assert(NULL != result && "AssetManager::AddSound() unable to allocate memory");
 
      // Register our App pointer with this asset
      result->RegisterApp(mApp);
 
      // Handle Immediate loading style
      if(AssetLoadStyleImmediate == theStyle)
      {
        result->LoadAsset();
      }
 
      // Add the asset to the map of available assets
      mSounds.insert(std::pair<const typeAssetID, SoundAsset*>(theAssetID, result));
 
      ILOG() << "AssetManager::AddSound(" << theAssetID << ") added" << std::endl;
    }
    else
    {
      // Return the previous asset that was added instead of creating a duplicate
      result = iter->second;
 
      ILOG() << "AssetManager::AddSound(" << theAssetID << ") returning existing" << std::endl;
    }
 
    // Make sure we are actually returning a good pointer
    assert(NULL != result && "AssetManager::AddSound() bad return result");
 
    // Increment our references to this asset
    result->AddReference();
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
 
    // Return our results
    return result;
  }
 
  void AssetManager::DeleteSounds(void)
  {
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Delete all sounds, since they are no longer needed
    std::map<const typeAssetID, SoundAsset*>::iterator iter;
    iter = mSounds.begin();
    while(iter != mSounds.end())
    {
      SoundAsset* anAsset = iter->second;
      mSounds.erase(iter++);
      delete anAsset;
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }
 
  SoundAsset* AssetManager::GetSound(const typeAssetID theAssetID)
  {
    SoundAsset* result = NULL;
 
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Locate the asset using theAssetID as the key
    std::map<const typeAssetID, SoundAsset*>::iterator iter;
    iter = mSounds.find(theAssetID);
    if(iter != mSounds.end())
    {
      // Get the asset found
      result = iter->second;
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
 
    // Return the result found
    return result;
  }
 
  sf::Sound* AssetManager::GetSoundPlayer(const typeAssetID theAssetID)
  {
    sf::Sound* result = NULL;
    SoundAsset* anSoundAsset = GetSound(theAssetID);
    if(NULL != anSoundAsset)
    {
      sf::SoundBuffer* anSoundBuffer = anSoundAsset->GetAsset();
      assert(NULL != anSoundBuffer && "AssetManager::GetSoundPlayer() failed to obtain sound buffer asset");
      result = new (std::nothrow) sf::Sound(*anSoundBuffer);
      assert(NULL != result && "AssetManager::GetSoundPlayer() unable to allocate memory");
    }
    else
    {
      FLOG(StatusAppMissingAsset) << "AssetManager::GetSoundPlayer(" << theAssetID << ") can't find sound" << std::endl;
    }

    // Return our result
    return result;
  }

  void AssetManager::LoadSounds(AssetLoadingStyle theStyle)
  {
    assert(AssetLoadStyleFirst < theStyle && AssetLoadStyleLast > theStyle &&
           "AssetManager::LoadSounds() invalid style provided");
 
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Load all unloaded sounds with theStyle specified
    std::map<const typeAssetID, SoundAsset*>::iterator iter;
    iter = mSounds.begin();
    while(iter != mSounds.end())
    {
      if(false == iter->second->IsLoaded() &&
         theStyle == iter->second->GetLoadingStyle())
      {
        iter->second->LoadAsset();
      }
      iter++;
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }
 
  void AssetManager::UnloadSound(const typeAssetID theAssetID)
  {
    // Obtain a lock before trying to access the asset maps
    mBackgroundMutex.Lock();
 
    // Locate the asset using theAssetID as the key
    std::map<const typeAssetID, SoundAsset*>::iterator iter;
    iter = mSounds.find(theAssetID);
    if(iter != mSounds.end())
    {
      // Drop our reference to this asset and unload if needed
      iter->second->DropReference();
    }
 
    // Release the lock after accessing the asset maps
    mBackgroundMutex.Unlock();
  }
 
  void AssetManager::BackgroundLoop(void* theAssetManager)
  {
    AssetManager* anManager = static_cast<AssetManager*>(theAssetManager);
    assert(NULL != anManager && "AssetManager::BackgroundLoop() invalid pointer provided");
 
    // Set our running flag
    anManager->mBackgroundLoading = true;
 
    // Load all the configs
    anManager->LoadConfigs(AssetLoadStyleBackground);

    // Load all the fonts
    anManager->LoadFonts(AssetLoadStyleBackground);
 
    // Load all the images
    anManager->LoadImages(AssetLoadStyleBackground);
 
    // Load all the music
    anManager->LoadMusic(AssetLoadStyleBackground);
 
    // Load all the sounds
    anManager->LoadSounds(AssetLoadStyleBackground);
 
    // Set our running flag
    anManager->mBackgroundLoading = false;
  }
} // namespace GQE

/**
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
