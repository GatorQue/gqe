/**
 * Provides the DummyHandler class used by the AssetManager to when the
 * caller requests for an unknown or unregistered Asset handler.
 *
 * @file src/GQE/Core/assets/DummyHandler.cpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */
 
#include <GQE/Core/assets/DummyHandler.hpp>
#include <GQE/Core/interfaces/IApp.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
 
namespace GQE
{
  /// Default Asset Handler ID for this Asset Handler
  const char* DummyHandler::DEFAULT_ID = "dummy";

  DummyHandler::DummyHandler() :
    IAssetHandler(DummyHandler::DEFAULT_ID)
  {
    ILOG() << "DummyHandler::ctor()" << std::endl;
  }
 
  DummyHandler::~DummyHandler()
  {
    ILOG() << "DummyHandler::dtor()" << std::endl;
  }
 
  void* DummyHandler::AcquireAsset(const typeAssetID theAssetID)
  {
    return GetDummyAsset();
  }
      
  void* DummyHandler::GetDummyAsset(void)
  {
    ELOG() << "DummyHandler(" << GetID()
      << "):GetDummyAsset() Returning Dummy Asset." << std::endl;
    return &mDummyAsset;
  }

  bool DummyHandler::LoadAsset(const typeAssetID theAssetID, void* theAsset)
  {
    // Default to NOT loaded
    bool anResult = false;
    Uint32* anAsset = static_cast<Uint32*>(theAsset);

    if(NULL != anAsset)
    {
      FLOG(StatusAppMissingAsset) << "DummyHandler::LoadAsset(" << theAssetID
        << ") Unable to load dummy asset!" << std::endl;
    }
    else
    {
      // Log fatal error if our static cast failed!
      FLOG(StatusAppMissingAsset) << "DummyHandler::LoadAsset(" << theAssetID
        << ") Bad static cast!" << std::endl;
    }

    // Return anResult determined above or false for NOT loaded
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
