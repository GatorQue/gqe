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
 */
 
#include <assert.h>
#include <direct.h>
#include "GQE/Core/classes/App.hpp"
#include "GQE/Core/classes/ConfigReader.hpp"
#include "GQE/Core/states/MenuState.hpp"
#include "GQE/Core/states/SplashState.hpp"
 
namespace GQE
{
  App::App(const std::string theTitle) :
    mTitle(theTitle),
    mVideoMode(DEFAULT_VIDEO_WIDTH, DEFAULT_VIDEO_HEIGHT, DEFAULT_VIDEO_BPP),
    mWindow(),
    mWindowSettings(),
    mWindowStyle(sf::Style::Close | sf::Style::Resize),
    mInput(mWindow.GetInput()),
    mAssetManager(),
    mStatManager(),
    mStateManager(),
    mExitCode(0),
    mRunning(false),
    mUpdateRate(1.0f / 100)
  {
    mLogFile.assign("output.txt");
    mLog.open(mLogFile.c_str());
    mLog << "LogFile: " << mLogFile << std::endl;
 
    // Output to log file
    mLog << "App::App() ctor called" << std::endl;
  }
 
  App::~App()
  {
    mRunning = false;
    // Output to log file
    mLog << "App::~App() dtor called" << std::endl;
  }
 
  void App::ProcessArguments(int argc, char* argv[])
  {
    // Handle command line arguments
    // TODO: Add handling of command line arguments
    mLog << "App::ProcessArguments() Program: " << argv[0] << std::endl;
    mLog << "App::ProcessArguments() Command Line: ";
    for(int iloop = 1; iloop<argc; iloop++)
    {
      mLog << argv[iloop] << ", ";
    }
    mLog << std::endl;
  }
 
  int App::Run(void)
  {
    // Log the starting of Run
    mLog << "App::Run() starting" << std::endl;
 
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
 
    // Initialize our application which might set our Running flag to false
    Init();
 
    // Loop if Running flag is still true
    Loop();
 
    // Cleanup our application
    Cleanup();
 
    // Make sure our Running flag is set to false before exiting
    mRunning = false;
 
    // Log our Exit Code value
    mLog << "App::Run() returning with " << mExitCode << std::endl;
 
    // Return the Exit Code specified by Quit or 0 of Quit was never called
    return mExitCode;
  }
 
  bool App::IsRunning(void) const
  {
    return mRunning;
  }

  float App::GetUpdateRate(void) const
  {
    return (1.0f / mUpdateRate);
  }

  void App::SetUpdateRate(float theRate)
  {
    if(1000.0f >= theRate && 1.0f <= theRate)
    {
      mUpdateRate = 1.0f / theRate;
    }
  }

  void App::Quit(int theExitCode)
  {
    mExitCode = theExitCode;
    mRunning = false;
  }
 
  void App::PreInit(void)
  {
    ConfigReader anConfig;       // For reading .INI style files
 
    // Use our default configuration file to obtain the initial window settings
    anConfig.RegisterApp(this);  // For logging purposes, let ConfigReader know about us
    anConfig.Read("window.cfg"); // Read in our window settings
 
    // Are we in Fullscreen mode?
    if(anConfig.GetBool("window","fullscreen",false))
    {
      mWindowStyle = sf::Style::Fullscreen;
    }
 
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
 
    // Output to log file
    mLog << "App::PreInit() completed" << std::endl;
  }
 
  void App::Init(void)
  {
    // Give the StatManager a chance to initialize
    mStatManager.DoInit();

    // Show statistics: Frames per second (FPS) and Updates per second (UPS)
    mStatManager.SetShow(true);

    // Add Menu State as the next active state
    mStateManager.AddActiveState(new(std::nothrow) MenuState(this));
 
    // Add Splash State as current active state
    mStateManager.AddActiveState(new(std::nothrow) SplashState(this));
 
    // Output to log file
    mLog << "App::Init() completed" << std::endl;
  }
 
  void App::Loop(void)
  {
    // Clock used in restricting Update loop to a fixed rate
    sf::Clock anUpdateClock;
    anUpdateClock.Reset();
 
    // When do we need to update next?
    float anUpdateNext = anUpdateClock.GetElapsedTime();
 
    // Make sure we have at least one state active
    if(mStateManager.IsEmpty())
    {
      // Exit with an error since there isn't an active state
      Quit(StatusAppInitFailed);
    }
 
    // Loop while IsRunning returns true
    while(IsRunning() && mWindow.IsOpened() && !mStateManager.IsEmpty())
    {
      // Get the currently active state
      IState* anState = mStateManager.GetActiveState();
 
      // Check for corrupt state returned by our StateManager
      assert(NULL != anState && "App::Loop() received a bad pointer");
 
      // Create a fixed rate Update loop
      while(anUpdateClock.GetElapsedTime() > anUpdateNext)
      {
        // Handle some events and let the current active state handle the rest
        sf::Event anEvent;
        while(mWindow.GetEvent(anEvent))
        {
          // Switch on Event Type
          switch(anEvent.Type)
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
 
        // Let the current active state perform updates next
        anState->Update();
 
        // Let the StatManager perfom its updates
        mStatManager.Update();

        // Update our update next time
        anUpdateNext += mUpdateRate;
      } // while(anUpdateClock.GetElapsedTime() > anUpdateNext)
 
      // Let the current active state draw stuff
      anState->Draw();
 
      // Let the StatManager perform its drawing
      mStatManager.Draw();

      // Display Render window to the screen
      mWindow.Display();

      // Handle Cleanup of any recently removed states at this point as needed
      mStateManager.HandleCleanup(); 
    } // while(IsRunning() && !mStates.empty())
  }
 
  void App::Cleanup(void)
  {
    // Give the StatManager a chance to de-initialize
    mStatManager.DeInit();

    // Close the Render window if it is still open
    if(mWindow.IsOpened())
    {
      // Show the Mouse cursor
      mWindow.ShowMouseCursor(true);
 
      // Close the Render window
      mWindow.Close();
    }
 
    // Output to log file
    mLog << "App::Cleanup() completed" << std::endl;
  }
 
}; // namespace GQE

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
