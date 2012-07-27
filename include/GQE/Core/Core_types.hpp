/**
 * Provides the GQE Core typedef's and forward class declarations.
 *
 * @file include/GQE/Core/Core_types.hpp
 * @author Ryan Lindeman
 * @date 20100710 - Initial Release
 * @date 20110118 - Add types used by ConfigReader.
 * @date 20110125 - Fix string compare issues
 * @date 20110127 - Changed name from GQE_types.hpp to Core_types.hpp
 * @date 20110218 - Added ConfigAsset to forward declarations
 * @date 20110523 - Readded NameValue types
 * @date 20110611 - Add new logging classes and new Log macros
 * @date 20110627 - Removed extra , from enum and ; from namespace
 * @date 20120426 - Remove gLogger variable (moved to ILogger::gInstance)
 * @date 20120428 - Added new IAsset and IAssetHandler types
 * @date 20120630 - Added new GraphicRange enumeration
 * @date 20120702 - Add new EventManager and Event ID typedef
 * @date 20120720 - Moved PropertyManager to Core library from Entity library
 */
#ifndef   GQE_CORE_TYPES_HPP_INCLUDED
#define   GQE_CORE_TYPES_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>
#include <GQE/Config.hpp>

#define INFO_LEVEL   0  ///< Defines the value for GQE::SeverityInfo
#define WARN_LEVEL   1  ///< Defines the value for GQE::SeverityWarning
#define ERROR_LEVEL  2  ///< Defines the value for GQE::SeverityError
#define FATAL_LEVEL  3  ///< Defines the value for GQE::Severity
#define NO_LOG_LEVEL 4  ///< Defines the value for no logging

namespace GQE
{
  /// Enumeration of recommended Graphic Range
  enum GraphicRange
  {
    LowRange  = 0, ///< Recommend using LowRange graphics (32x32 pixels)
    MidRange  = 1, ///< Recommend using MidRange graphics (64x64 pixels)
    HighRange = 2  ///< Recommend using HighRange graphics (128x128 pixels)
  };

  /// Enumeration of AssetLoadTime
  enum AssetLoadTime
  {
    AssetLoadNow   = 0, ///< Load the asset now
    AssetLoadLater = 1  ///< Load the asset later
  };

  /// Enumeration of AssetDropTime
  enum AssetDropTime
  {
    AssetDropUnspecified = 0, ///< Drop/unload time unspecified, use default AtZero
    AssetDropAtZero      = 1, ///< Drop/unload when count reaches 0
    AssetDropAtExit      = 2  ///< Drop/unload when program exits (deconstructor)
  };

  /// Enumeration of all Asset loading techniques
  enum AssetLoadStyle
  {
    AssetLoadFromUnknown = 0, ///< Error condition returned by GetLoadStyle
    AssetLoadFromFile    = 1, ///< Load the asset from a file
    AssetLoadFromMemory  = 2, ///< Load the asset from memory
    AssetLoadFromNetwork = 3  ///< Load the asset from the network
  };

  /// Enumeration of all Logging severity types
  enum SeverityType
  {
    // Logger severity types range from 0 to 5
    SeverityInfo          = INFO_LEVEL,   ///< Informational severity type
    SeverityWarning       = WARN_LEVEL,   ///< Warning severity type
    SeverityError         = ERROR_LEVEL,  ///< Error severity type
    SeverityFatal         = FATAL_LEVEL   ///< Fatal severity type
  };

  /// Status Enumeration for Status Return values
  enum StatusType
  {
    // Values from -99 to 99 are common Error and Good status responses
    StatusAppMissingAsset = -4,  ///< Application failed due to missing asset file
    StatusAppStackEmpty   = -3,  ///< Application States stack is empty
    StatusAppInitFailed   = -2,  ///< Application initialization failed
    StatusError           = -1,  ///< General error status response
    StatusAppOK           =  0,  ///< Application quit without error
    StatusNoError         =  0,  ///< General no error status response
    StatusFalse           =  0,  ///< False status response
    StatusTrue            =  1,  ///< True status response
    StatusOK              =  1   ///< OK status response

    // Values from +-100 to +-199 are reserved for File status responses
  };

  // Forward declare GQE core interfaces provided
  class IApp;
  class IAssetHandler;
  class IEvent;
  class ILogger;
  class IProperty;
  class IState;

  // Forward declare GQE core classes provided
  class AssetManager;
  class ConfigReader;
  class EventManager;
  class PropertyManager;
  class StateManager;

  // Forward declare GQE core assets provided
  class ConfigAsset;
  class ConfigHandler;
  class DummyHandler;
  class FontAsset;
  class FontHandler;
  class ImageAsset;
  class ImageHandler;
  class MusicAsset;
  class MusicHandler;
  class SoundAsset;
  class SoundHandler;

  // Forward declare GQE core loggers provided
  class FatalLogger;
  class FileLogger;
  class ScopeLogger;
  class StringLogger;

  // Forward declare GQE core states provided
  class MenuState;
  class SplashState;

  /// Declare Asset ID typedef which is used for identifying Asset objects
  typedef std::string typeAssetID;

  /// Declare Asset Handler ID typedef which is used for identifying Asset Handler objects
  typedef std::string typeAssetHandlerID;

  /// Declare Event ID typedef which is used for identifying Event objects
  typedef std::string typeEventID;

  /// Declare Property ID typedef which is used for identifying Properties
  typedef std::string typePropertyID;

  /// Declare State ID typedef which is used for identifying State objects
  typedef std::string typeStateID;

  /// Declare NameValue typedef which is used for config section maps
  typedef std::map<const std::string, const std::string> typeNameValue;

  /// Declare NameValueIter typedef which is used for name,value pair maps
  typedef std::map<const std::string, const std::string>::iterator typeNameValueIter;
} // namespace GQE
#endif // GQE_CORE_TYPES_HPP_INCLUDED

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
