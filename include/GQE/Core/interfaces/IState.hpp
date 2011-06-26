/**
 * Provides the State class in the GQE namespace which is responsible for
 * providing the State interface used in the GQE core library.
 *
 * @file include/GQE/Core/interfaces/IState.hpp
 * @author Ryan Lindeman
 * @date 20100705 - Initial Release
 * @date 20110125 - Use stringstream for UPS/FPS display
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110127 - Use new OS independent Uint/Int types
 * @date 20110128 - Moved UPS/FPS calculations to StatManager.
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Added ReInit method for StateManager::ResetActiveState
 * @date 20110218 - Reset our Cleanup flag after HandleCleanup is called and
 *                  call HandleCleanup in DoInit if cleanup flag is set.
 * @date 20110218 - Change to system include style
 * @date 20110611 - Convert logging to new Log macros
 * @date 20110625 - Added UpdateVariable and changed Update to UpdateFixed
 */
#ifndef   CORE_ISTATE_HPP_INCLUDED
#define   CORE_ISTATE_HPP_INCLUDED
 
#include <assert.h>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/classes/App.hpp>
#include <SFML/System.hpp>
 
namespace GQE
{
  /// Provides the base class interface for all game states
  class GQE_API IState
  {
  public:
 
    /**
     * State deconstructor
     */
    virtual ~IState()
    {
      ILOG() << "IState::dtor(" << mID << ")" << std::endl;
 
      // Clear out pointers that we don't need anymore
      mApp = NULL;
    }
 
    /**
     * GetID will return the ID used to identify this State object
     * @return GQE::typeStateID is the ID for this State object
     */
    const GQE::typeStateID GetID(void) const
    {
      return mID;
    }
 
    /**
     * DoInit is responsible for initializing this State.  HandleCleanup will
     * be called if mCleanup is true so Derived classes should always call
     * IState::DoInit() first before initializing their assets.
     */
    virtual void DoInit(void)
    {
      ILOG() << "IState::DoInit(" << mID << ")" << std::endl;

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
        mElapsedClock.Reset();
        mPausedTime = 0.0f;
        mPausedClock.Reset();
      }
    }
 
    /**
     * ReInit is responsible for Reseting this state when the 
     * StateManager::ResetActiveState() method is called.  This way a Game
     * State can be restarted without unloading and reloading the game assets
     */
    virtual void ReInit(void) = 0;

    /**
     * DeInit is responsible for marking this state to be cleaned up
     */
    void DeInit(void)
    {
      ILOG() << "IState::DeInit(" << mID << ")" << std::endl;

      if(true == mInit)
      {
        mCleanup = true;
        mInit = false;
#if (SFML_VERSION_MAJOR < 2)
        mElapsedTime += mElapsedClock.GetElapsedTime();
#else
        mElapsedTime += (float)mElapsedClock.GetElapsedTime() / 1000.0f;
#endif
        if(true == mPaused)
        {
#if (SFML_VERSION_MAJOR < 2)
          mPausedTime += mPausedClock.GetElapsedTime();
#else
          mPausedTime += (float)mPausedClock.GetElapsedTime() / 1000.0f;
#endif
        }
      }
    }

    /**
     * IsInitComplete will return true if the DoInit method has been called
     * for this state.
     * @return true if DoInit has been called, false otherwise or if DeInit
     *         was called
     */
    bool IsInitComplete(void)
    {
      return mInit;
    }

    /**
     * IsPaused will return true if this state is not the currently active
     * state.
     * @return true if state is not current active state, false otherwise
     */
    bool IsPaused(void)
    {
      return mPaused;
    }

    /**
     * Pause is responsible for pausing this State since the Application
     * may have lost focus or another State has become activate.
     */
    virtual void Pause(void)
    {
      ILOG() << "IState::Pause(" << mID << ")" << std::endl;

      if(false == mPaused)
      {
        mPaused = true;
        mPausedClock.Reset();
      }
    }

    /**
     * Resume is responsible for resuming this State since the Application
     * may have gained focus or the previous State was removed.
     */
    virtual void Resume(void)
    {
      ILOG() << "IState::Resume(" << mID << ")" << std::endl;

      if(true == mPaused)
      {
        mPaused = false;
#if (SFML_VERSION_MAJOR < 2)
        mPausedTime += mPausedClock.GetElapsedTime();
#else
        mPausedTime += (float)mPausedClock.GetElapsedTime() / 1000.0f;
#endif
      }
    }

