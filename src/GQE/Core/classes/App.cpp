/**
 * Provides the App class in the GQE namespace which is responsible for
 * providing the App base class implementation used in the GQE core library.
 *
 * @file src/GQE/Core/classes/App.cpp
 * @author Ryan Lindeman
 * @date 20100705 - Initial Release
 * @date 20110106 - Added ConfigReader for loading window settings in PreInit
 * @date 20110118 - Fixed compiler problems with ConfigReader
 * @date 20110125 - Drop use of non-standard _getcwd for now.
 * @date 20110125 - IState::HandleCleanup is now called from StateManager
 * @date 20110127 - Moved to GQE Core library and src directory
 * @date 20110128 - Add Get/SetUpdateRate methods for changing game rate speed
 * @date 20110128 - Added new StatManager class for collecting game statistics
 * @date 20110218 - Change to system include style
 * @date 20110331 - Removed direct.h include as it is no longer needed
 * @date 20110611 - Convert logging to new Log macros and added gApp pointer
 * @date 20110625 - Added UpdateVariable and changed Update to UpdateFixed
 * @date 20110627 - Removed extra ; from namespace
 * @date 20110704 - Removed Init method (moved StatManager.DoInit to Run method)
 * @date 20110831 - Support new SFML2 snapshot changes
 * @date 20120211 - Support new SFML2 snapshot changes
 * @date 20120322 - Support new SFML2 snapshot changes
 */

#include <assert.h>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/classes/App.hpp>
#include <GQE/Core/classes/ConfigReader.hpp>
#include <GQE/Core/interfaces/IState.hpp>

namespace GQE
{
  App::App(const std::string theTitle) :
    mTitle(theTitle),
    mVideoMode(DEFAULT_VIDEO_WIDTH, DEFAULT_VIDEO_HEIGHT, DEFAULT_VIDEO_BPP),
    mWindow(),
#if (SFML_VERSION_MAJOR < 2)
    mWindowSettings(),
#else
    mContextSettings(),
#endif
    mWindowStyle(sf::Style::Close | sf::Style::Resize),
#if (SFML_VERSION_MAJOR < 2)
    mInput(mWindow.GetInput()),
#endif
    mAssetManager(),
    mStatManager(),
    mStateManager(),
    mExitCode(0),
    mRunning(false),
#if (SFML_VERSION_MAJOR < 2)
    mUpdateRate(1.0f / 100.0f) // in seconds
#else
    mUpdateRate((Uint32)(1000.0f / 100.0f)) // in milliseconds
#endif
  {
    // Save our global App pointer
    gApp = this;
  }

  App::~App()
  {
    // Make sure our running flag is false
    mRunning = false;

    // Remove our global App pointer
    gApp = NULL;
  }

  void App::ProcessArguments(int argc, char* argv[])
  {
    // Handle command line arguments
    // TODO: Add handling of command line arguments
    if(argc == 1)
    {
      ILOG() << "App::ProcessArguments(" << argv[0] << ") command line: (none)" << std::endl;
    }
    else
    {
      ILOG() << "App::ProcessArguments(" << argv[0] << ") command line:" << std::endl;
      for(int iloop = 1; iloop<argc; iloop++)
      {
        ILOG() << "Argument" << iloop << "=(" << argv[iloop] << ")" << std::endl;
      }
    }
  }

  int App::Run(void)
  {
    SLOG(App_Run,SeverityInfo) << std::endl;

    // First set our Running flag to true
    mRunning = true;

    // Register our App pointer with our AssetManager
    mAssetManager.RegisterApp(this);

    // Register our App pointer with our StatManager
    mStatManager.RegisterApp(this);

    // Register our App pointer with our StateManager
    mStateManager.RegisterApp(this);

    // Pre-init is responsible for the following:
    // 1) Opening our configuration file
    // 2) Setting up our render window
    PreInit();

    // Give the StatManager a chance to initialize
    mStatManager.DoInit();

    // Show statistics: Frames per second (FPS) and Updates per second (UPS)
    mStatManager.SetShow(false);

    // Initialize our application which might set our Running flag to false
    Init();

    // Loop if Running flag is still true
    Loop();

    // Cleanup our application
    Cleanup();

    // Make sure our Running flag is set to false before exiting
    mRunning = false;

    if(mExitCode < 0)
      SLOGR(App_Run,SeverityError) << "exitCode=" << mExitCode << std::endl;
    else
      SLOGR(App_Run,SeverityInfo) << "exitCode=" << mExitCode << std::endl;

    // Return the Exit Code specified by Quit or 0 of Quit was never called
    return mExitCode;
  }

