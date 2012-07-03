/**
 * Provides the IState class in the GQE namespace which is responsible for
 * providing the IState interface used in the GQE core library.
 *
 * @file src/GQE/Core/interfaces/IState.cpp
 * @author Ryan Lindeman
 * @date 20110801 - Initial Release
 * @date 20120211 - Support new SFML2 snapshot changes
 * @date 20120322 - Support new SFML2 snapshot changes
 * @date 20120512 - Renamed App to IApp since it really is just an interface
 * @date 20120702 - Switched names of Cleanup and HandleCleanup and added cleanup events
 */

#include <assert.h>
#include <GQE/Core/interfaces/IState.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/interfaces/IApp.hpp>

namespace GQE
{
  IState::IState(const typeStateID theStateID, IApp& theApp) :
    mApp(theApp),
    mStateID(theStateID),
    mInit(false),
    mPaused(false),
    mCleanup(false),
    mElapsedTime(0.0f),
    mPausedTime(0.0f)
  {
    ILOG() << "IState::ctor(" << mStateID << ")" << std::endl;
  }

  IState::~IState()
  {
    ILOG() << "IState::dtor(" << mStateID << ")" << std::endl;
  }

  const typeStateID IState::GetID(void) const
  {
    return mStateID;
  }

  void IState::DoInit(void)
  {
    ILOG() << "IState::DoInit(" << mStateID << ")" << std::endl;

    // If Cleanup hasn't been called yet, call it now!
    if(true == mCleanup)
    {
      HandleCleanup();
    }

    // Initialize if necessary
    if(false == mInit)
    {
      mInit = true;
      mPaused = false;
      mElapsedTime = 0.0f;
#if (SFML_VERSION_MAJOR < 2)
      mElapsedClock.Reset();
#else
      mElapsedClock.restart();
#endif
      mPausedTime = 0.0f;
#if (SFML_VERSION_MAJOR < 2)
      mPausedClock.Reset();
#else
      mPausedClock.restart();
#endif
    }
  }

  void IState::DeInit(void)
  {
    ILOG() << "IState::DeInit(" << mStateID << ")" << std::endl;

    if(true == mInit)
    {
      mCleanup = true;
      mInit = false;
#if (SFML_VERSION_MAJOR < 2)
      mElapsedTime += mElapsedClock.GetElapsedTime();
#else
      mElapsedTime += mElapsedClock.getElapsedTime().asSeconds();
#endif
      if(true == mPaused)
      {
#if (SFML_VERSION_MAJOR < 2)
        mPausedTime += mPausedClock.GetElapsedTime();
#else
        mPausedTime += mPausedClock.getElapsedTime().asSeconds();
#endif
      }
    }
  }

  bool IState::IsInitComplete(void)
  {
    return mInit;
  }

  bool IState::IsPaused(void)
  {
    return mPaused;
  }

  void IState::Pause(void)
  {
    ILOG() << "IState::Pause(" << mStateID << ")" << std::endl;

    if(false == mPaused)
    {
      mPaused = true;
#if (SFML_VERSION_MAJOR < 2)
      mPausedClock.Reset();
#else
      mPausedClock.restart();
#endif
    }
  }

  void IState::Resume(void)
  {
    ILOG() << "IState::Resume(" << mStateID << ")" << std::endl;

    if(true == mPaused)
    {
      mPaused = false;
#if (SFML_VERSION_MAJOR < 2)
      mPausedTime += mPausedClock.GetElapsedTime();
#else
      mPausedTime += mPausedClock.getElapsedTime().asSeconds();
#endif

    }
  }

  float IState::GetElapsedTime(void) const
  {
#if (SFML_VERSION_MAJOR < 2)
    float result = mElapsedClock.GetElapsedTime();
#else
    float result = mElapsedClock.getElapsedTime().asSeconds();
#endif

    if(false == mInit)
    {
      result = mElapsedTime;
    }

    return result;
  }

  void IState::Cleanup(void)
  {
    // Always call our cleanup events with our pointer when this method is called
    mCleanupEvents.DoEvents(this);

    // This will be true if this IState is about to be deleted soon
    if(true == mCleanup)
    {
      // Call our handle cleanup virtual method
      HandleCleanup();

      // Clear our cleanup flag
      mCleanup = false;
    }
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
