/**
 * Provides the MenuState in the GQE namespace which is typically
 * the first state to be loaded for an application.
 *
 * @file src/GQE/Core/states/MenuState.hpp
 * @author Ryan Lindeman
 * @date 20100710 - Initial Release
 * @date 20110127 - Moved to GQE Core library and src directory
 */
#include "GQE/Core/states/MenuState.hpp"
#include "GQE/Core/classes/App.hpp"
#include "GQE/Core/assets/FontAsset.hpp"

namespace GQE
{
  MenuState::MenuState(App* theApp) :
    IState("Menu", theApp),
    mMenuFont(NULL),
    mMenuSprite(NULL),
    mMenuString1(NULL),
    mMenuString2(NULL)
  {
  }

  MenuState::~MenuState(void)
  {
  }

  void MenuState::DoInit(void)
  {
    // First call our base class implementation
    IState::DoInit();
    
    // Check our App pointer
    assert(NULL != mApp && "MenuState::DoInit() bad app pointer");

    // Load our arial font
    mMenuFont = mApp->mAssetManager.AddFont("arial", "arial.ttf", AssetLoadStyleImmediate)->GetAsset();

    // Create our String
    mMenuString1 = new sf::String("Play Game", *mMenuFont);
    assert(NULL != mMenuString1 && "MenuState::DoInit() can't allocate memory for string");

    mMenuString2 = new sf::String("Exit", *mMenuFont);
    assert(NULL != mMenuString2 && "MenuState::DoInit() can't allocate memory for string");

    // Position the string
    mMenuString1->SetColor(sf::Color(255,255,255));
    mMenuString1->Move(400.f, 300.f);

    mMenuString2->SetColor(sf::Color(255,255,255));
    mMenuString2->Move(400.f, 400.f);

    // Load our splash image
    mApp->mAssetManager.AddImage("Menu", "MenuImage.png", AssetLoadStyleImmediate);

    // Retrieve a sprite to the above image
    mMenuSprite = mApp->mAssetManager.GetSprite("Menu");
  }

  void MenuState::HandleEvents(sf::Event theEvent)
  {
    // Escape key pressed
    if ((theEvent.Type == sf::Event::KeyPressed) && (theEvent.Key.Code == sf::Key::Escape))
    {
      if(NULL != mApp)
      {
        mApp->Quit(StatusAppOK);
      }
    }
  }

  void MenuState::Update(void)
  {
    // Check our App pointer
    assert(NULL != mApp && "MenuState::Update() bad app pointer, init must be called first");
  }

  void MenuState::Draw(void)
  {
    // Check our App pointer
    assert(NULL != mApp && "MenuState::Draw() bad app pointer, init must be called first");

    // Draw our Splash sprite
    mApp->mWindow.Draw(*mMenuSprite);

    // Draw our Menu String in the middle of the screen
    mApp->mWindow.Draw(*mMenuString2);
  }

  void MenuState::Cleanup(void)
  {
    // Delete our sprite
    delete mMenuSprite;
    mMenuSprite = NULL;

    // Delete our string
    delete mMenuString1;
    mMenuString1 = NULL;
    delete mMenuString2;
    mMenuString2 = NULL;

    // Unload our font since we don't need it anymore
    mApp->mAssetManager.UnloadFont("arial");

    // Unload our image since we don't need it anymore
    mApp->mAssetManager.UnloadImage("Menu");

    // Last of all, call our base class implementation
    IState::Cleanup();
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
