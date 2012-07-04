/**
 * Provides the ActionSystem class for handing all IEntity actions in a game
 *
 * @file src/GQE/Entity/systems/ActionSystem.cpp
 * @author Jacob Dix
 * @date 20120620 - Initial Release
 */
#include <SFML/Graphics.hpp>
#include <GQE/Entity/systems/ActionSystem.hpp>
#include <GQE/Entity/interfaces/IAction.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>
#include <GQE/Entity/classes/ActionGroup.hpp>

namespace GQE
{
  ActionSystem::ActionSystem(IApp& theApp):
    ISystem("ActionSystem",theApp)
  {
  }

  ActionSystem::~ActionSystem()
  {
    // Make sure clear all of our IAction classes
    std::map<const typeActionID, IAction*>::iterator anActionIter;

    // Start at the beginning of the list of IAction classes
    anActionIter = mActions.begin();
    while(anActionIter != mActions.end())
    {
      // Erase this IAction and move to the next one
      EraseAction(anActionIter++);
    }

    // Last of all clear our list of IActions
    mActions.clear();
  }

  IAction* ActionSystem::GetAction(const typeActionID theActionID)
  {
    IAction* anResult = NULL;
    std::map<const typeActionID, IAction*>::iterator iter;

    // Is theActionID found in our list of available actions?
    iter = mActions.find(theActionID);
    if(iter != mActions.end())
    {
      // Retrieve the IAction pointer found
      anResult = iter->second;
    }

    // Return the IAction found above or NULL if none was found
    return anResult;
  }

  void ActionSystem::AddAction(IAction* theAction)
  {
    if(theAction != NULL)
    {
      // Make sure theAction doesn't already exist
      if(mActions.find(theAction->GetID()) == mActions.end())
      {
        // Add theAction provided to our map
        mActions.insert(
            std::pair<const typeActionID, IAction*>(
              theAction->GetID(), theAction));
      }
      else
      {
        WLOG() << "ActionSystem::AddAction(" << theAction->GetID()
          << ") action already exists!" << std::endl;
      }
    }
    else
    {
      ELOG() << "ActionSystem::AddAction() Null pointer provided!" << std::endl;
    }
  }

  void ActionSystem::AddProperties(IEntity* theEntity)
  {
    theEntity->mProperties.Add<ActionGroup>("Actions",ActionGroup(this));
    theEntity->AddSystem(this);
  }

  void ActionSystem::HandleEvents(sf::Event theEvent)
  {
  }

  void ActionSystem::UpdateFixed()
  {
    std::map<const typeEntityID, IEntity*>::iterator anEntityIter;
    for(anEntityIter=mEntities.begin();
        anEntityIter!=mEntities.end();
        ++anEntityIter)
    {
      // Get the IEntity to process
      IEntity* anEntity = anEntityIter->second;

      // Get the ActionGroup stored as a property in the IEntity object
      ActionGroup anActionGroup = anEntity->mProperties.Get<ActionGroup>("Actions");

      // Call the ActionGroup DoActions method with the given IEntity
      anActionGroup.DoActions(anEntity);
    }
  }

  void ActionSystem::UpdateVariable(float theElaspedTime)
  {
  }

  void ActionSystem::Draw()
  {
  }

  void ActionSystem::HandleInit(IEntity* theEntity)
  {
    // Do nothing
  }

  void ActionSystem::HandleCleanup(IEntity* theEntity)
  {
    // Do nothing
  }

  void ActionSystem::EraseAction(std::map<const typeActionID, IAction*>::iterator theActionIter)
  {
    // Get our IEntity reference first
    IAction* anEntity = theActionIter->second;

    // First remove the IAction from our list
    mActions.erase(theActionIter);
  }
} // namespace GQE

/**
 * Copyright (c) 2010-2012 Jacob Dix
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

