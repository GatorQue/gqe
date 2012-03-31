/**
 * Provides the TicTacToe GameState example in the GQE library.
 *
 * @file examples/demo/GameState.cpp
 * @author Ryan Lindeman
 * @date 20110704 - Initial Release
 * @date 20110721 - Remove * from GetAsset() calls since it now returns TYPE&
 * @date 20110831 - Support new SFML2 snapshot changes
 */
#include "GameState.hpp"
#include <GQE/Core/assets/ImageAsset.hpp>
#include <GQE/Core/classes/App.hpp>
#include <SFML/Graphics/Color.hpp>

GameState::GameState(GQE::App& theApp) :
  GQE::IState("Game",theApp),
  mBackground(NULL),
  mPlayer1(NULL),
  mPlayer2(NULL),
  mEmpty(NULL),
  mCurrentPlayer(0)
{
}

GameState::~GameState(void)
{
}

void GameState::DoInit(void)
{
  // First call our base class implementation
  IState::DoInit();

  // Load our Background image which will show the TicTacToe game board
  mBackground = mApp.mAssetManager.AddImage("Board", "resources/Board.png",
      GQE::AssetLoadStyleImmediate);
  if(NULL != mBackground)
  {
#if (SFML_VERSION_MAJOR < 2)
    mBackgroundSprite.SetImage(mBackground->GetAsset());
#else
    mBackgroundSprite.setTexture(mBackground->GetAsset());
#endif
  }

  // Load our Player 1 and Player 2 images which will show an X and O pieces
  mPlayer1 = mApp.mAssetManager.AddImage("Player1", "resources/Player1.png",
      GQE::AssetLoadStyleImmediate);
  mPlayer2 = mApp.mAssetManager.AddImage("Player2", "resources/Player2.png",
      GQE::AssetLoadStyleImmediate);

  // Load our Empty square image which will be used when there is nothing to show
  mEmpty = mApp.mAssetManager.AddImage("Empty", "resources/Empty.png",
      GQE::AssetLoadStyleImmediate);

#if (SFML_VERSION_MAJOR < 2)
  // Setup winner text color as White
  mWinnerText.SetColor(sf::Color::White);
#else
  // Setup winner text color as White
  mWinnerText.setColor(sf::Color::White);
#endif

  // Call ReInit to reset the board
  ReInit();

  // Make sure our update loop is only called 30 times per second
  mApp.SetUpdateRate(30.0f);
}

void GameState::ReInit(void)
{
  // Reset our board
  for(GQE::Uint8 row = 0; row < 3; row++)
  {
    for(GQE::Uint8 col = 0; col < 3; col++)
    {
      // Reset the sprite for this square to empty
#if (SFML_VERSION_MAJOR < 2)
      mBoardSprite[row][col].SetImage(mEmpty->GetAsset());
      mBoardSprite[row][col].SetPosition((col*270.0f), (row*202.0f));
#else
      mBoardSprite[row][col].setTexture(mEmpty->GetAsset());
      mBoardSprite[row][col].setPosition((col*270.0f), (row*202.0f));
#endif

      // Set this squares owner to no player
      mBoardPlayer[row][col] = 0;
    }
  }

  // Set Cursor to Player 1 image
#if (SFML_VERSION_MAJOR < 2)
  mCursor.SetImage(mPlayer1->GetAsset());

  // Set Cursor scale to be 25% of original image
  mCursor.SetScale(0.25f, 0.25f);
#else
  mCursor.setTexture(mPlayer1->GetAsset());

  // Set Cursor scale to be 25% of original image
  mCursor.setScale(0.25f, 0.25f);
#endif

  // Set current player to player 1
  mCurrentPlayer = 1;

  // Reset our winner text
#if (SFML_VERSION_MAJOR < 2)
  mWinnerText.SetText("");
#else
  mWinnerText.setString("");
#endif
}

