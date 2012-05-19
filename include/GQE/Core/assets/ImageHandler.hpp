/**
 * Provides the ImageHandler class used by the AssetManager to manage all
 * sf::Image assets for the application.
 *
 * @file include/GQE/Core/assets/ImageHandler.hpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */
#pragma once//added to make MSVC 2010 stop complaining.
#ifndef   CORE_IMAGE_HANDLER_HPP_INCLUDED
#define   CORE_IMAGE_HANDLER_HPP_INCLUDED
 
#include <SFML/Graphics.hpp>
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/interfaces/TAssetHandler.hpp>

namespace GQE
{
  /// Provides the ImageHandler class for managing sf::Image assets
  class GQE_API ImageHandler :
#if (SFML_VERSION_MAJOR < 2)
    public TAssetHandler<sf::Image>
#else
    public TAssetHandler<sf::Texture>
#endif
  {
  public:
    /**
     * ImageHandler constructor
     */
    ImageHandler();
 
    /**
     * ImageHandler deconstructor
     */
    virtual ~ImageHandler();
 
  protected:
    /**
     * LoadFromFile is responsible for loading theAsset from a file and must
     * be defined by the derived class since the interface for TYPE is
     * unknown at this stage.
     * @param[in] theAssetID of the asset to be loaded
     * @param[in] theAsset pointer to load
     * @return true if the asset was successfully loaded, false otherwise
     */
#if (SFML_VERSION_MAJOR < 2)
    virtual bool LoadFromFile(const typeAssetID theAssetID, sf::Image& theAsset);
#else
    virtual bool LoadFromFile(const typeAssetID theAssetID, sf::Texture& theAsset);
#endif

    /**
     * LoadFromMemory is responsible for loading theAsset from memory and
     * must be defined by the derived class since the interface for TYPE is
     * unknown at this stage.
     * @param[in] theAssetID of the asset to be loaded
     * @param[in] theAsset pointer to load
     * @return true if the asset was successfully loaded, false otherwise
     */
#if (SFML_VERSION_MAJOR < 2)
    virtual bool LoadFromMemory(const typeAssetID theAssetID, sf::Image& theAsset);
#else
    virtual bool LoadFromMemory(const typeAssetID theAssetID, sf::Texture& theAsset);
#endif

    /**
     * LoadFromNetwork is responsible for loading theAsset from network and
     * must be defined by the derived class since the interface for TYPE is
     * unknown at this stage.
     * @param[in] theAssetID of the asset to be loaded
     * @param[in] theAsset pointer to load
     * @return true if the asset was successfully loaded, false otherwise
     */
#if (SFML_VERSION_MAJOR < 2)
    virtual bool LoadFromNetwork(const typeAssetID theAssetID, sf::Image& theAsset);
#else
    virtual bool LoadFromNetwork(const typeAssetID theAssetID, sf::Texture& theAsset);
#endif

  private:
  }; // class ImageHandler
} // namespace GQE

#endif // CORE_IMAGE_MANAGER_HPP_INCLUDED

/**
 * @class GQE::ImageHandler
 * @ingroup Core
 * The ImageHandler class is used to reference count and manage all sf::Image
 * classes used in a GQE application.
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
