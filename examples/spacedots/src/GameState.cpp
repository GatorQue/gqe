/**
 * Provides the SpaceDots GameState example in the GQE library.
 *
 * @file src/GameState.cpp
 * @author Ryan Lindeman
 * @date 20120323 - Initial Release
 * @date 20120421 - Make sure SFML 2 doesn't use its default font since it will crash on exit
 */
#include "GameState.hpp"
#include <GQE/Core/assets/FontAsset.hpp>
#include <GQE/Core/assets/ImageAsset.hpp>
#include <GQE/Core/assets/SoundAsset.hpp>
#include <GQE/Core/classes/App.hpp>

GameState::GameState(GQE::App& theApp) :
  GQE::IState("Game",theApp),
  mWinFont(NULL),
  mBackground(NULL),
  mEmptyHorizontal(NULL),
  mEmptyVertical(NULL),
  mEmptySquare(NULL),
  mBlueHorizontal(NULL),
  mBlueVertical(NULL),
  mBlueSquare(NULL),
  mBlueWinner(NULL),
  mRedHorizontal(NULL),
  mRedVertical(NULL),
  mRedSquare(NULL),
  mRedWinner(NULL),
  mCorner(NULL),
  mRedGain(NULL),
  mRedWin(NULL),
  mBlueGain(NULL),
  mBlueWin(NULL),
  mLightsaber(NULL),
  mSelectedCol(1),
  mSelectedRow(1),
  mCurrentPlayer(0),
  mWinnerText(NULL)
{
}

GameState::~GameState(void)
{
  delete mWinnerText;
  mWinnerText = NULL;

  mApp.mAssetManager.UnloadFont("WinFont");
}