  bool App::IsRunning(void) const
  {
    return mRunning;
  }

  float App::GetUpdateRate(void) const
  {
#if (SFML_VERSION_MAJOR < 2)
    return (1.0f / mUpdateRate);
#else
    return (1000.0f / (float)mUpdateRate);
#endif
  }

  void App::SetUpdateRate(float theRate)
  {
    if(1000.0f >= theRate && 1.0f <= theRate)
    {
#if (SFML_VERSION_MAJOR < 2)
      mUpdateRate = 1.0f / theRate;
#else
      mUpdateRate = (Uint32)(1000.0f / theRate);
#endif
    }
  }

  void App::Quit(int theExitCode)
  {
    mExitCode = theExitCode;
    mRunning = false;
  }

  void App::PreInit(void)
  {
    SLOG(App_PreInit, SeverityInfo) << std::endl;
    ConfigReader anConfig;       // For reading .INI style files

    // Use our default configuration file to obtain the initial window settings
    anConfig.LoadFromFile("resources/window.cfg"); // Read in our window settings

    // Are we in Fullscreen mode?
    if(anConfig.GetBool("window","fullscreen",true))
    {
      mWindowStyle = sf::Style::Fullscreen;
    }

#if (SFML_VERSION_MAJOR < 2)
    // What size window does the user want?
    mVideoMode.Width = anConfig.GetUint32("window","width",DEFAULT_VIDEO_WIDTH);
    mVideoMode.Height = anConfig.GetUint32("window","height",DEFAULT_VIDEO_HEIGHT);
    mVideoMode.BitsPerPixel = anConfig.GetUint32("window","depth",DEFAULT_VIDEO_BPP);

    // For Fullscreen, verify valid VideoMode, otherwise revert to defaults for Fullscreen
    if(sf::Style::Fullscreen == mWindowStyle && false == mVideoMode.IsValid())
    {
      mVideoMode.Width = DEFAULT_VIDEO_WIDTH;
      mVideoMode.Height = DEFAULT_VIDEO_HEIGHT;
      mVideoMode.BitsPerPixel = DEFAULT_VIDEO_BPP;
    }

    // Create a RenderWindow object using VideoMode object above
    mWindow.Create(mVideoMode, mTitle, mWindowStyle, mWindowSettings);

    // Use Vertical Sync
    mWindow.UseVerticalSync(true);
#else
    // What size window does the user want?
    mVideoMode.width = anConfig.GetUint32("window","width",DEFAULT_VIDEO_WIDTH);
    mVideoMode.height = anConfig.GetUint32("window","height",DEFAULT_VIDEO_HEIGHT);
    mVideoMode.bitsPerPixel = anConfig.GetUint32("window","depth",DEFAULT_VIDEO_BPP);

    // For Fullscreen, verify valid VideoMode, otherwise revert to defaults for Fullscreen
    if(sf::Style::Fullscreen == mWindowStyle && false == mVideoMode.isValid())
    {
      mVideoMode.width = DEFAULT_VIDEO_WIDTH;
      mVideoMode.height = DEFAULT_VIDEO_HEIGHT;
      mVideoMode.bitsPerPixel = DEFAULT_VIDEO_BPP;
    }

    // Create a RenderWindow object using VideoMode object above
    mWindow.create(mVideoMode, mTitle, mWindowStyle, mContextSettings);

    // Use Vertical Sync
    mWindow.setVerticalSyncEnabled(true);
#endif
  }

