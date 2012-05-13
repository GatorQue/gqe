/**
 * Provides the MenuState in the GQE namespace which is typically
 * the first state to be loaded for an application.
 *
 * @file src/GQE/Core/states/MenuState.hpp
 * @author Ryan Lindeman
 * @date 20100710 - Initial Release
 * @date 20110127 - Moved to GQE Core library and src directory
 * @date 20110218 - Added ReInit method
 * @date 20110218 - Change to system include style
 * @date 20110625 - Added UpdateVariable and changed Update to UpdateFixed
 * @date 20110627 - Removed extra ; from namespace
 * @date 20110721 - Remove * from GetAsset() calls since it now returns TYPE&
 * @date 20110831 - Support new SFML2 snapshot changes
 * @date 20110906 - Change mApp from a pointer to an address reference
 * @date 20120322 - Support new SFML2 snapshot changes
 * @date 20120512 - Renamed App to IApp since it really is just an interface
 */
#include <assert.h>
#include <GQE/Core/interfaces/IApp.hpp>
#include <GQE/Core/states/MenuState.hpp>
#include <GQE/Core/assets/FontAsset.hpp>

namespace GQE
{
  MenuState::MenuState(IApp& theApp) :
    IState("Menu", theApp),
    mDefaultFont("resources/arial.ttf", true),
    mMenuImage("resources/menu.png", true),
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

    // Create our String
#if (SFML_VERSION_MAJOR < 2)
    mMenuString1 = new sf::String("Play Game", mDefaultFont.GetAsset());
#else
    mMenuString1 = new sf::Text("Play Game", mDefaultFont.GetAsset());
#endif
    assert(NULL != mMenuString1 && "MenuState::DoInit() can't allocate memory for string");

#if (SFML_VERSION_MAJOR < 2)
    mMenuString2 = new sf::String("Exit", mDefaultFont.GetAsset());
#else
    mMenuString2 = new sf::Text("Exit", mDefaultFont.GetAsset());
#endif
    assert(NULL != mMenuString2 && "MenuState::DoInit() can't allocate memory for string");

#if (SFML_VERSION_MAJOR < 2)
    // Position the string
    mMenuString1->SetColor(sf::Color(255,255,255));
    mMenuString1->Move(400.f, 300.f);

    mMenuString2->SetColor(sf::Color(255,255,255));
    mMenuString2->Move(400.f, 400.f);
#else
    // Position the string
    mMenuString1->setColor(sf::Color(255,255,255));
    mMenuString1->move(400.f, 300.f);

    mMenuString2->setColor(sf::Color(255,255,255));
    mMenuString2->move(400.f, 400.f);
#endif

    // Show the menu image using the Menu sprite
    mMenuSprite = new sf::Sprite(mMenuImage.GetAsset());
  }

  void MenuState::ReInit(void)
  {
    // Do nothing yet
  }

  void MenuState::HandleEvents(sf::Event theEvent)
  {
    // Escape key pressed
#if (SFML_VERSION_MAJOR < 2)
    if ((theEvent.Type == sf::Event::KeyPressed) && (theEvent.Key.Code == sf::Key::Escape))
#else
      if ((theEvent.type == sf::Event::KeyPressed) && (theEvent.key.code == sf::Keyboard::Escape))
#endif
      {
        mApp.Quit(StatusAppOK);
      }
  }

  void MenuState::UpdateFixed(void)
  {
  }

  void MenuState::UpdateVariable(float theElapsedTime)
  {
  }

  void MenuState::Draw(void)
  {
#if (SFML_VERSION_MAJOR < 2)
    // Draw our Splash sprite
    mApp.mWindow.Draw(*mMenuSprite);

    // Draw our Menu String in the middle of the screen
    mApp.mWindow.Draw(*mMenuString2);
#else
    // Draw our Splash sprite
    mApp.mWindow.draw(*mMenuSprite);

    // Draw our Menu String in the middle of the screen
    mApp.mWindow.draw(*mMenuString2);
#endif
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

    // Last of all, call our base class implementation
    IState::Cleanup();
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
