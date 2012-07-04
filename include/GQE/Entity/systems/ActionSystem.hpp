/**
 * Provides the ActionSystem class for handing all IEntity input in a game.
 *
 * @file src/GQE/Entity/systems/ActionSystem.cpp
 * @author Jacob Dix
 * @date 20120620 - Initial Release
 */
#ifndef INPUT_SYSTEM_HPP_INCLUDED
#define INPUT_SYSTEM_HPP_INCLUDED

#include <GQE/Entity/interfaces/ISystem.hpp>
namespace GQE
{
  /// The ActionSystem for managing all IEntity input in a game
  class GQE_API ActionSystem : public ISystem
  {
    public:
      /**
       * ActionSystem Constructor.
       * @param[in] theApp is the current GQE app.
       */
      ActionSystem(IApp& theApp);

      /**
       * ActionSystem Destructor.
       */
      virtual ~ActionSystem();

      /**
       * GetAction is responsible for retrieving an IAction previously
       * registered under theActionID specified and returning it to the caller
       * which is typically an ActionGroup class. All IActions should be first
       * registered with the ActionSystem before ActionGroups are created and
       * composed for each IEntity class.
       * @param[in] theActionID to find and retrieve
       * @return pointer to IAction found or NULL otherwise
       */
      IAction* GetAction(const typeActionID theActionID);

      /**
       * AddAction is responsible for adding the IAction provided to the list
       * of available actions to be assigned to an ActionGroup.
       * @param[in] theAction to be added
       */
      void AddAction(IAction* theAction);

      /**
       * AddProperties is responsible for adding the properties used by this
       * ISystem derived class to the IEntity derived class provided.
       * @param[in] theEntity to add the properties to.
       */
      virtual void AddProperties(IEntity* theEntity);

      /**
       * HandleEvents is responsible for letting each Instance class have a
       * chance to handle theEvent specified.
       * @param[in] theEvent to handle
       */
      virtual void HandleEvents(sf::Event theEvent);

      /**
       * UpdateFixed is called a specific number of times every game loop and
       * this method will allow each Instance class a chance to have its
       * UpdateFixed method called for each game loop iteration.
       */
      virtual void UpdateFixed(void);

      /**
       * UpdateVariable is called every time the game loop draws a frame and
       * includes the elapsed time between the last UpdateVariable call for
       * use with equations that use time as a variable. (e.g. physics velocity
       * and acceleration equations).
       */
      virtual void UpdateVariable(float theElapsedTime);

      /**
       * Draw is called during the game loop after events and the fixed update
       * loop calls are completed and depends largely on the speed of the
       * computer to determine how frequently it will be called. This gives the
       * EntityManager a chance to call the Draw method for each Instance
       * class.
       */
      virtual void Draw(void);

    protected:
      /**
       * HandleInit is called to allow each derived ISystem to perform any
       * initialization steps when a new IEntity is added.
       */
      virtual void HandleInit(IEntity* theEntity);

      /**
       * HandleCleanup is called when the IEntity that was added is finally
       * dropped from this ISystem and gives the derived ISystem class a chance
       * to perform any custom work before the IEntity is deleted.
       */
      virtual void HandleCleanup(IEntity* theEntity);
    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Map of all available actions
      std::map<const typeActionID, IAction*> mActions;

      /**
       * EraseAction will erase the IAction iterator provided.
       * @param[in] theActionIter iterator to be erased
       */
      void EraseAction(std::map<const typeActionID, IAction*>::iterator theActionIter);
  }; // class ActionSystem
} // namespace GQE

#endif // INPUT_SYSTEM_HPP_INCLUDED

/**
 * @class GQE::ActionSystem
 * @ingroup Entity
 * The ActionSystem class represents the system used to manage all Actions
 * to be run against with each IEntity class in a game.
 *
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