void GameState::HandleEvents(sf::Event theEvent)
{
  // Exit program if Escape key is pressed
#if (SFML_VERSION_MAJOR < 2)
  if((theEvent.Type == sf::Event::KeyReleased) && (theEvent.Key.Code == sf::Key::Escape))
#else
    if((theEvent.type == sf::Event::KeyReleased) && (theEvent.key.code == sf::Keyboard::Escape))
#endif
    {
      // Signal the application to exit
      mApp.Quit(GQE::StatusAppOK);
    }

#if (SFML_VERSION_MAJOR < 2)
  if(theEvent.Type == sf::Event::MouseButtonReleased)
#else
    if(theEvent.type == sf::Event::MouseButtonReleased)
#endif
    {
#if (SFML_VERSION_MAJOR < 2)
      // Determine which square they clicked on
      GQE::Uint8 col = (theEvent.MouseButton.X / 270);
      GQE::Uint8 row = (theEvent.MouseButton.Y / 202);
#else
      // Determine which square they clicked on
      GQE::Uint8 col = (theEvent.mouseButton.x / 270);
      GQE::Uint8 row = (theEvent.mouseButton.y / 202);
#endif
      if(3 > col && 3 > row && mBoardPlayer[row][col] == 0)
      {
        // Set ownership of this square to the current player
        mBoardPlayer[row][col] = mCurrentPlayer;

        // Determine which Sprite to use for this square
        switch(mCurrentPlayer)
        {
          case 1:
            // Set Player 1 image for this square
#if (SFML_VERSION_MAJOR < 2)
            mBoardSprite[row][col].SetImage(mPlayer1->GetAsset());
#else
            mBoardSprite[row][col].setTexture(mPlayer1->GetAsset());
#endif

            // Set Cursor to Player 2 image
#if (SFML_VERSION_MAJOR < 2)
            mCursor.SetImage(mPlayer2->GetAsset());
#else
            mCursor.setTexture(mPlayer2->GetAsset());
#endif

            // Switch to Player 2
            mCurrentPlayer = 2;
            break;
          case 2:
            // Set Player 2 image for this square
#if (SFML_VERSION_MAJOR < 2)
            mBoardSprite[row][col].SetImage(mPlayer2->GetAsset());
#else
            mBoardSprite[row][col].setTexture(mPlayer2->GetAsset());
#endif

            // Set Cursor to Player 1 image
#if (SFML_VERSION_MAJOR < 2)
            mCursor.SetImage(mPlayer1->GetAsset());
#else
            mCursor.setTexture(mPlayer1->GetAsset());
#endif

            // Switch to Player 1
            mCurrentPlayer = 1;
            break;
          default:
            // Leave as empty, we shouldn't even be here!
            break;
        }
      }

      // If current player is 0 then the last game ended, start a new game
      if(0 == mCurrentPlayer)
      {
        // Reinitialize the board and start a new game
        ReInit();
      }
    }
}

