/**
 * Provides the App class in the GQE namespace which is responsible for
 * providing the App base class implementation used in the GQE core library.
 *
 * @file include/GQE/Core/classes/App.hpp
 * @author Ryan Lindeman
 * @date 20100705 - Initial Release
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110128 - Add Get/SetUpdateRate methods for changing game rate speed
 * @date 20110128 - Added new StatManager class for collecting game statistics
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Change to system include style
 * @date 20110611 - Convert logging to new Log macros
 * @date 20110625 - Added UpdateVariable and changed Update to UpdateFixed
 * @date 20110704 - Changed Init to pure virtual function and defaults to 800x600
 * @date 20110831 - Support new SFML2 snapshot changes
 * @date 20120512 - Add new Init hooks for derived classes and changed name to IApp
 * @date 20120609 - Default to 20 UPS, 20 FPS, and windowed mode and added new
 *                  improved gameloop.
 */
#ifndef   CORE_APP_HPP_INCLUDED
#define   CORE_APP_HPP_INCLUDED

#include <fstream>
#include <stddef.h>
#include <string.h>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GQE/Core/classes/AssetManager.hpp>
#include <GQE/Core/classes/StatManager.hpp>
#include <GQE/Core/classes/StateManager.hpp>
#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// Provides the core game loop algorithm for all game engines.
  class GQE_API IApp
  {
    public:
      // Constants
      ///////////////////////////////////////////////////////////////////////////
      /// Default Video Width to use if config file not found
      static const unsigned int DEFAULT_VIDEO_WIDTH = 800;
      /// Default Video Height to use if config file not found
      static const unsigned int DEFAULT_VIDEO_HEIGHT = 600;
      /// Default Video bits per pixel (color depth) if config file not found
      static const unsigned int DEFAULT_VIDEO_BPP = 32;
      /// Default application wide settings file string
      static const char* APP_SETTINGS;

      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Title to use for Window
      std::string               mTitle;
      /// Video Mode to use (width, height, bpp)
      sf::VideoMode             mVideoMode;
      /// Render window to draw to
      sf::RenderWindow          mWindow;
      /// Window settings to use when creating Render window
#if (SFML_VERSION_MAJOR < 2)
      sf::WindowSettings        mWindowSettings;
#else
      sf::ContextSettings       mContextSettings;
#endif
      /// Window style to use when createing Render window
      unsigned long             mWindowStyle;
      /// Input manager for Render window above
#if (SFML_VERSION_MAJOR < 2)
      const sf::Input&          mInput;
#endif
      /// AssetManager for managing assets
      AssetManager              mAssetManager;
      /// StatManager for managing game statistics
      StatManager               mStatManager;
      /// StateManager for managing states
      StateManager              mStateManager;

      /**
       * IApp deconstructor
       */
      virtual ~IApp();

      /**
       * GetApp will return the most recent App based class that was created so
       * it can be used to access the App interface for retrieving assets,
       * exiting the program, etc. NULL will be returned if none is available
       */
      static IApp* GetApp(void);

      /**
       * ProcessArguments is responsible for processing command line arguments
       * provided to the application.
       * @param[in] argc is the number of arguments
       * @param[in] argv are the actual arguments
       */
      virtual void ProcessArguments(int argc, char* argv[]);

      /**
       * Run is called after the Application is created and will call the
       * Init, Loop, and Cleanup methods that are defined by the derived
       * class.
       * @return the value of mExitCode as set by the Quit method
       */
      int Run(void);

      /**
       * IsRunning will return true if the Application is still running.
       * @return true if Application is running, false otherwise
       */
      bool IsRunning(void) const;

      /**
       * GetUpdateRate will return the current game loop update rate being
       * used.
       * @return update rate in Hz (updates per second)
       */
      float GetUpdateRate(void) const;

      /**
       * SetUpdateRate will set the game loop update rate to theRate specified
       * (from 1 Hz to 200 Hz, any other value outside this range will not be
       * accepted). The update rate is always fixed to provide a deterministic
       * game loop. But the actual updates per second is affected by the
       * performance of the system and the complexity of your UpdateFixed game
       * logic. Use SetMaxUpdates to improve the frames per second on lower
       * performing systems.
       * @param[in] theRate in Hz (updates per second) range is [1,200]
       */
      void SetUpdateRate(float theRate);

      /**
       * SetMaxUpdates will set the maximum number of sequential updates
       * allowed in any given game loop. If your frames per second rate is
       * struggling, you should set theMaxUpdates to 1.
       * @param[in] theMaxUpdates range is [1,200]
       */
      void SetMaxUpdates(Uint32 theMaxUpdates);

      /**
       * Quit will signal the Application to stop running.
       * @param[in] theExitCode to use when the Run method returns
       */
      void Quit(int theExitCode = StatusAppOK);

    protected:
      /**
       * App constructor
       * @param[in] theTitle is the title of the window
       */
      IApp(const std::string theTitle = "GQE Application");

      /**
       * InitAssetHandlers is responsible for registering custom IAssetHandler
       * derived classes for a specific game application.
       */
      virtual void InitAssetHandlers(void) = 0;

      /**
       * InitScreenFactory is responsible for initializing any IScreen derived
       * classes with the ScreenManager class that will be used to create new
       * IScreen derived classes as requested.
       */
      virtual void InitScreenFactory(void) = 0;

      /**
       * GameLoop is responsible for monitoring IsRunning and exiting when the
       * Application is done.
       */
      virtual void GameLoop(void);

      /**
       * ProcessInput is responsible for performing all input processing for
       * the game loop.
       */
      virtual void ProcessInput(IState& theState);

      /**
       * HandleCleanup is responsible for performing any custom last minute
       * Application cleanup steps before exiting the Application.
       */
      virtual void HandleCleanup(void) = 0;

    private:
      /// Instance variable assigned at construction time
      static IApp* gApp;

      /// The exit code value that will be returned by the program
      int          mExitCode;
      /// True if the Application is currently running
      bool         mRunning;
#if (SFML_VERSION_MAJOR < 2)
      /// Update rate in seconds to use for fixed update in game loop
      float        mUpdateRate;
#else
      /// Update rate in milliseconds to use for fixed update in game loop
      sf::Int32    mUpdateRate;
#endif
      /// Maximum sequential UpdateFixed calls allowed to still meet minimum frame rate
      Uint32       mMaxUpdates;

      /**
       * InitApplication is responsible for registering and loading the
       * application wide configuration file (resources/settings.cfg) found in
       * the resources directory from the current working directory.
       */
      void InitSettingsConfig(void);

      /**
       * InitRenderer is responsible for initializing the Rendering window that
       * will be used to display the games graphics.
       */
      void InitRenderer(void);

      /**
       * Cleanup is responsible for performing any last minute Application
       * cleanup steps before exiting the Application.
       */
      void Cleanup(void);

      /**
       * App copy constructor is private because we do not allow copies of
       * our Singleton class
       */
      IApp(const IApp&);               // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our Singleton class
       */
      IApp& operator=(const IApp&);    // Intentionally undefined
  }; // class App
} // namespace GQE

#endif // CORE_APP_HPP_INCLUDED
/**
 * @class GQE::App
 * @ingroup Core
 * The App class is at the heart of every game engine in GQ Engines. Its'
 * purpose is to provide the basic game loop control and as a central
 * location for all Manager classes.  Each individual game state will have
 * access to the App class so it can access these Manager classes.  The
 * App class can be easily subclassed to provide custom Manager classes
 * for the specific game genre needed.
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