  void App::Loop(void)
  {
    SLOG(App_Loop, SeverityInfo) << std::endl;

    // Clock used in restricting Update loop to a fixed rate
    sf::Clock anUpdateClock;

#if (SFML_VERSION_MAJOR < 2)
    // Restart/Reset our Update clock
    anUpdateClock.Reset();

    // When do we need to update next (in seconds)?
    float anUpdateNext = anUpdateClock.GetElapsedTime();
#else
    // Clock used in calculating the time elapsed since the last frame
    sf::Clock anFrameClock;

    // Restart/Reset our Update clock
    anUpdateClock.restart();

    // When do we need to update next (in milliseconds)?
    Int32 anUpdateNext = anUpdateClock.getElapsedTime().asMilliseconds();
#endif

    // Make sure we have at least one state active
    if(mStateManager.IsEmpty())
    {
      // Exit with an error since there isn't an active state
      Quit(StatusAppInitFailed);
    }

    // Loop while IsRunning returns true
#if (SFML_VERSION_MAJOR < 2)
    while(IsRunning() && mWindow.IsOpened() && !mStateManager.IsEmpty())
#else
    while(IsRunning() && mWindow.isOpen() && !mStateManager.IsEmpty())
#endif
    {
      // Get the currently active state
      IState* anState = mStateManager.GetActiveState();

      // Check for corrupt state returned by our StateManager
      assert(NULL != anState && "App::Loop() received a bad pointer");

      // Create a fixed rate Update loop
#if (SFML_VERSION_MAJOR < 2)
      while(anUpdateClock.GetElapsedTime() > anUpdateNext)
#else
      while(anUpdateClock.getElapsedTime().asMilliseconds() > anUpdateNext)
#endif
      {
        // Handle some events and let the current active state handle the rest
        sf::Event anEvent;
#if (SFML_VERSION_MAJOR < 2)
        while(mWindow.GetEvent(anEvent))
#else
        while(mWindow.pollEvent(anEvent))
#endif
        {
#if (SFML_VERSION_MAJOR < 2)
          // Switch on Event Type
          switch(anEvent.Type)
#else
          // Switch on Event Type
          switch(anEvent.type)
#endif
          {
            case sf::Event::Closed:       // Window closed
              Quit(StatusAppOK);
              break;
            case sf::Event::GainedFocus:  // Window gained focus
              anState->Resume();
              break;
            case sf::Event::LostFocus:    // Window lost focus
              anState->Pause();
              break;
            case sf::Event::Resized:      // Window resized
              break;
            default:                      // Current active state will handle
              anState->HandleEvents(anEvent);
          } // switch(anEvent.Type)
        } // while(mWindow.GetEvent(anEvent))

        // Let the current active state perform fixed updates next
        anState->UpdateFixed();

        // Let the StatManager perfom its updates
        mStatManager.UpdateFixed();

        // Update our update next time
        anUpdateNext += mUpdateRate;
      } // while(anUpdateClock.GetElapsedTime() > anUpdateNext)

      // Let the current active state perform its variable update
#if (SFML_VERSION_MAJOR < 2)
      anState->UpdateVariable(mWindow.GetFrameTime());
#else
      // Convert to floating point value of seconds for SFML 2.0
      anState->UpdateVariable(anFrameClock.restart().asSeconds());
#endif

      // Let the current active state draw stuff
      anState->Draw();

      // Let the StatManager perform its drawing
      mStatManager.Draw();

#if (SFML_VERSION_MAJOR < 2)
      // Display Render window to the screen
      mWindow.Display();
#else
      // Display Render window to the screen
      mWindow.display();
#endif

      // Handle Cleanup of any recently removed states at this point as needed
      mStateManager.HandleCleanup(); 
    } // while(IsRunning() && !mStates.empty())
  }

  void App::Cleanup(void)
  {
    SLOG(App_Cleanup, SeverityInfo) << std::endl;

    // Give the StatManager a chance to de-initialize
    mStatManager.DeInit();

    // Close the Render window if it is still open
#if (SFML_VERSION_MAJOR < 2)
    if(mWindow.IsOpened())
#else
    if(mWindow.isOpen())
#endif
    {
#if (SFML_VERSION_MAJOR < 2)
      // Show the Mouse cursor
      mWindow.ShowMouseCursor(true);

      // Close the Render window
      mWindow.Close();
#else
      // Show the Mouse cursor
      mWindow.setMouseCursorVisible(true);

      // Close the Render window
      mWindow.close();
#endif
    }
  }

} // namespace GQE

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
