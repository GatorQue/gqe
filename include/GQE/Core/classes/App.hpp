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
 */
#ifndef   CORE_APP_HPP_INCLUDED
#define   CORE_APP_HPP_INCLUDED
 
#include <fstream>
#include <stddef.h>
#include <string.h>
#include <vector>
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/classes/AssetManager.hpp>
#include <GQE/Core/classes/StatManager.hpp>
#include <GQE/Core/classes/StateManager.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
 
namespace GQE
{
  /// Provides the core game loop algorithm for all game engines.
  class GQE_API App
  {
  public:
    // Constants
    ///////////////////////////////////////////////////////////////////////////
	/// Default Video Width to use if config file not found
    static const unsigned int DEFAULT_VIDEO_WIDTH = 1024;
	/// Default Video Height to use if config file not found
    static const unsigned int DEFAULT_VIDEO_HEIGHT = 768;
	/// Default Video bits per pixel (color depth) if config file not found
    static const unsigned int DEFAULT_VIDEO_BPP = 32;

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
    const sf::Input&          mInput;
    /// AssetManager for managing assets
    AssetManager              mAssetManager;
    /// StatManager for managing game statistics
    StatManager               mStatManager;
    /// StateManager for managing states
    StateManager              mStateManager;

    /**
     * App constructor
     * @param[in] theTitle is the title of the window
     */
    App(const std::string theTitle = "GQE Application");

    /**
     * App deconstructor
     */
    virtual ~App();

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
     * from 1 Hz to 1000 Hz.  Any other value outside this range will not be
     * accepted.
     * 
     * @param[in] theRate in Hz (updates per second) range is [1,1000]
     */
    void SetUpdateRate(float theRate);

    /**
     * Quit will signal the Application to stop running.
	 * @param[in] theExitCode to use when the Run method returns
     */
    void Quit(int theExitCode);

  protected:
    /**
     * PreInit is responsible for getting things ready before the derived
     * classes Init method is called.  This prevents problems that might occur
     * with how the derived classes Init methods are written.
     */
    void PreInit(void);

    /**
     * Init is responsible for initializing the Application.
     */
    virtual void Init(void);

    /**
     * Loop is responsible for monitoring IsRunning and exiting when the
     * Application is done.
     */
    virtual void Loop(void);

    /**
     * Cleanup is responsible for performing any last minute Application
     * cleanup steps before exiting the Application.
     */
    virtual void Cleanup(void);

  private:
    /// The exit code value that will be returned by the program
    int                       mExitCode;
    /// True if the Application is currently running
    bool                      mRunning;
    /// Update rate in seconds (1.0f / UpdateRate) to use for game loop
    float                     mUpdateRate;

    /**
     * App copy constructor is private because we do not allow copies of
     * our Singleton class
     */
    App(const App&);                 // Intentionally undefined

    /**
     * Our assignment operator is private because we do not allow copies
     * of our Singleton class
     */
    App& operator=(const App&);      // Intentionally undefined
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