    /**
     * HandleEvents is responsible for handling input events for this
     * State when it is the active State.
     * @param[in] theEvent to process from the App class Loop method
     */
    virtual void HandleEvents(sf::Event theEvent) = 0;
 
    /**
     * UpdateFixed is responsible for handling all State fixed update needs for
     * this State when it is the active State.
     */
    virtual void UpdateFixed(void) = 0;
 
    /**
     * UpdateVariable is responsible for handling all State variable update
     * needs for this State when it is the active State.
     * @param[in] theElapsedTime since the last Draw was called
     */
    virtual void UpdateVariable(float theElapsedTime) = 0;
 
    /**
     * Draw is responsible for handling all Drawing needs for this State
     * when it is the Active State.
     */
    virtual void Draw(void) = 0;
 
    /**
     * HandleCleanup is responsible for calling Cleanup if this class has been
     * flagged to be cleaned up after it completes the game loop.
     */
    void HandleCleanup(void)
    {
      if(true == mCleanup)
      {
        // Call cleanup
        Cleanup();

        // Clear our cleanup flag
        mCleanup = false;
      }
    }

    /**
     * GetElapsedTime will return one of the following:
     * 1) If this state is not paused: total elapsed time since DoInit was called
     * 2) If this state is paused: total elapsed time since Pause was called
     * 3) If this state is not initialized: total elapsed time from DoInit to DeInit
     * @return total elapsed time as described above.
     */
    float GetElapsedTime(void) const
    {
#if (SFML_VERSION_MAJOR < 2)
      float result = mElapsedClock.GetElapsedTime();
#else
      float result = (float)mElapsedClock.GetElapsedTime() / 1000.0f;
#endif

      if(false == mInit)
      {
        result = mElapsedTime;
      }
 
      return result;
    }

  protected:
    /// Pointer to the App class
    App*                  mApp;

    /**
     * IState constructor is private because we do not allow copies of our
     * Singleton class
     * @param[in] theID to use for this State object
     * @param[in] theApp is a pointer to the App derived class
     */
    IState(const typeStateID theID, App* theApp) :
        mApp(theApp),
        mID(theID),
        mInit(false),
        mPaused(false),
        mCleanup(false),
        mElapsedTime(0.0f),
        mPausedTime(0.0f)
    {
      ILOG() << "IState::ctor(" << mID << ")" << std::endl;

      // Check that our pointer is good
      assert(NULL != theApp && "IState::IState() theApp pointer is bad");
 
      // Keep a copy of our Application pointer
      mApp = theApp; 
    }

    /**
     * Cleanup is responsible for performing any cleanup required before
     * this State is removed.
     */
    virtual void Cleanup(void)
    {
      ILOG() << "IState::Cleanup(" << mID << ")" << std::endl;
    }

  private:
    /// The State ID
    const typeStateID     mID;
    /// Boolean that indicates that DoInit has been called
    bool                  mInit;
    /// State is currently paused (not active)
    bool                  mPaused;
    /// State needs to be cleaned up at the end of the next game loop
    bool                  mCleanup;
    /// Clock will help us keep track of running and paused elapsed time
    sf::Clock             mElapsedClock;
    /// Total elapsed time since DoInit was called
    float                 mElapsedTime;
    /// Clock will help us keep track of time paused
    sf::Clock             mPausedClock;
    /// Total elapsed time paused since DoInit was called
    float                 mPausedTime;

    /**
     * Our copy constructor is private because we do not allow copies of
     * our Singleton class
     */
    IState(const IState&);  // Intentionally undefined

    /**
     * Our assignment operator is private because we do not allow copies
     * of our Singleton class
     */
    IState& operator=(const IState&); // Intentionally undefined

  }; // class IState
}; // namespace GQE

#endif // CORE_ISTATE_HPP_INCLUDED

/**
 * @class GQE::IState
 * @ingroup Core
 * The IState interface class is what defines the minimum methods that a
 * game state must implement.  These minimum methods are used by the App
 * class, the StateManager class, the StatManager class, and other classes.
 * This is what allows game states to interact with each other.
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