void GameState::DoInit(void)
{
  // First call our base class implementation
  IState::DoInit();

  // Load our background image
  mBackground = mApp.mAssetManager.AddImage("Background",
      "resources/Background.png", GQE::AssetLoadStyleImmediate);

  // Assign our background image sprite texture
#if (SFML_VERSION_MAJOR < 2)
  mBackgroundSprite.SetImage(mBackground->GetAsset());
#else
  mBackgroundSprite.setTexture(mBackground->GetAsset());
#endif

  // Load our edge images which will be used when edge is not selected
  mEmptyHorizontal = mApp.mAssetManager.AddImage("EmptyHorizontal",
      "resources/EmptyHorizontal.png", GQE::AssetLoadStyleImmediate);
  mEmptyVertical = mApp.mAssetManager.AddImage("EmptyVertical",
      "resources/EmptyVertical.png", GQE::AssetLoadStyleImmediate);
  mEmptySquare = mApp.mAssetManager.AddImage("EmptySquare",
      "resources/EmptySquare.png", GQE::AssetLoadStyleImmediate);

  // Load our edge images which will be used for blue players turn
  mBlueHorizontal = mApp.mAssetManager.AddImage("BlueHorizontal",
      "resources/BlueHorizontal.png", GQE::AssetLoadStyleImmediate);
  mBlueVertical = mApp.mAssetManager.AddImage("BlueVertical",
      "resources/BlueVertical.png", GQE::AssetLoadStyleImmediate);
  mBlueSquare = mApp.mAssetManager.AddImage("BlueSquare",
      "resources/BlueSquare.png", GQE::AssetLoadStyleImmediate);
  mBlueWinner = mApp.mAssetManager.AddImage("BlueWinner",
      "resources/BlueWinner.png", GQE::AssetLoadStyleImmediate);

  // Load our edge image which will be used for red players turn
  mRedHorizontal = mApp.mAssetManager.AddImage("RedHorizontal",
      "resources/RedHorizontal.png", GQE::AssetLoadStyleImmediate);
  mRedVertical = mApp.mAssetManager.AddImage("RedVertical",
      "resources/RedVertical.png", GQE::AssetLoadStyleImmediate);
  mRedSquare = mApp.mAssetManager.AddImage("RedSquare",
      "resources/RedSquare.png", GQE::AssetLoadStyleImmediate);
  mRedWinner = mApp.mAssetManager.AddImage("RedWinner",
      "resources/RedWinner.png", GQE::AssetLoadStyleImmediate);

  // Load our Corner image which will be used in between edges
  mCorner = mApp.mAssetManager.AddImage("Corner", "resources/Corner.png",
      GQE::AssetLoadStyleImmediate);

  // Load our gain a square sound effects for each player
  mRedGain = mApp.mAssetManager.AddSound("RedGain", "resources/RedGain.ogg",
      GQE::AssetLoadStyleImmediate);
  mRedWin = mApp.mAssetManager.AddSound("RedWin", "resources/RedWin.ogg",
      GQE::AssetLoadStyleImmediate);
  mBlueGain = mApp.mAssetManager.AddSound("BlueGain", "resources/BlueGain.ogg",
      GQE::AssetLoadStyleImmediate);
  mBlueWin = mApp.mAssetManager.AddSound("BlueWin", "resources/BlueWin.ogg",
      GQE::AssetLoadStyleImmediate);

  // Load our Lightsaber sound effect
  mLightsaber = mApp.mAssetManager.AddSound("Lightsaber", "resources/Lightsaber.ogg",
      GQE::AssetLoadStyleImmediate);

#if (SFML_VERSION_MAJOR < 2)
  mRedGainSound.SetBuffer(mRedGain->GetAsset());
  mRedGainSound.SetVolume(25.0f);
  mRedWinSound.SetBuffer(mRedWin->GetAsset());
  mBlueGainSound.SetBuffer(mBlueGain->GetAsset());
  mBlueGainSound.SetVolume(25.0f);
  mBlueWinSound.SetBuffer(mBlueWin->GetAsset());
  mLightsaberSound.SetBuffer(mLightsaber->GetAsset());
  mLightsaberSound.SetVolume(10.0f);
#else
  mRedGainSound.setBuffer(mRedGain->GetAsset());
  mRedGainSound.setVolume(25.0f);
  mRedWinSound.setBuffer(mRedWin->GetAsset());
  mBlueGainSound.setBuffer(mBlueGain->GetAsset());
  mBlueGainSound.setVolume(25.0f);
  mBlueWinSound.setBuffer(mBlueWin->GetAsset());
  mLightsaberSound.setBuffer(mLightsaber->GetAsset());
  mLightsaberSound.setVolume(10.0f);
#endif

  // Load our win text font
  mWinFont = mApp.mAssetManager.AddFont("WinFont", "resources/WinFont.ttf",
      GQE::AssetLoadStyleImmediate);

#if (SFML_VERSION_MAJOR < 2)
  // Setup winner text color as Yellow
  mWinnerText = new sf::String("", mWinFont->GetAsset(), 30);
  mWinnerText->SetColor(sf::Color::Yellow);
#else
  // Setup winner text color as Yellow
  mWinnerText = new sf::Text("", mWinFont->GetAsset(), 30);
  mWinnerText->setColor(sf::Color::Yellow);
#endif

  // Call ReInit to reset the board
  ReInit();

  // Make sure our update loop is only called 30 times per second
  mApp.SetUpdateRate(30.0f);
}

