/**
 * Provides a system for Managing Prototype and Instance IEntity derived
 * classes.
 *
 * @file include/GQE/Entity/classes/PrototypeManager.hpp>
 * @author Jacob Dix
 * @date 20120425 - Initial Release
 * @date 20120618 - Changed name from mPrototypeList to mPrototypes
 */
#ifndef PROTOTYPE_MANAGER_HPP_INCLUDED
#define PROTOTYPE_MANAGER_HPP_INCLUDED

#include <map>
#include <vector>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  /// Provides the PrototypeManager class for managing Prototype and Instance classes
  class GQE_API PrototypeManager
  {
    public:
      /**
       * EntityManager default constructor
       */
      PrototypeManager();

      /**
       * EntityManager deconstructor
       */
      virtual ~PrototypeManager();

      /**
       * AddPrototype is responsible for adding a Prototype entity to the list
       * of available Prototype classes that can be used to create an Instance
       * entity class.
       * @param[in] thePrototype class to add
       */
      void AddPrototype(Prototype* thePrototype);

      /**
       * GetPrototype is responsible for returning the Prototype class pointer
       * that matches thePrototypeID provided.
       * @param[in] thePrototypeID to find and return
       * @return the pointer to the Prototype class specified
       */
      Prototype* GetPrototype(const typePrototypeID thePrototypeID);
    protected:

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// A map of Prototype classes managed by the EntityManager
      std::map<const typePrototypeID, Prototype*> mPrototypes;
  };
} // namespace GQE

#endif // PROTOTYPE_MANAGER_HPP_INCLUDED

/**
 * @class GQE::PrototypeManager
 * @ingroup Entity
 * The PrototypeManager class is responsible for providing management for
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