void GameState::UpdateFixed(void)
{
  // Start with a tie game
  GQE::Uint8 anWinner = 3;

  // Check each row to see if some player has won!
  for(GQE::Uint8 row = 0; row < 3; row++)
  {
    // Make sure each column matches and that its not the Empty player (0)
    if(mBoardPlayer[row][0] != 0 &&
        mBoardPlayer[row][0] == mBoardPlayer[row][1] &&
        mBoardPlayer[row][0] == mBoardPlayer[row][2])
    {
      // Make a note of which player is the winner!
      anWinner = mBoardPlayer[row][0];
      break;
    }

    // Check for empty columns on each row
    if(mBoardPlayer[row][0] == 0 ||
        mBoardPlayer[row][1] == 0 ||
        mBoardPlayer[row][2] == 0 &&
        anWinner == 3)
    {
      // No tie game, there are still empty spaces
      anWinner = 0;
    }
  }

  // Check each column to see if some player has won!
  for(GQE::Uint8 col = 0; col < 3; col++)
  {
    // Make sure each column matches and that its not the Empty player (0)
    if(mBoardPlayer[0][col] != 0 &&
        mBoardPlayer[0][col] == mBoardPlayer[1][col] &&
        mBoardPlayer[0][col] == mBoardPlayer[2][col])
    {
      // Make a note of which player is the winner!
      anWinner = mBoardPlayer[0][col];
      break;
    }
  }

  // Check diagonals
  if(mBoardPlayer[1][1] != 0 &&
      ((mBoardPlayer[1][1] == mBoardPlayer[0][0] && mBoardPlayer[1][1] == mBoardPlayer[2][2]) ||
       (mBoardPlayer[1][1] == mBoardPlayer[0][2] && mBoardPlayer[1][1] == mBoardPlayer[2][0])))
  {
    anWinner = mBoardPlayer[1][1];
  }

  // Did we find a winner?
  if(anWinner == 1)
  {
#if (SFML_VERSION_MAJOR < 2)
    mWinnerText.SetText("Player 1 Wins!");
    // Setup winner text in middle of screen
    mWinnerText.SetPosition(300.0f,280.0f);
#else
    mWinnerText.setString("Player 1 Wins!");
    // Setup winner text in middle of screen
    mWinnerText.setPosition(300.0f,280.0f);
#endif
  }
  else if(anWinner == 2)
  {
#if (SFML_VERSION_MAJOR < 2)
    mWinnerText.SetText("Player 2 Wins!");
    // Setup winner text in middle of screen
    mWinnerText.SetPosition(300.0f,280.0f);
#else
    mWinnerText.setString("Player 2 Wins!");
    // Setup winner text in middle of screen
    mWinnerText.setPosition(300.0f,280.0f);
#endif
  }
  else if(anWinner == 3)
  {
#if (SFML_VERSION_MAJOR < 2)
    mWinnerText.SetText("Tie Game");
    // Setup winner text in middle of screen
    mWinnerText.SetPosition(340.0f,280.0f);
#else
    mWinnerText.setString("Tie Game");
    // Setup winner text in middle of screen
    mWinnerText.setPosition(340.0f,280.0f);
#endif
  }

  // Set current player and cursor to empty if a winner was declared
  if(anWinner != 0)
  {
    // Set Cursor to Player 1 image
#if (SFML_VERSION_MAJOR < 2)
    mCursor.SetImage(mEmpty->GetAsset());
#else
    mCursor.setTexture(mEmpty->GetAsset());
#endif

    // Switch to empty (no player)
    mCurrentPlayer = 0;
  }
}

void GameState::UpdateVariable(float theElapsedTime)
{
  // Draw the current player image at the mouse position
#if (SFML_VERSION_MAJOR < 2)
  mCursor.SetPosition(mApp.mInput.GetMouseX()-32.0f, mApp.mInput.GetMouseY()-25.25f);
#else
  mCursor.setPosition(sf::Mouse::getPosition().x-32.0f, sf::Mouse::getPosition().y-25.25f);
#endif
}

void GameState::Draw(void)
{
#if (SFML_VERSION_MAJOR < 2)
  // Draw our Board sprite
  mApp.mWindow.Draw(mBackgroundSprite);
#else
  // Draw our Board sprite
  mApp.mWindow.draw(mBackgroundSprite);
#endif

  for(GQE::Uint8 row = 0; row < 3; row++)
  {
    for(GQE::Uint8 col = 0; col < 3; col++)
    {
#if (SFML_VERSION_MAJOR < 2)
      // Draw our Board
      mApp.mWindow.Draw(mBoardSprite[row][col]);
#else
      // Draw our Board
      mApp.mWindow.draw(mBoardSprite[row][col]);
#endif
    }
  }

#if (SFML_VERSION_MAJOR < 2)
  // Draw winner text
  mApp.mWindow.Draw(mWinnerText);

  // Draw our cursor
  mApp.mWindow.Draw(mCursor);
#else
  // Draw winner text
  mApp.mWindow.draw(mWinnerText);

  // Draw our cursor
  mApp.mWindow.draw(mCursor);
#endif
}

void GameState::Cleanup(void)
{
  // Unload our images since we don't need them anymore
  mApp.mAssetManager.UnloadImage("Board");
  mApp.mAssetManager.UnloadImage("Player1");
  mApp.mAssetManager.UnloadImage("Player2");
  mApp.mAssetManager.UnloadImage("Empty");

  // Last of all, call our base class implementation
  IState::Cleanup();
}

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
