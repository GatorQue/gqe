/**
 * Provides the StateManager class in the GQE namespace which is responsible
 * for providing the State management facilities to the App base class used
 *  in the GQE core library.
 *
 * @file include/GQE/Core/classes/StateManager.hpp
 * @author Ryan Lindeman
 * @date 20100728 - Initial Release
 * @date 20110120 - Add ability to add inactive states
 * @date 20110120 - Add ability to drop active state as inactive state
 * @date 20110125 - IState::HandleCleanup is now called from here
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Change mDropped to mDead to remove potential confusion
 * @date 20110218 - Added InactivateActiveState and ResetActiveState methods
 */
#ifndef   CORE_STATE_MANAGER_HPP_INCLUDED
#define   CORE_STATE_MANAGER_HPP_INCLUDED
 
#include <vector>
#include <string>
#include "GQE/Core/Core_types.hpp"
 
namespace GQE
{
  /// Provides game state manager class for managing game states
  class GQE_API StateManager
  {
  public:
    /**
     * StateManager constructor
     */
    StateManager();
 
    /**
     * StateManager deconstructor
     */
    virtual ~StateManager();
 
    /**
     * RegisterApp will register a pointer to the App class so it can be used
     * by the StateManager for error handling and log reporting.
     * @param[in] theApp is a pointer to the App (or App derived) class
     */
    void RegisterApp(App* theApp);
 
    /**
     * IsEmpty will return true if there are no active states on the stack.
     * @return true if state stack is empty, false otherwise.
     */
    bool IsEmpty(void);
 
    /**
     * AddActiveState will add theState provided as the current active
     * state.
     * @param[in] theState to set as the current state
     */
    void AddActiveState(IState* theState);
 
    /**
     * AddInactiveState will add theState provided as an inactive state.
     * @param[in] theState to be added as an inactive state
     */
    void AddInactiveState(IState* theState);

    /**
     * GetActiveState will return the current active state on the stack.
     * @return pointer to the current active state on the stack
     */
    IState* GetActiveState(void);

    /**
     * InactivateActiveState will cause the current active state to
     * become an inactive state without uninitializing its assets (doesn't
     * call DeInit) and return to the previous state on the stack. This
     * will cause the state to retain its assets.
     */
    void InactivateActivateState(void);

    /**
     * DropActiveState will cause the current active state to uninitialize
     * (calls DeInit) and become an inactive state and return to the
     * previous state on the stack. When a state is uninitialized its
     * assets are unloaded.
     */
    void DropActiveState(void);
 
    /**
     * ResetActiveState will cause the current active state to be reset
     * by calling the ReInit method for that state. This is useful for
     * "Play Again Y/N?" scenarios.
     */
    void ResetActiveState(void);

    /**
     * RemoveActiveState will cause the current active state to be removed
     * and return to the previous state on the stack.  Once a state has
     * been removed, you must re-add the state.  If you just want to
     * inactivate the current active state then use DropActiveState instead.
     */
    void RemoveActiveState(void);

    /**
     * SetActiveState will find the state specified by theStateID and make
     * it the current active state and move the previously current active
     * state as the next state.
     * @param[in] theStateID is the ID of the State to make active
     */
    void SetActiveState(typeStateID theStateID);
 
    /**
     * HandleCleanup is responsible for dealing with the cleanup of recently
     * dropped states.
     */
    void HandleCleanup(void);

  private:
    // Constants
    ///////////////////////////////////////////////////////////////////////////
 
    // Variables
    ///////////////////////////////////////////////////////////////////////////
    /// Pointer to the App class for error handling and logging
    App*                  mApp;
    /// Stack to store the current and previously active states
    std::vector<IState*>  mStack;
    /// Stack to store the dead states until they properly cleaned up
    std::vector<IState*>  mDead;
 
    /**
     * StateManager copy constructor is private because we do not allow copies
     * of our class
     */
    StateManager(const StateManager&); // Intentionally undefined
 
    /**
     * Our assignment operator is private because we do not allow copies
     * of our class
     */
    StateManager& operator=(const StateManager&); // Intentionally undefined
 
  }; // class StateManager
}; // namespace GQE
 
#endif // CORE_STATE_MANAGER_HPP_INCLUDED

/**
 * @class GQE::StateManager
 * @ingroup Core
 * The StateManager class is used by the App class to manage all game
 * states. A game state represents a distinct part of the game flow and
 * often includes the following: game transitions, splash screens,
 * menu screens (not to be confused with dialog windows), loading files
 * screens, high score screens, etc.  You can also think of the game
 * state as a realization of an entire game or level if desired. The
 * StateManager is responsible for switching between game states and
 * facilitating game state management (adding, deleting, inactivating,
 * pausing, resuming, etc).
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