void GameState::ReInit(void)
{
  // Reset our board egdes
  for(GQE::Uint8 row = 0; row < 19; row++)
  {
    for(GQE::Uint8 col = 0; col < 19; col++)
    {
      // Make this spot available
      mBoardPlayer[row][col] = 0;

      // Row: Even && Col: Even == Corner
      if((row % 2) == 0 && (col % 2) == 0)
      {
#if (SFML_VERSION_MAJOR < 2)
        mBoardSprites[row][col].SetImage(mCorner->GetAsset());
        mBoardSprites[row][col].SetPosition(2.0f+(col/2.0f)*(12.0f+75.0f),
            2.0f+(row/2.0f)*(9.0f+56.0f));
#else
        mBoardSprites[row][col].setTexture(mCorner->GetAsset());
        mBoardSprites[row][col].setPosition(2.0f+(col/2.0f)*(12.0f+75.0f),
            2.0f+(row/2.0f)*(9.0f+56.0f));
#endif
        // Make this corner unavailable
        mBoardPlayer[row][col] = 3;
      }
      // Row: Odd && Col: Odd == Square
      else if((row % 2) == 1 && (col % 2) == 1)
      {
#if (SFML_VERSION_MAJOR < 2)
        mBoardSprites[row][col].SetImage(mEmptySquare->GetAsset());
        mBoardSprites[row][col].SetPosition(2.0f+12.0f+((col-1)/2.0f)*(12.0f+75.0f),
            2.0f+9.0f+((row-1)/2.0f)*(9.0f+56.0f));
#else
        mBoardSprites[row][col].setTexture(mEmptySquare->GetAsset());
        mBoardSprites[row][col].setPosition(2.0f+12.0f+((col-1)/2.0f)*(12.0f+75.0f),
            2.0f+9.0f+((row-1)/2.0f)*(9.0f+56.0f));
#endif
      }
      // Row: Even && Col: Odd == Horizontal Edge
      else if((row % 2) == 0 && (col % 2) == 1)
      {
#if (SFML_VERSION_MAJOR < 2)
        mBoardSprites[row][col].SetImage(mEmptyHorizontal->GetAsset());
        mBoardSprites[row][col].SetPosition(2.0f+12.0f+((col-1)/2.0f)*(12.0f+75.0f),
            2.0f+(row/2.0f)*(9.0f+56.0f));
#else
        mBoardSprites[row][col].setTexture(mEmptyHorizontal->GetAsset());
        mBoardSprites[row][col].setPosition(2.0f+12.0f+((col-1)/2.0f)*(12.0f+75.0f),
            2.0f+(row/2.0f)*(9.0f+56.0f));
#endif
      }
      // Row: Odd && Col: Even == Vertical Edge
      else if((row % 2) == 1 && (col % 2) == 0)
      {
#if (SFML_VERSION_MAJOR < 2)
        mBoardSprites[row][col].SetImage(mEmptyVertical->GetAsset());
        mBoardSprites[row][col].SetPosition(2.0f+(col/2.0f)*(12.0f+75.0f),
            2.0f+9.0f+((row-1)/2.0f)*(9.0f+56.0f));
#else
        mBoardSprites[row][col].setTexture(mEmptyVertical->GetAsset());
        mBoardSprites[row][col].setPosition(2.0f+(col/2.0f)*(12.0f+75.0f),
            2.0f+9.0f+((row-1)/2.0f)*(9.0f+56.0f));
#endif
      }
    }
  }

  // Set current player to player 1
  mCurrentPlayer = 1;

  // Set selected row and column to known values
  mSelectedCol = 1;
  mSelectedRow = 1;

  // Reset our winner text
#if (SFML_VERSION_MAJOR < 2)
  mWinnerText->SetText("");
#else
  mWinnerText->setString("");
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

  // Update selected edge if mouse moved
#if (SFML_VERSION_MAJOR < 2)
  if(theEvent.Type == sf::Event::MouseMoved)
#else
    if(theEvent.type == sf::Event::MouseMoved)
#endif
    {
      UpdateSelected(theEvent);
    }

#if (SFML_VERSION_MAJOR < 2)
  if(theEvent.Type == sf::Event::MouseButtonReleased)
#else
    if(theEvent.type == sf::Event::MouseButtonReleased)
#endif
    {
      // If current player is 0 then the last game ended, start a new game
      if(0 == mCurrentPlayer)
      {
        // Reinitialize the board and start a new game
        ReInit();
      }
      else
      {
        // Player has selected an edge
        SelectEdge();
      }
    }
}

