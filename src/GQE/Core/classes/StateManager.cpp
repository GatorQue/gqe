/**
 * Provides the StateManager class in the GQE namespace which is responsible
 * for providing the State management facilities to the App base class used in
 * the GQE core library.
 *
 * @file src/GQE/Core/classes/StateManager.cpp
 * @author Ryan Lindeman
 * @date 20100728 - Initial Release
 * @date 20110120 - Added ability to add inactive states and initialize them
 *                  when they become active and send Dialog signal events.
 * @date 20110120 - Add ability to drop active state as inactive state
 * @date 20110125 - IState::HandleCleanup is now called from here
 * @date 20110127 - Moved to GQE Core library and src directory
 * @date 20110218 - Change mDropped to mDead to remove potential confusion
 * @date 20110218 - Added InactivateActiveState and ResetActiveState methods
 * @date 20110218 - Change to system include style
 * @date 20110611 - Convert logging to new Log macros
 */

#include <assert.h>
#include <stddef.h>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/classes/StateManager.hpp>
#include <GQE/Core/classes/App.hpp>
#include <GQE/Core/interfaces/IState.hpp>

namespace GQE
{
  StateManager::StateManager() :
    mApp(NULL)
  {
    ILOGM("StateManager::ctor()");
  }

  StateManager::~StateManager()
  {
    ILOGM("StateManager::dtor()");

    // Drop all active states
    while(!mStack.empty())
    {
      // Retrieve the currently active state
      IState* anState = mStack.back();
 
      // Pop the currently active state off the stack
      mStack.pop_back();
 
      // Pause the currently active state
      anState->Pause();
 
      // De-initialize the state
      anState->DeInit();

      // Handle the cleanup before we pop it off the stack
      anState->HandleCleanup();
 
      // Just delete the state now
      delete anState;

      // Don't keep pointers around we don't need
      anState = NULL;
    }

    // Delete all our dropped states
    while(!mDead.empty())
    {
      // Retrieve the currently active state
      IState* anState = mDead.back();
 
      // Pop the currently active state off the stack
      mDead.pop_back();
 
      // Pause the currently active state
      anState->Pause();
 
      // De-initialize the state
      anState->DeInit();
 
      // Handle the cleanup before we pop it off the stack
      anState->HandleCleanup();
 
      // Just delete the state now
      delete anState;

      // Don't keep pointers around we don't need
      anState = NULL;
    }
 
    // Clear pointers we don't need anymore
    mApp = NULL;
  }

  void StateManager::RegisterApp(App* theApp)
  {
    // Check that our pointer is good
    assert(NULL != theApp && "StateManager::RegisterApp() theApp pointer provided is bad");

    // Make a note of the pointer
    assert(NULL == mApp && "StateManager::RegisterApp() theApp pointer was already registered");
    mApp = theApp;
  }

  bool StateManager::IsEmpty(void)
  {
    return mStack.empty();
  }

  void StateManager::AddActiveState(IState* theState)
  {
    // Check that they didn't provide a bad pointer
    assert(NULL != theState && "StateManager::AddActiveState() received a bad pointer");

    // Log the adding of each state
    ILOG() << "StateManager::AddActiveState(" << theState->GetID() << ")" << std::endl;
 
    // Is there a state currently running? then Pause it
    if(!mStack.empty())
    {
      // Pause the currently running state since we are changing the
      // currently active state to the one provided
      mStack.back()->Pause();
    }

    // Add the active state
    mStack.push_back(theState);
 
    // Initialize the new active state
    mStack.back()->DoInit();
  }

  void StateManager::AddInactiveState(IState* theState)
  {
    // Check that they didn't provide a bad pointer
    assert(NULL != theState && "StateManager::AddInactiveState() received a bad pointer");

    // Log the adding of each state
    ILOG() << "StateManager::AddInactiveState(" << theState->GetID() << ")" << std::endl;

    // Add the inactive state to the bottom of the stack
    mStack.insert(mStack.begin(), theState);
  }

  IState* StateManager::GetActiveState(void)
  {
    return mStack.back();
  }

  void StateManager::InactivateActivateState(void)
  {
    // Is there no currently active state to drop?
    if(!mStack.empty())
    {
      // Retrieve the currently active state
      IState* anState = mStack.back();
 
      // Log the inactivating an active state
      ILOG() << "StateManager::InactivateActiveState(" << anState->GetID() << ")" << std::endl;
 
      // Pause the currently active state
      anState->Pause();

      // Pop the currently active state off the stack
      mStack.pop_back();
 
      // Move this now inactive state to the absolute back of our stack
      mStack.insert(mStack.begin(), anState);
 
      // Don't keep pointers around we don't need anymore
      anState = NULL;
    }
    else
    {
      // Quit the application with an error status response
      if(NULL != mApp)
      {
        mApp->Quit(StatusAppStackEmpty);
      }
      return;
    }
 
    // Is there another state to activate? then call Resume to activate it
    if(!mStack.empty())
    {
      // Has this state ever been initialized?
      if(mStack.back()->IsInitComplete())
      {
        // Resume the new active state
        mStack.back()->Resume();
      }
      else
      {
        // Initialize the new active state
        mStack.back()->DoInit();
      }
    }
    else
    {
      // There are no states on the stack, exit the program
      if(NULL != mApp)
      {
        mApp->Quit(StatusAppOK);
      }
    }
  }

