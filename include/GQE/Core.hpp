/**
 * Provides the GQE namespace includes for the GQE Core engine includes and
 * variables.
 *
 * @file include/GQE/Core.hpp
 * @author Ryan Lindeman
 * @date 20100710 - Initial Release
 * @date 20110118 - Added ConfigReader and StateManager includes.
 * @date 20110127 - Changed name from GQE.hpp to Core.hpp and moved to include
 * @date 20110218 - Added ConfigAsset to Core includes
 * @date 20110906 - Added new StringUtil include file
 * @date 20120512 - Add new Asset Handler classes
 * @date 20120702 - Add new EventManager and IEvent classes
 * @date 20120720 - Moved PropertyManager to Core library from Entity library
 */
#ifndef   GQE_CORE_HPP_INCLUDED
#define   GQE_CORE_HPP_INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Headers
/////////////////////////////////////////////////////////////////////////////
#include <GQE/Config.hpp>

// GQE Core includes
#include <GQE/Core/assets/ConfigAsset.hpp>
#include <GQE/Core/assets/ConfigHandler.hpp>
#include <GQE/Core/assets/FontAsset.hpp>
#include <GQE/Core/assets/FontHandler.hpp>
#include <GQE/Core/assets/ImageAsset.hpp>
#include <GQE/Core/assets/ImageHandler.hpp>
#include <GQE/Core/assets/MusicAsset.hpp>
#include <GQE/Core/assets/MusicHandler.hpp>
#include <GQE/Core/assets/SoundAsset.hpp>
#include <GQE/Core/assets/SoundHandler.hpp>
#include <GQE/Core/classes/AssetManager.hpp>
#include <GQE/Core/classes/ConfigReader.hpp>
#include <GQE/Core/classes/EventManager.hpp>
#include <GQE/Core/classes/PropertyManager.hpp>
#include <GQE/Core/classes/StatManager.hpp>
#include <GQE/Core/classes/StateManager.hpp>
#include <GQE/Core/interfaces/IApp.hpp>
#include <GQE/Core/interfaces/IAssetHandler.hpp>
#include <GQE/Core/interfaces/IEvent.hpp>
#include <GQE/Core/interfaces/ILogger.hpp>
#include <GQE/Core/interfaces/IProperty.hpp>
#include <GQE/Core/interfaces/IState.hpp>
#include <GQE/Core/interfaces/TAsset.hpp>
#include <GQE/Core/interfaces/TAssetHandler.hpp>
#include <GQE/Core/interfaces/TEvent.hpp>
#include <GQE/Core/interfaces/TProperty.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/loggers/FatalLogger.hpp>
#include <GQE/Core/loggers/FileLogger.hpp>
#include <GQE/Core/loggers/ScopeLogger.hpp>
#include <GQE/Core/loggers/StringLogger.hpp>
#include <GQE/Core/loggers/onullstream>
#include <GQE/Core/states/SplashState.hpp>
#include <GQE/Core/utils/StringUtil.hpp>

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