void GameState::UpdateSelected(sf::Event theEvent)
{
#if (SFML_VERSION_MAJOR < 2)
  // Determine which square they clicked on
  GQE::Uint16 col = (GQE::Uint16)(theEvent.MouseMove.X / ((12+75)/2));
  GQE::Uint16 row = (GQE::Uint16)(theEvent.MouseMove.Y / ((9+56)/2));
#else
  // Determine which square they clicked on
  GQE::Uint16 col = (GQE::Uint16)(theEvent.mouseMove.x / ((12+75)/2));
  GQE::Uint16 row = (GQE::Uint16)(theEvent.mouseMove.y / ((9+56)/2));
#endif

  // Update board selection image if different
  if((mBoardPlayer[mSelectedRow][mSelectedCol] == 0 &&
        mBoardPlayer[row][col] == 0) &&
      (row != mSelectedRow || col != mSelectedCol))
  {
    // Clear previous selected edge image
    // Row: Even && Col: Odd == Horizontal Edge
    if((mSelectedRow % 2) == 0 && (mSelectedCol % 2) == 1)
    {
#if (SFML_VERSION_MAJOR < 2)
      mBoardSprites[mSelectedRow][mSelectedCol].SetImage(
          mEmptyHorizontal->GetAsset());
#else
      mBoardSprites[mSelectedRow][mSelectedCol].setTexture(
          mEmptyHorizontal->GetAsset());
#endif
    }
    // Row: Odd && Col: Even == Vertical Edge
    else if((mSelectedRow % 2) == 1 && (mSelectedCol % 2) == 0)
    {
#if (SFML_VERSION_MAJOR < 2)
      mBoardSprites[mSelectedRow][mSelectedCol].SetImage(
          mEmptyVertical->GetAsset());
#else
      mBoardSprites[mSelectedRow][mSelectedCol].setTexture(
          mEmptyVertical->GetAsset());
#endif
    }
    else
    {
      /* Corner or square, do nothing */
    }

    // Set new row and column image
    // Row: Even && Col: Odd == Horizontal Edge
    if((row % 2) == 0 && (col % 2) == 1)
    {
      if(mCurrentPlayer == 1)
      {
#if (SFML_VERSION_MAJOR < 2)
        mBoardSprites[row][col].SetImage(
            mBlueHorizontal->GetAsset());
#else
        mBoardSprites[row][col].setTexture(
            mBlueHorizontal->GetAsset());
#endif
      }
      else
      {
#if (SFML_VERSION_MAJOR < 2)
        mBoardSprites[row][col].SetImage(
            mRedHorizontal->GetAsset());
#else
        mBoardSprites[row][col].setTexture(
            mRedHorizontal->GetAsset());
#endif
      }
    }
    // Row: Odd && Col: Even == Vertical Edge
    else if((row % 2) == 1 && (col % 2) == 0)
    {
      if(mCurrentPlayer == 1)
      {
#if (SFML_VERSION_MAJOR < 2)
        mBoardSprites[row][col].SetImage(
            mBlueVertical->GetAsset());
#else
        mBoardSprites[row][col].setTexture(
            mBlueVertical->GetAsset());
#endif
      }
      else
      {
#if (SFML_VERSION_MAJOR < 2)
        mBoardSprites[row][col].SetImage(
            mRedVertical->GetAsset());
#else
        mBoardSprites[row][col].setTexture(
            mRedVertical->GetAsset());
#endif
      }
    }
    else
    {
      /* Corner or square, do nothing */
    }

    // Update our mSelectedRow and mSelectedCol values
    mSelectedRow = row;
    mSelectedCol = col;
  }
}

