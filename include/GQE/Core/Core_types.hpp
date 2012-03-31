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
 */
#ifndef   GQE_CORE_TYPES_HPP_INCLUDED
#define   GQE_CORE_TYPES_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>
#include <GQE/Config.hpp>
#include <GQE/Core/loggers/onullstream>

#define INFO_LEVEL   0  ///< Defines the value for GQE::SeverityInfo
#define WARN_LEVEL   1  ///< Defines the value for GQE::SeverityWarning
#define ERROR_LEVEL  2  ///< Defines the value for GQE::SeverityError
#define FATAL_LEVEL  3  ///< Defines the value for GQE::Severity
#define NO_LOG_LEVEL 4  ///< Defines the value for no logging

namespace GQE
{
  /// Enumeration of all Asset loading styles
  enum AssetLoadingStyle
  {
    AssetLoadStyleFirst      = 0,  ///< First Loading Style
    AssetLoadStyleBackground = 1,  ///< Background thread loading style
    AssetLoadStyleForeground = 2,  ///< Foreground thread loading style
    AssetLoadStyleImmediate  = 3,  ///< Immediate loading style
    AssetLoadStyleLast             ///< Last Loading Style
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
  class ILogger;
  class IState;

  // Forward declare GQE core classes provided
  class App;
  class AssetManager;
  class ConfigReader;
  class StateManager;

  // Forward declare GQE core assets provided
  class ConfigAsset;
  class FontAsset;
  class ImageAsset;
  class MusicAsset;
  class SoundAsset;

  // Forward declare GQE core loggers provided
  class FatalLogger;
  class FileLogger;
  class ScopeLogger;
  class StringLogger;

  // Forward declare GQE core states provided
  class MenuState;
  class SplashState;

  /// Declare Action ID typedef which is used for identifying Action objects
  typedef std::string typeActionID;

  /// Declare Asset ID typedef which is used for identifying Asset objects
  typedef std::string typeAssetID;

  /// Declare State ID typedef which is used for identifying State objects
  typedef std::string typeStateID;

  /// Declare NameValue typedef which is used for config section maps
  typedef std::map<const std::string, const std::string> typeNameValue;

  /// Declare NameValueIter typedef which is used for name,value pair maps
  typedef std::map<const std::string, const std::string>::iterator typeNameValueIter;

  // Internal Core global variables
  /// Pointer to App class used by Logger to gracefully shutdown App.
  extern GQE_API App*             gApp;
  /// Pointer to Logger tool, usually created in main().
  extern GQE_API ILogger*         gLogger;
  /// Null stream for Logger macros if not active or severity level is disabled
  extern GQE_API std::onullstream gNullStream;
} // namespace GQE
#endif // GQE_CORE_TYPES_HPP_INCLUDED

/**
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
