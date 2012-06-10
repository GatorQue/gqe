/**
 * Provides the MusicHandler class used by the AssetManager to manage all
 * sf::Music assets for the application.
 *
 * @file src/GQE/Core/assets/MusicHandler.cpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */
 
#include <GQE/Core/assets/MusicHandler.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
 
namespace GQE
{
  MusicHandler::MusicHandler() :
    TAssetHandler<sf::Music>()
  {
    ILOG() << "MusicHandler::ctor()" << std::endl;
  }
 
  MusicHandler::~MusicHandler()
  {
    ILOG() << "MusicHandler::dtor()" << std::endl;
  }

  bool MusicHandler::LoadFromFile(const typeAssetID theAssetID, sf::Music& theAsset)
  {
    // Start with a return result of false
    bool anResult = false;

    // Retrieve the filename for this asset
    std::string anFilename = GetFilename(theAssetID);

    // Was a valid filename found? then attempt to load the asset from anFilename
    if(anFilename.length() > 0)
    {
      // Load the asset from a file
#if (SFML_VERSION_MAJOR < 2)
      anResult = theAsset.OpenFromFile(anFilename);
#else
      anResult = theAsset.openFromFile(anFilename);
#endif
    }
    else
    {
      ELOG() << "MusicHandler::LoadFromFile(" << theAssetID
        << ") No filename provided!" << std::endl;
    }

    // Return anResult of true if successful, false otherwise
    return anResult;
  }

  bool MusicHandler::LoadFromMemory(const typeAssetID theAssetID, sf::Music& theAsset)
  {
    // Start with a return result of false
    bool anResult = false;

    // TODO: Retrieve the const char* pointer to load data from
    const char* anData = NULL;
    // TODO: Retrieve the size in bytes of the font to load from memory
    size_t anDataSize = 0;

    // Try to obtain the font from the memory location specified
    if(NULL != anData && anDataSize > 0)
    {
      // Load the image from the memory location specified
#if (SFML_VERSION_MAJOR < 2)
      anResult = theAsset.OpenFromMemory(anData, anDataSize);
#else
      anResult = theAsset.openFromMemory(anData, anDataSize);
#endif
    }
    else
    {
      ELOG() << "MusicHandler::LoadFromMemory(" << theAssetID
        << ") Bad memory location or size!" << std::endl;
    }

    // Return anResult of true if successful, false otherwise
    return anResult;
  }

  bool MusicHandler::LoadFromNetwork(const typeAssetID theAssetID, sf::Music& theAsset)
  {
    // Start with a return result of false
    bool anResult = false;

    // TODO: Add load from network for this asset

    // Return anResult of true if successful, false otherwise
    return anResult;
  }
} // namespace GQE
 
/**
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