void GameState::SelectEdge(void)
{
  // Row: Even && Col: Odd == Horizontal Edge
  if((mSelectedRow % 2) == 0 && (mSelectedCol % 2) == 1)
  {
    // Mark this edge as selected
    mBoardPlayer[mSelectedRow][mSelectedCol] = 1;

    if(mCurrentPlayer == 1)
    {
#if (SFML_VERSION_MAJOR < 2)
      mBoardSprites[mSelectedRow][mSelectedCol].SetImage(
          mBlueHorizontal->GetAsset());
#else
      mBoardSprites[mSelectedRow][mSelectedCol].setTexture(
          mBlueHorizontal->GetAsset());
#endif
    }
    else
    {
#if (SFML_VERSION_MAJOR < 2)
      mBoardSprites[mSelectedRow][mSelectedCol].SetImage(
          mRedHorizontal->GetAsset());
#else
      mBoardSprites[mSelectedRow][mSelectedCol].setTexture(
          mRedHorizontal->GetAsset());
#endif
    }

#if (SFML_VERSION_MAJOR < 2)
    // Only play if not already playing a sound effect
    if(sf::Sound::Playing != mLightsaberSound.GetStatus())

    {

      mLightsaberSound.Play();

    }
#else
    // Only play if not already playing a sound effect
    if(sf::Sound::Playing != mLightsaberSound.getStatus())

    {
      mLightsaberSound.play();

    }
#endif

  }
  // Row: Odd && Col: Even == Vertical Edge
  else if((mSelectedRow % 2) == 1 && (mSelectedCol % 2) == 0)
  {
    // Mark this edge as selected
    mBoardPlayer[mSelectedRow][mSelectedCol] = 1;

    if(mCurrentPlayer == 1)
    {
#if (SFML_VERSION_MAJOR < 2)
      mBoardSprites[mSelectedRow][mSelectedCol].SetImage(
          mBlueVertical->GetAsset());
#else
      mBoardSprites[mSelectedRow][mSelectedCol].setTexture(
          mBlueVertical->GetAsset());
#endif
    }
    else
    {
#if (SFML_VERSION_MAJOR < 2)
      mBoardSprites[mSelectedRow][mSelectedCol].SetImage(
          mRedVertical->GetAsset());
#else
      mBoardSprites[mSelectedRow][mSelectedCol].setTexture(
          mRedVertical->GetAsset());
#endif
    }
#if (SFML_VERSION_MAJOR < 2)
    // Only play if not already playing a sound effect
    if(sf::Sound::Playing != mLightsaberSound.GetStatus())
    {
      mLightsaberSound.Play();
    }

#else

    // Only play if not already playing a sound effect
    if(sf::Sound::Playing != mLightsaberSound.getStatus())
    {
      mLightsaberSound.play();
    }

#endif

  }
  else
  {
    /* Corner or square, return to caller */
    return;
  }

  // Find all completed squares and look for end game
  bool anChangePlayers = true;
  bool anGameOver = true;
  GQE::Uint8 anScore[2] = {0,0};
  for(GQE::Uint8 row = 1; row < 18; row+=2)
  {
    for(GQE::Uint8 col = 1; col < 18; col+=2)
    {
      if(mBoardPlayer[row][col] == 0)
      {
        if(mBoardPlayer[row-1][col] == 1 &&
            mBoardPlayer[row][col-1] == 1 &&
            mBoardPlayer[row+1][col] == 1 &&
            mBoardPlayer[row][col+1] == 1)
        {
          // Update image for this square
          if(mCurrentPlayer == 1)
          {
#if (SFML_VERSION_MAJOR < 2)
            mBoardSprites[row][col].SetImage(
                mBlueSquare->GetAsset());
#else
            mBoardSprites[row][col].setTexture(
                mBlueSquare->GetAsset());
#endif
          }
          else
          {
#if (SFML_VERSION_MAJOR < 2)
            mBoardSprites[row][col].SetImage(
                mRedSquare->GetAsset());
#else
            mBoardSprites[row][col].setTexture(
                mRedSquare->GetAsset());
#endif
          }

          // Assign this square to the current player
          mBoardPlayer[row][col] = mCurrentPlayer;

          // Add this square to the current players score
          anScore[mCurrentPlayer - 1]++;

          // Give this player another turn
          anChangePlayers = false;
        }
        else
        {
          // Game is not over
          anGameOver = false;

          // Reset selected row and column to this row and column
          mSelectedRow = row;
          mSelectedCol = col;
        }
      }
      else
      {
        // Count the score for each player
        anScore[mBoardPlayer[row][col] - 1]++;
      }
    }
  }

  // Is game not over? then see if we need to switch players
  if(!anGameOver)
  {
    // Didn't complete a square? then change players now
    if(anChangePlayers)
    {
      if(mCurrentPlayer == 1)
      {
        mCurrentPlayer = 2;
      }
      else
      {
        mCurrentPlayer = 1;
      }
    }
    else
    {
#if SFML_VERSION_MAJOR<2
      // Play appropriate sound
      if(mCurrentPlayer == 1)
      {
        // Only play if not currently playing this sound effect
        if(sf::Sound::Playing != mBlueGainSound.GetStatus())
        {
          mBlueGainSound.Play();
        }
      }
      else
      {
        // Only play if not currently playing this sound effect
        if(sf::Sound::Playing != mRedGainSound.GetStatus())
        {
          mRedGainSound.Play();
        }
      }
#else
    // Play appropriate sound
      if(mCurrentPlayer == 1)
      {
        // Only play if not currently playing this sound effect
        if(sf::Sound::Playing != mBlueGainSound.getStatus())
        {
          mBlueGainSound.play();
        }
      }
      else
      {
        // Only play if not currently playing this sound effect
        if(sf::Sound::Playing != mRedGainSound.getStatus())
        {
          mRedGainSound.play();
        }
      }
#endif
    }
  }
  else
  {
    // Determine which theme to play at end of game
    if(anScore[0] > anScore[1])
    {


      // Set correct winner image to be displayed
#if (SFML_VERSION_MAJOR < 2)
      mBlueWinSound.Play();
      mWinnerSprite.SetImage(mBlueWinner->GetAsset());
      mWinnerSprite.SetPosition(0.0f,0.0f);
#else
      mBlueWinSound.play();
      mWinnerSprite.setTexture(mBlueWinner->GetAsset());
      mWinnerSprite.setPosition(0.0f,0.0f);
#endif
    }
    else
    {


      // Set correct winner image to be displayed
#if (SFML_VERSION_MAJOR < 2)
      mRedWinSound.Play();
      mWinnerSprite.SetImage(mRedWinner->GetAsset());
      mWinnerSprite.SetPosition(0.0f,0.0f);
#else
      mRedWinSound.play();
      mWinnerSprite.setTexture(mRedWinner->GetAsset());
      mWinnerSprite.setPosition(0.0f,0.0f);
#endif
    }

#if (SFML_VERSION_MAJOR < 2)
    mWinnerText->SetText("Click button to play again!");

    // Setup winner text in middle of screen
    mWinnerText->SetPosition(110.0f,540.0f);
#else
    mWinnerText->setString("Click button to play again!");

    // Setup winner text in middle of screen
    mWinnerText->setPosition(110.0f,540.0f);
#endif

    // Switch to no player (wait until mouse clicks to reset game)
    mCurrentPlayer = 0;
  }
}

