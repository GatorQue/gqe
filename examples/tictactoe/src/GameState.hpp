/**
 * Provides the TicTacToe GameState example in the GQE library.
 *
 * @file examples/demo/GameState.cpp
 * @author Ryan Lindeman
 * @date 20110704 - Initial Release
 */

#ifndef   GAME_STATE_HPP_INCLUDED
#define   GAME_STATE_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/interfaces/IState.hpp>

/// Provides the TicTacToe GameState example
class GameState : public GQE::IState
{
public:
  /**
   * GameState constructor
   * @param[in] theApp is a pointer to the App class.
   */
  GameState(GQE::App* theApp);

  /**
   * GameState deconstructor
   */
  virtual ~GameState(void);

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
  GQE::ImageAsset* mBackground;
  GQE::ImageAsset* mPlayer1;
  GQE::ImageAsset* mPlayer2;
  GQE::ImageAsset* mEmpty;
  sf::Sprite       mBackgroundSprite;
  sf::Sprite       mCursor;
  sf::Sprite       mBoardSprite[3][3];
  GQE::Uint8       mBoardPlayer[3][3];
  GQE::Uint8       mCurrentPlayer;
#if (SFML_VERSION_MAJOR < 2)
  /// Winner string
  sf::String       mWinnerText;
#else
  /// Winner string
  sf::Text         mWinnerText;
#endif

}; // class GameState

#endif // GAME_STATE_HPP_INCLUDED

/**
 * @class GameState
 * @ingroup Examples
 * The TicTacToe GameState class provides the TicTacToe example game
 * implementation for the GQE library.
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
