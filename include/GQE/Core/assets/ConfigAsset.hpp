/**
 * Provides the Config Asset type used by the AssetManager in the GQE namespace
 * which is responsible for providing the Asset management facilities for the
 * App base class in the GQE core library.
 *
 * @file include/GQE/Core/assets/ConfigAsset.hpp
 * @author Ryan Lindeman
 * @date 20110218 - Initial Release
 * @date 20110218 - Change to system include style
 * @date 20110611 - Add abreviated comment for Docs
 * @date 20110627 - Removed extra ; from namespace
 */
#ifndef   CORE_CONFIG_ASSET_HPP_INCLUDED
#define   CORE_CONFIG_ASSET_HPP_INCLUDED

#include <GQE/Core/interfaces/TAsset.hpp>
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/classes/ConfigReader.hpp>

namespace GQE
{
  /// Provides the ConfigReader asset class
  class ConfigAsset : public TAsset<ConfigReader>
  {
    public:
      /**
       * ConfigAsset constructor
       * @param[in] theFilename to use when loading this asset
       * @param[in] theStyle to use when loading this asset
       */
      ConfigAsset(std::string theFilename, AssetLoadingStyle theStyle = AssetLoadStyleBackground);

      /**
       * ConfigAsset deconstructor
       */
      virtual ~ConfigAsset();

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
  }; // class ConfigAsset
} // namespace GQE

#endif // CORE_CONFIG_ASSET_HPP_INCLUDED

/**
 * @class GQE::ConfigAsset
 * @ingroup Core
 * The ConfigAsset class is a wrapper around the ConfigReader class and is used
 * by the AssetManager to keep track of and provide access to config settings
 * to game states and other entities.
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