void GameState::UpdateFixed(void)
{
}

void GameState::UpdateVariable(float theElapsedTime)
{
}

void GameState::Draw(void)
{
  // Draw our Background
#if (SFML_VERSION_MAJOR < 2)
  mApp.mWindow.Draw(mBackgroundSprite);
#else
  mApp.mWindow.draw(mBackgroundSprite);
#endif

  // Draw our Board
  for(GQE::Uint8 row = 0; row < 19; row++)
  {
    for(GQE::Uint8 col = 0; col < 19; col++)
    {
      // Draw our Board
#if (SFML_VERSION_MAJOR < 2)
      mApp.mWindow.Draw(mBoardSprites[row][col]);
#else
      mApp.mWindow.draw(mBoardSprites[row][col]);
#endif
    }
  }

  // Show winner image
  if(mCurrentPlayer == 0)
  {
#if (SFML_VERSION_MAJOR < 2)
    mApp.mWindow.Draw(mWinnerSprite);
#else
    mApp.mWindow.draw(mWinnerSprite);
#endif
  }

#if (SFML_VERSION_MAJOR < 2)
  // Draw winner text
  mApp.mWindow.Draw(*mWinnerText);
#else
  // Draw winner text
  mApp.mWindow.draw(*mWinnerText);
#endif

}

void GameState::Cleanup(void)
{
  // Unload our images since we don't need them anymore
  mApp.mAssetManager.UnloadImage("EmptyVertical");
  mApp.mAssetManager.UnloadImage("EmptyHorizontal");
  mApp.mAssetManager.UnloadImage("EmptySquare");
  mApp.mAssetManager.UnloadImage("BlueVertical");
  mApp.mAssetManager.UnloadImage("BlueHorizontal");
  mApp.mAssetManager.UnloadImage("BlueSquare");
  mApp.mAssetManager.UnloadImage("RedVertical");
  mApp.mAssetManager.UnloadImage("RedHorizontal");
  mApp.mAssetManager.UnloadImage("RedSquare");
  mApp.mAssetManager.UnloadImage("Corner");

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
