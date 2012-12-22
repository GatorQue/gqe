/**
 * The ISystem interface class defines a useful system that can be used for all
 * registered IEntity classes. This includes a RenderSystem, CollisionSystem,
 * MovementSystem, AnimationSystem, etc.
 *
 * @file include/GQE/Entity/interfaces/ISystem.hpp
 * @author Jacob Dix
 * @date 20120417 - Initial Release
 * @date 20120618 - Use IEntity not Instance and changed AddPrototype to AddProperties
 * @date 20120620 - Drop ourselves from registered IEntity classes
 * @date 20120622 - Added EraseSystem, DropAllEntities and fixed comments and changed
 *                  HandleInit and HandleCleanup to be called during AddEntity and
 *                  EraseEntity.
 * @date 20120623 - Adjusted scope of some ISystem variables
 * @date 20120707 - Fix pure-virtual crash which calls HandleCleanup in dtor
 * @date 20121108 - Fix warning: type qualifiers ignored on function return type
 */
#ifndef ISYSTEM_HPP_INCLUDED
#define ISYSTEM_HPP_INCLUDED

#include <map>
#include <deque>
#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
  /// The ISystem interface used by all IEntity system managers
  class GQE_API ISystem
  {
    public:
      /**
       * ISystem Constructor.
       * @param[in] theSystemID is the ID of this system.
       * @param[in] theApp is the Current GQE App.
       */
      ISystem(const typeSystemID theSystemID, IApp& theApp);

      /**
       * ISystem Destructor.
       */
      virtual ~ISystem();

      /**
       * GetID will return the ID used for this .
       * @return the system ID for this system
       */
      const typeSystemID GetID(void);

      /**
       * AddEntity is responsible for adding a new IEntity class to be managed by
       * this ISystem derived class.
       * @param[in] theEntity to add to this ISystem class
       * @return theEntityID of theEntity that was provided and added
       */
      typeEntityID AddEntity (IEntity* theEntity);

      /**
       * AddProperties is responsible for adding the properties used by this
       * ISystem derived class to the IEntity derived class provided.
       * @param[in] theEntity to add the properties to.
       */
      virtual void AddProperties(IEntity* theEntity) = 0;

      /**
       * HasEntity is responsible for finding if the IEntity class has already
       * been registered with this ISystem.
       * @param[in] theEntityID to find and confirm has been added
       */
      bool HasEntity(const typeEntityID theEntityID) const;

      /**
       * DropEntity is responsible for removing the IEntity class that matches
       * the ID provided from this ISystem derived class.
       * @param[in] theEntityID to remove from the ISystem class
       */
      void DropEntity(const typeEntityID theEntityID);

      /**
       * DropAllEntities is responsible for removing this ISystem class from
       * all registered IEntity classes.
       */
      void DropAllEntities(void);

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
      /**
       * EntityHandleEvents is responsible handling SFML events per entity.
       * @param[in] theEntity to handle
       * @param[in] theEvent to handle
       */
      virtual void EntityHandleEvents(IEntity* theEntity,sf::Event theEvent);

      /**
       * EntityUpdateFixed is called for handling Updateing the given entity.
       * @param[in] theEntity to update
       */
      virtual void EntityUpdateFixed(IEntity* theEntity);

      /**
       * EntityUpdateVariable is called for handling Updateing the given entity.
       * @param[in] theEntity to update
       * @param[in] theElapsedTime from last update.
       */
      virtual void EntityUpdateVariable(IEntity* theEntity,float theElapsedTime);

      /**
       * EntityDraw is called during the game loop after events and the fixed update
       * loop calls are completed and depends largely on the speed of the
       * computer to determine how frequently it will be called.
       * @param[in] theEntity to update
       */
      virtual void EntityDraw(IEntity* theEntity);
    protected:
      //Variables
      /////////////////////////////////////////////////////////////
      /// A linked list of all IEntity classes managed by this ISystem
      std::map<const Uint32, std::deque<IEntity*>> mEntities;
      /// The address to the IApp derived class
      IApp& mApp;

      /**
       * HandleInit is called to allow each derived ISystem to perform any
       * initialization steps when a new IEntity is added.
       */
      virtual void HandleInit(IEntity* theEntity) = 0;

      /**
       * HandleCleanup is called when the IEntity that was added is finally
       * dropped from this ISystem and gives the derived ISystem class a chance
       * to perform any custom work before the IEntity is deleted.
       */
      virtual void HandleCleanup(IEntity* theEntity);

    private:
      //Variables
      /////////////////////////////////////////////////////////////
      /// The ID for this System.
      const typeSystemID mSystemID;

      /**
       * EraseEntity will erase the IEntity iterator provided.
       * @param[in] theEntityIter iterator to be erased
       * @return the next iterator that should be used
       */
      std::deque<IEntity*>::iterator EraseEntity(std::deque<IEntity*>::iterator theEntityIter);
  }; // class ISystem
} // namespace GQE

#endif // ICOMPONENT_HPP_INCLUDED

/**
 * @class GQE::ISystem
 * @ingroup Entity
 * The ISystem class represents the interface used by all Components that
 * can be added to an IEntity derived class. Components enable IEntity classes
 * to differ from each other without requiring the game designer to create
 * complicated inheritance models for all their game entities.
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
