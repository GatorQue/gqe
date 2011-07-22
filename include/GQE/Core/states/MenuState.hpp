/**
 * Provides the MenuState in the GQE namespace which is typically
 * the second state to be loaded for an application.
 *
 * @file include/GQE/Core/states/MenuState.hpp
 * @author Ryan Lindeman
 * @date 20100717 - Initial Release
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Added ReInit method
 * @date 20110218 - Change to system include style
 * @date 20110625 - Added UpdateVariable and changed Update to UpdateFixed
 * @date 20110627 - Removed extra ; from namespace
 */
#ifndef   CORE_MENU_STATE_HPP_INCLUDED
#define   CORE_MENU_STATE_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/interfaces/IState.hpp>
#include <SFML/System.hpp>

namespace GQE
{
  /// Provides simple Menu game state
  class GQE_API MenuState : public IState
  {
  public:
    /**
     * MenuState constructor
     * @param[in] theApp is a pointer to the App class.
     */
    MenuState(App* theApp);

    /**
     * MenuState deconstructor
     */
    virtual ~MenuState(void);

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
    sf::Sprite*         mMenuSprite;
#if (SFML_VERSION_MAJOR < 2)
    sf::String*         mMenuString1;
    sf::String*         mMenuString2;
#else
    sf::Text*           mMenuString1;
    sf::Text*           mMenuString2;
#endif
  }; // class MenuState
} // namespace GQE

#endif // CORE_MENU_STATE_HPP_INCLUDED
/**
 * @class GQE::MenuState
 * @ingroup Core
 * The MenuState class provides a simple example of using the DialogManager
 * to create a main menu for doing the following: launching the game,
 * changing game application options, showing the high score, changing
 * players, etc.
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