  void StateManager::DropActiveState(void)
  {
    // Is there no currently active state to drop?
    if(!mStack.empty())
    {
      // Retrieve the currently active state
      IState* anState = mStack.back();

      // Log the dropping of an active state
      ILOG() << "StateManager::DropActiveState(" << anState->GetID() << ")" << std::endl;

      // Pause the currently active state
      anState->Pause();

      // Deinit currently active state before we pop it off the stack
      // (HandleCleanup() will be called by IState::DoInit() method if this
      // state is ever set active again)
      anState->DeInit();
 
      // Pop the currently active state off the stack
      mStack.pop_back();
 
      // Move this now inactive state to the absolute back of our stack
      mStack.insert(mStack.begin(), anState);
 
      // Don't keep pointers around we don't need anymore
      anState = NULL;
    }
    else
    {
      // Quit the application with an error status response
      if(NULL != mApp)
      {
        mApp->Quit(StatusAppStackEmpty);
      }
      return;
    }

    // Is there another state to activate? then call Resume to activate it
    if(!mStack.empty())
    {
      // Has this state ever been initialized?
      if(mStack.back()->IsInitComplete())
      {
        // Resume the new active state
        mStack.back()->Resume();
      }
      else
      {
        // Initialize the new active state
        mStack.back()->DoInit();
      }
    }
    else
    {
      // There are no states on the stack, exit the program
      if(NULL != mApp)
      {
        mApp->Quit(StatusAppOK);
      }
    }
  }

  void StateManager::ResetActiveState(void)
  {
    // Is there no currently active state to reset?
    if(!mStack.empty())
    {
      // Retrieve the currently active state
      IState* anState = mStack.back();
 
      // Log the resetting of an active state
      ILOG() << "StateManager::ResetActiveState(" << anState->GetID() << ")" << std::endl;
 
      // Pause the currently active state
      anState->Pause();
 
      // Call the ReInit method to Reset the currently active state
      anState->ReInit();
 
      // Resume the currently active state
      anState->Resume();
 
      // Don't keep pointers around we don't need anymore
      anState = NULL;
    }
    else
    {
      // Quit the application with an error status response
      if(NULL != mApp)
      {
        mApp->Quit(StatusAppStackEmpty);
      }
      return;
    }
  }

  void StateManager::RemoveActiveState(void)
  {
    // Is there no currently active state to drop?
    if(!mStack.empty())
    {
      // Retrieve the currently active state
      IState* anState = mStack.back();
 
      // Log the removing of an active state
      ILOG() << "StateManager::RemoveActiveState(" << anState->GetID() << ")" << std::endl;
 
      // Pause the currently active state
      anState->Pause();
 
      // Cleanup the currently active state before we pop it off the stack
      anState->DeInit();
 
      // Pop the currently active state off the stack
      mStack.pop_back();
 
      // Move this state to our dropped stack
      mDead.push_back(anState);
 
      // Don't keep pointers around we don't need anymore
      anState = NULL;
    }
    else
    {
      // Quit the application with an error status response
      if(NULL != mApp)
      {
        mApp->Quit(StatusAppStackEmpty);
      }
      return;
    }
 
    // Is there another state to activate? then call Resume to activate it
    if(!mStack.empty())
    {
      // Has this state ever been initialized?
      if(mStack.back()->IsInitComplete())
      {
        // Resume the new active state
        mStack.back()->Resume();
      }
      else
      {
        // Initialize the new active state
        mStack.back()->DoInit();
      }
    }
    else
    {
      // There are no states on the stack, exit the program
      if(NULL != mApp)
      {
        mApp->Quit(StatusAppOK);
      }
    }
  }
 
  void StateManager::SetActiveState(typeStateID theStateID)
  {
    std::vector<IState*>::iterator it;
 
    // Find the state that matches theStateID
    for(it=mStack.begin(); it < mStack.end(); it++)
    {
      // Does this state match theStateID? then activate it as the new
      // currently active state
      if((*it)->GetID() == theStateID)
      {
        // Get a pointer to soon to be currently active state
        IState* anState = *it;
 
        // Log the setting of a previously active state as the current active state
        ILOG() << "StateManager::SetActiveState(" << anState->GetID() << ")" << std::endl;

        // Erase it from the list of previously active states
        mStack.erase(it);
 
        // Is there a state currently running? then Pause it
        if(!mStack.empty())
        {
          // Pause the currently running state since we are changing the
          // currently active state to the one specified by theStateID
          mStack.back()->Pause();
        }

        // Add the new active state
        mStack.push_back(anState);
 
        // Don't keep pointers we don't need around
        anState = NULL;

        // Has this state ever been initialized?
        if(mStack.back()->IsInitComplete())
        {
          // Resume the new active state
          mStack.back()->Resume();
        }
        else
        {
          // Initialize the new active state
          mStack.back()->DoInit();
        }

        // Exit our find loop
        break;
      } // if((*it)->GetID() == theStateID)
    } // for(it=mStack.begin(); it < mStack.end(); it++)
  }
 
  void StateManager::HandleCleanup(void)
  {
    // Remove one of our dead states
    if(!mDead.empty())
    {
      // Retrieve the dead state
      IState* anState = mDead.back();
      assert(NULL != anState && "StateManager::HandleCleanup() invalid dropped state pointer");
 
      // Pop the dead state off the stack
      mDead.pop_back();
 
      // Call the DeInit if it hasn't been called yet
      if(anState->IsInitComplete())
      {
        anState->DeInit();
      }

      // Next, call HandleCleanup for this state
      anState->HandleCleanup();

      // Just delete the state now
      delete anState;

      // Don't keep pointers around we don't need
      anState = NULL;
    }
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
