/**
 * Provides the Music Asset type used by the AssetManager in the GQE namespace
 * which is responsible for providing the Asset management facilities for the
 * App base class in the GQE core library.
 *
 * @file include/GQE/Core/assets/MusicAsset.hpp
 * @author Ryan Lindeman
 * @date 20100724 - Initial Release
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Change to system include style
 * @date 20110627 - Removed extra ; from namespace
 * @date 20120512 - Use new RAII Asset and Asset Handler management style
 * @date 20120514 - Add default constructor for missing Asset ID at construction
 * @date 20120615 - Adjust comments for default constructor
 */
#ifndef   CORE_MUSIC_ASSET_HPP_INCLUDED
#define   CORE_MUSIC_ASSET_HPP_INCLUDED

#include <SFML/Audio.hpp>
#include <GQE/Core/interfaces/TAsset.hpp>
#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// Provides the MusicAsset class
  class GQE_API MusicAsset : public TAsset<sf::Music>
  {
    public:
      /**
       * MusicAsset default constructor is used when you don't know theAssetID
       * until later. You must call the SetID method before calling the
       * GetAsset method to prevent getting the DummyAsset instead.
       */
      MusicAsset();

      /**
       * MusicAsset constructor
       * @param[in] theAssetID to uniquely identify this asset
       * @param[in] theLoadTime (Now, Later) of when to load this asset
       * @param[in] theLoadStyle (File, Mem, Network) to use when loading this asset
       * @param[in] theDropTime at (Zero, Exit) for when to unload this asset
       */
      MusicAsset(const typeAssetID theAssetID,
        AssetLoadTime theLoadTime = AssetLoadLater,
        AssetLoadStyle theLoadStyle = AssetLoadFromFile,
        AssetDropTime theDropTime = AssetDropAtZero);

      /**
       * MusicAsset deconstructor
       */
      virtual ~MusicAsset();

    protected:

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
  }; // class MusicAsset
} // namespace GQE

#endif // CORE_MUSIC_ASSET_HPP_INCLUDED

/**
 * @class GQE::MusicAsset
 * @ingroup Core
 * The MusicAsset class is a wrapper around the SFML Music class and is
 * used by the AssetManager to keep track of and provide SFML Music to
 * game states and other entities.
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
