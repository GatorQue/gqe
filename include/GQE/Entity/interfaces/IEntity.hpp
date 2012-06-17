/**
 * Provides the IEntity interface class for all game entities in a game.
 *
 * @file include/GQE/Entity/interfaces/IEntity.hpp>
 * @author Jacob Dix
 * @date 20120418 - Initial Release
 * @date 20120507 - Changed how properties are retrived/set.
 * @date 20120519 - Renaimed Entity class to IEntity and moved to interfaces folder.
 * @date 20120609 - Moved property methods to new PropertyManager class
 */
#ifndef IENTITY_HPP_INCLUDED
#define IENTITY_HPP_INCLUDED

#include <map>
#include <SFML/Window/Event.hpp>
#include <GQE/Entity/classes/PropertyManager.hpp>
#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
  /// The IEntity base class to be managed by the EntityManager class
  class GQE_API IEntity
  {
    public:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      PropertyManager mProperties;

      /**
       * IEntity default constructor
       */
      IEntity();

      /**
       * IEntity destructor
       */
      virtual ~IEntity();

			/**
       * AddSystem adds a dependant system to the entity.
       * @param[in] theSystem is a pointer to the active system.
       */
			void AddSystem(ISystem* theSystem);
			
			bool HasSystem(typeSystemID theSystemID);
    protected:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
			//A list of systems this entity is controlled by
			std::map<const typeSystemID,ISystem*> mSystemList;
  };
}

#endif // IENTITY_HPP_INCLUDED

/**
 * @class GQE::IEntity
 * @ingroup Entity
 * The IEntity class represents the interface used by all Entities managed by
 * the EntityManager class. This class is at the heart of the GQE Entity
 * library.
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

