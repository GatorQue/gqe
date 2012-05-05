/**
 * Provides the SplashState in the GQE namespace which is typically
 * the first state to be loaded for an application.
 *
 * @file include/GQE/Core/states/SplashState.hpp
 * @author Ryan Lindeman
 * @date 20100710 - Initial Release
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Added ReInit method
 * @date 20110218 - Change to system include style
 * @date 20110625 - Added UpdateVariable and changed Update to UpdateFixed
 * @date 20110627 - Removed extra ; from namespace
 * @date 20110906 - Change mApp from a pointer to an address reference
 */
#ifndef   CORE_SPLASH_STATE_HPP_INCLUDED
#define   CORE_SPLASH_STATE_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/interfaces/IState.hpp>
#include <SFML/Graphics.hpp>

namespace GQE
{
  /// Provides simple Splash screen game state
  class GQE_API SplashState : public IState
  {
    public:
      /**
       * SplashState constructor
       * @param[in] theApp is the address to the App class.
       */
      SplashState(App& theApp, typeAssetID theSplashID,
          const std::string theFilename = "resources/Splash.png", float theDelay = 10.0f);

      /**
       * SplashState deconstructor
       */
      virtual ~SplashState(void);

      /**
       * DoInit is responsible for initializing this State
       */
      virtual void DoInit(void);

      /**
       * ReInit is responsible for Reseting this state when the
       * StateManager::ResetActiveState() method is called.  This way a Game
       * State can be restarted without unloading and reloading the game assets
       */
      virtual void ReInit(void);

      /**
       * HandleEvents is responsible for handling input events for this
       * State when it is the active State.
       * @param[in] theEvent to process from the App class Loop method
       */
      virtual void HandleEvents(sf::Event theEvent);

      /**
       * UpdateFixed is responsible for handling all State fixed update needs for
       * this State when it is the active State.
       */
      virtual void UpdateFixed(void);

      /**
       * UpdateVariable is responsible for handling all State variable update
       * needs for this State when it is the active State.
       * @param[in] theElapsedTime since the last Draw was called
       */
      virtual void UpdateVariable(float theElapsedTime);

      /**
       * Draw is responsible for handling all Drawing needs for this State
       * when it is the Active State.
       */
      virtual void Draw(void);

    protected:
      /**
       * Cleanup is responsible for performing any cleanup required before
       * this State is removed.
       */
      virtual void Cleanup(void);

    private:
      // Variables
      /////////////////////////////////////////////////////////////////////////
      /// The Asset ID to assign to the splash image
      typeAssetID         mSplashID;
      /// The filename of the splash image to load
      std::string         mSplashFilename;
      /// The time in seconds to wait before continuing to next game state
      float               mSplashDelay;
      /// The sprite to hold the splash image being displayed
      sf::Sprite*         mSplashSprite;

  }; // class SplashState
} // namespace GQE

#endif // CORE_SPLASH_STATE_HPP_INCLUDED

/**
 * @class GQE::SplashState
 * @ingroup Core
 * The SplashState class provides a simple Splash screen that removes
 * itself from the StateManager after 10 seconds.  This state is expected
 * to be used at the initial launch of the game application to display
 * the logo of the publisher or game developer.
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
