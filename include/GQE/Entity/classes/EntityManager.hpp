/**
 * Provides a system for Managing Prototype and Instance IEntity derived
 * classes.
 *
 * @file include/GQE/Entity/classes/EntityManager.hpp>
 * @author Jacob Dix
 * @date 20120425 Initial Release
 */
#ifndef ENTITY_MANAGER_HPP_INCLUDED
#define ENTITY_MANAGER_HPP_INCLUDED

#include <map>
#include <vector>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  /// Provides the EntityManager class for managing Prototype and Instance classes
  class GQE_API EntityManager
  {
    public:
      /**
       * EntityManager default constructor
       */
      EntityManager();

      /**
       * EntityManager deconstructor
       */
      virtual ~EntityManager();

      /**
       * AddPrototype is responsible for adding a Prototype entity to the list
       * of available Prototype classes that can be used to create an Instance
       * entity class.
       * @param[in] thePrototype class to add
       */
      void AddPrototype(Prototype* thePrototype);

      /**
       * AddInstance is responsible for adding a new Instance class using
       * thePrototypeID as the template from which to make the new Instance
       * class from.
       * @param[in] thePrototypeID to use to create the new Instance class
       * @return theInstanceID assigned to the new Instance class
       */
      typeInstanceID AddInstance(const typePrototypeID thePrototypeID);

      /**
       * GetInstance is responsible for returning the Instance class pointer
       * that matches theInstanceID provided.
       * @param[in] theInstanceID to find and return
       * @return the pointer to the Instance class specified
       */
      Instance* GetInstance(const typeInstanceID theInstanceID);

      /**
       * GetPrototype is responsible for returning the Prototype class pointer
       * that matches thePrototypeID provided.
       * @param[in] thePrototypeID to find and return
       * @return the pointer to the Prototype class specified
       */
      Prototype* GetPrototype(const typePrototypeID thePrototypeID);

      /**
       * HandleEvents is responsible for letting each Instance class have a
       * chance to handle theEvent specified.
       * @param[in] theEvent to handle
       */
      void HandleEvents(sf::Event theEvent);

      /**
       * UpdateFixed is called a specific number of times every game loop and
       * this method will allow each Instance class a chance to have its
       * UpdateFixed method called for each game loop iteration.
       */
      void UpdateFixed(void);

      /**
       * UpdateVariable is called every time the game loop draws a frame and
       * includes the elapsed time between the last UpdateVariable call for
       * use with equations that use time as a variable. (e.g. physics velocity
       * and acceleration equations).
       */
      void UpdateVariable(float theElapsedTime);

      /**
       * Draw is called during the game loop after events and the fixed update
       * loop calls are completed and depends largely on the speed of the
       * computer to determine how frequently it will be called. This gives the
       * EntityManager a chance to call the Draw method for each Instance
       * class.
       */
      void Draw(void);

      /**
       * HandleCleanup will be called during the game loop or IState shutdown
       * process and gives a chance for each Instance and Prototype class an
       * opportunity to clean up any loose ends and free up memory resources.
       */
      void HandleCleanup(void);

    protected:

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// A linked list of all Instance classes managed by the EntityManager
      std::vector<Instance*> mInstanceList;
      /// A map of Prototype classes managed by the EntityManager
      std::map<const typePrototypeID, Prototype*> mPrototypeList;
      /// The next ID to assign to a new Instance class
      typeInstanceID mNextID;
  };
} // namespace GQE

#endif

/**
 * @class GQE::EntityManager
 * @ingroup Entity
 * The EntityManager class is responsible for providing management for
 * Prototype and Instance classes used for a game. It can be a variable in an
 * IApp derived class or a variable in an IState derived class depending on the
 * desired scope (game application wide or game state only wide).
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

