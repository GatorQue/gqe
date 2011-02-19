/**
 * Provides the GQE namespace includes for the GQE Core engine includes and variables.
 *
 * @file include/GQE/Core.hpp
 * @author Ryan Lindeman
 * @date 20100710 - Initial Release
 * @date 20110118 - Added ConfigReader and StateManager includes.
 * @date 20110127 - Changed name from GQE.hpp to Core.hpp and moved to include
 * @date 20110218 - Added ConfigAsset to Core includes
 */
#ifndef   GQE_CORE_HPP_INCLUDED
#define   GQE_CORE_HPP_INCLUDED
 
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <GQE/Config.hpp>

// GQE Core includes
#include "GQE/Core/assets/ConfigAsset.hpp"
#include "GQE/Core/assets/FontAsset.hpp"
#include "GQE/Core/assets/ImageAsset.hpp"
#include "GQE/Core/assets/MusicAsset.hpp"
#include "GQE/Core/assets/SoundAsset.hpp"
#include "GQE/Core/classes/App.hpp"
#include "GQE/Core/classes/AssetManager.hpp"
#include "GQE/Core/classes/ConfigReader.hpp"
#include "GQE/Core/classes/StateManager.hpp"
#include "GQE/Core/interfaces/IState.hpp"
#include "GQE/Core/interfaces/TAsset.hpp"
#include "GQE/Core/states/MenuState.hpp"
#include "GQE/Core/states/SplashState.hpp"
 
namespace GQE
{
  /// Core module global variable declarations
}; // namespace GQE

#endif // GQE_CORE_HPP_INCLUDED

/**
 * @defgroup Core GQE Core module
 * The GQE Core module/library provides all basic classes used by all game
 * engines in the GQE collection of modules/libraries. Each game engine genre
 * will be built on top of the GQE Core module/library.
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
