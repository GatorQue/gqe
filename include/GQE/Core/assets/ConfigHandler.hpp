/**
 * Provides the ConfigHandler class used by the AssetManager to manage all
 * ConfigReader assets for the application.
 *
 * @file include/GQE/Core/assets/ConfigHandler.hpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */
#ifndef   CORE_CONFIG_HANDLER_HPP_INCLUDED
#define   CORE_CONFIG_HANDLER_HPP_INCLUDED
 
#include <GQE/Core/classes/ConfigReader.hpp>
#include <GQE/Core/interfaces/IAssetHandler.hpp>
#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// Provides the ConfigHandler class for managing ConfigReader assets
  class GQE_API ConfigHandler : public IAssetHandler
  {
  public:
    // Constants
    ///////////////////////////////////////////////////////////////////////////
    /// Default Asset Handler ID for this Asset Handler
    static const char* DEFAULT_ID;

    /**
     * ConfigHandler constructor
     */
    ConfigHandler();
 
    /**
     * ConfigHandler deconstructor
     */
    virtual ~ConfigHandler();
 
  protected:
    /**
     * AcquireAsset is responsible for creating an IAsset derived asset and
     * returning it to the caller.
     * @param[in] theAssetID of the asset to acquire
     * @return a pointer to the newly created asset
     */
    virtual void* AcquireAsset(const typeAssetID theAssetID);

    /**
     * GetDummyAsset is responsible for returning a pointer to a Dummy asset
     * which will be returned if AcquireAsset fails to create an asset or
     * GetReference() is called. This makes the system more stable since all
     * assets will have a valid reference pointer
     * @return pointer to a dummy asset
     */
    virtual void* GetDummyAsset(void);

    /**
     * LoadAsset is responsible for loading theAsset returned by AcquireAsset
     * and must provide a default implementation for all loading style types.
     * @param[in] theAsset pointer to load
     * @return true if the asset was successfully loaded, false otherwise
     */
    virtual bool LoadAsset(const typeAssetID theAssetID, void* theAsset);

  private:
    // Variables
    ///////////////////////////////////////////////////////////////////////////
    /// Dummy asset that will be returned if an asset can't be Acquired
    ConfigReader mDummyAsset;
  }; // class ConfigHandler
} // namespace GQE

#endif // CORE_CONFIG_MANAGER_HPP_INCLUDED

/**
 * @class GQE::ConfigHandler
 * @ingroup Core
 * The ConfigHandler class is used to reference count and manage all
 * ConfigReader classes used in a GQE application.
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
