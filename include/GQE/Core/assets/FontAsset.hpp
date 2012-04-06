/**
 * Provides the Font Asset type used by the AssetManager in the GQE namespace
 * which is responsible for providing the Asset management facilities for the
 * App base class in the GQE core library.
 *
 * @file include/GQE/Core/assets/FontAsset.hpp
 * @author Ryan Lindeman
 * @date 20100724 - Initial Release
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Change to system include style
 * @date 20110627 - Removed extra ; from namespace
 */
#ifndef   CORE_FONT_ASSET_HPP_INCLUDED
#define   CORE_FONT_ASSET_HPP_INCLUDED

#include <GQE/Core/interfaces/TAsset.hpp>
#include <GQE/Core/Core_types.hpp>
#include <SFML/Graphics.hpp>

namespace GQE
{
  /// Provides the Font asset class
  class GQE_API FontAsset : public TAsset<sf::Font>
  {
    public:
      /**
       * FontAsset constructor
       * @param[in] theFilename to use when loading this asset
       * @param[in] theStyle to use when loading this asset
       */
      FontAsset(std::string theFilename, AssetLoadingStyle theStyle = AssetLoadStyleBackground);

      /**
       * FontAsset deconstructor
       */
      virtual ~FontAsset();

      /**
       * LoadAsset is responsible for loading the Asset.
       */
      virtual void LoadAsset(void);

    protected:
      /**
       * UnloadAsset is responsible for destroying or unloading the Asset and
       * is called by FreeAsset.
       */
      virtual void UnloadAsset(void);

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
  }; // class FontAsset
} // namespace GQE

#endif // CORE_FONT_ASSET_HPP_INCLUDED

/**
 * @class GQE::FontAsset
 * @ingroup Core
 * The FontAsset class is a wrapper around the SFML Font class and is used
 * by the AssetManager to keep track of and provide SFML Fonts to game
 * states and other entities.
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
