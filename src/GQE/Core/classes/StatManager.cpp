/**
 * Provides the StatManager class in the GQE namespace which is responsible
 * for collecting and providing Statistical information about the application.
 * This information includes, the current Updates per second and Frames per
 * second and other statistics.
 *
 * @file src/GQE/Core/classes/StatManager.hpp
 * @author Ryan Lindeman
 * @date 20110128 - Initial Release
 * @date 20110128 - Moved to GQE Core library and src directory
 * @date 20110218 - Change to system include style
 * @date 20110611 - Convert logging to new Log macros
 * @date 20110625 - Added UpdateVariable and changed Update to UpdateFixed
 * @date 20110627 - Fixed ctor init order and removed extra ; from namespace
 * @date 20110704 - Changed Move to SetPosition
 * @date 20120211 - Support new SFML2 snapshot changes
 * @date 20120322 - Support new SFML2 snapshot changes
 * @date 20120421 - Use arial.ttf font since SFML 2 crashes on exit when using default font
 * @date 20120512 - Renamed App to IApp since it really is just an interface
 * @date 20120518 - Use sf::Font instead of FontAsset to remove circular dependency
 * @date 20120609 - Whitespace changes
 */

#include <assert.h>
#include <sstream>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/classes/StatManager.hpp>
#include <GQE/Core/interfaces/IApp.hpp>

namespace GQE
{
  StatManager::StatManager() :
    mApp(NULL),
    mShow(false),
    mFrames(0),
    mFrameClock(),
    mDefaultFont(),
    mFPS(NULL),
    mUpdates(0),
    mUpdateClock(),
    mUPS(NULL)
  {
    ILOGM("StatManager::ctor()");
#if SFML_VERSION_MAJOR < 2
		mDefaultFont.LoadFromFile("resources/arial.ttf");
#else
		mDefaultFont.loadFromFile("resources/arial.ttf");
#endif
  }

  StatManager::~StatManager()
  {
    ILOGM("StatManager::dtor()");

    // Clear pointers we don't need anymore
    mApp = NULL;
  }

  void StatManager::DoInit(void)
  {
    ILOGM("StatManager::DoInit()");

    // Reset our counters
    mFrames = 0;
    mUpdates = 0;

    // Reset our clocks
#if (SFML_VERSION_MAJOR < 2)
    mFrameClock.Reset();
    mUpdateClock.Reset();

    // Position and color for the FPS/UPS string
    mFPS = new sf::String("", mDefaultFont, 30.0F);
    mFPS->SetColor(sf::Color(255,255,255,128));
    mFPS->SetPosition(0,0);
    
    mUPS = new sf::String("", mDefaultFont, 30.0F);
    mUPS->SetColor(sf::Color(255,255,255,128));
    mUPS->SetPosition(0,30);
#else
    mFrameClock.restart();
    mUpdateClock.restart();

    // Position and color for the FPS/UPS string
    mFPS = new sf::Text("", mDefaultFont, 30);
    mFPS->setColor(sf::Color(255,255,255,128));
    mFPS->setPosition(0,0);

    mUPS = new sf::Text("", mDefaultFont, 30);
    mUPS->setColor(sf::Color(255,255,255,128));
    mUPS->setPosition(0,30);
#endif
  }

  void StatManager::DeInit(void)
  {
    ILOGM("StatManager::DeInit()");

    // Delete our FPS string
    delete mFPS;
    mFPS = NULL;

    // Delete our UPS string
    delete mUPS;
    mUPS = NULL;
  }

  bool StatManager::IsShowing(void) const
  {
    return mShow;
  }

  void StatManager::SetShow(bool theShow)
  {
    mShow = theShow;
  }

  Uint32 StatManager::GetUpdates(void) const
  {
    return mUpdates;
  }

  Uint32 StatManager::GetFrames(void) const
  {
    return mFrames;
  }

  void StatManager::RegisterApp(IApp* theApp)
  {
    // Check that our pointer is good
    assert(NULL != theApp && "StatManager::RegisterApp() theApp pointer provided is bad");

    // Make a note of the pointer
    assert(NULL == mApp && "StatManager::RegisterApp() theApp pointer was already registered");
    mApp = theApp;
  }

  void StatManager::UpdateFixed(void)
  {
    // Check our App pointer
    assert(NULL != mApp && "StatManager::Update() bad app pointer");

    // Increment our update counter
    mUpdates++;
#if (SFML_VERSION_MAJOR < 2)
    if(mUpdateClock.GetElapsedTime() > 1.0f)
#else
      if(mUpdateClock.getElapsedTime().asSeconds() > 1.0f)
#endif
      {
        // Updates string stream
        std::ostringstream updates;

        // Update our UPS string to be displayed
        updates.precision(2);
        updates.width(7);
        updates << "UPS: " << std::fixed << mUpdates;
#if (SFML_VERSION_MAJOR < 2)
        mUPS->SetText(updates.str());
#else
        mUPS->setString(updates.str());
#endif

        // Reset our Update clock and update counter
        mUpdates = 0;
#if (SFML_VERSION_MAJOR < 2)
        mUpdateClock.Reset();
#else
        mUpdateClock.restart();
#endif
      }
  }

  void StatManager::Draw(void)
  {
    // Check our mApp pointer
    assert(NULL != mApp && "StatManager::Draw() invalid app pointer provided");

    // Increment our frame counter
    mFrames++;
#if (SFML_VERSION_MAJOR < 2)
    if(mFrameClock.GetElapsedTime() > 1.0f)
#else
    if(mFrameClock.getElapsedTime().asSeconds() > 1.0f)
#endif
    {
      // Frames string stream
      std::ostringstream frames;

      // Get our FramesPerSecond value
      frames.precision(2);
      frames.width(7);
      frames << "FPS: " << std::fixed << mFrames;
#if (SFML_VERSION_MAJOR < 2)
      mFPS->SetText(frames.str());
#else
      mFPS->setString(frames.str());
#endif

      // Reset our Frames clock and frame counter
      mFrames = 0;
#if (SFML_VERSION_MAJOR < 2)
      mFrameClock.Reset();
#else
      mFrameClock.restart();
#endif
    }

    // Are we showing the current statistics?
    if(mShow)
    {
#if (SFML_VERSION_MAJOR < 2)
      // Draw the Frames Per Second debug value on the screen
      mApp->mWindow.Draw(*mFPS);

      // Draw the Updates Per Second debug value on the screen
      mApp->mWindow.Draw(*mUPS);
#else
      // Draw the Frames Per Second debug value on the screen
      mApp->mWindow.draw(*mFPS);

      // Draw the Updates Per Second debug value on the screen
      mApp->mWindow.draw(*mUPS);
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
