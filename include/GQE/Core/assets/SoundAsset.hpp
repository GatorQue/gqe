/**
 * Provides the Sound Asset type used by the AssetManager in the GQE namespace
 * which is responsible for providing the Asset management facilities for the
 * App base class in the GQE core library.
 *
 * @file include/GQE/Core/assets/SoundAsset.hpp
 * @author Ryan Lindeman
 * @date 20100724 - Initial Release
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Change to system include style
 * @date 20110627 - Removed extra ; from namespace
 * @date 20120512 - Use new RAII Asset and Asset Handler management style
 * @date 20120514 - Add default constructor for missing Asset ID at construction
 */
#ifndef   CORE_SOUND_ASSET_HPP_INCLUDED
#define   CORE_SOUND_ASSET_HPP_INCLUDED

#include <SFML/Audio.hpp>
#include <GQE/Core/interfaces/TAsset.hpp>
#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// Provides the Sound asset class
  class GQE_API SoundAsset : public TAsset<sf::SoundBuffer>
  {
    public:
      /**
       * SoundAsset default constructor is used when you don't know the asset
       * filename until later.
       */
      SoundAsset();

      /**
       * SoundAsset constructor
       * @param[in] theFilename to use when loading this asset
       * @param[in] theLoadFlag indicating immediate loading of asset
       */
      SoundAsset(std::string theFilename, bool theLoadFlag = false);

      /**
       * SoundAsset deconstructor
       */
      virtual ~SoundAsset();

    protected:

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
  }; // class SoundAsset
} // namespace GQE

#endif // CORE_SOUND_ASSET_HPP_INCLUDED

/**
 * @class GQE::SoundAsset
 * @ingroup Core
 * The SoundAsset class is a wrapper around the SFML SoundBuffer
 * class and is used by the AssetManager to keep track of and provide
 * SFML SoundBuffers to game states and other entities.
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
