/**
 * The IComponent interface class used for all derived IComponent entities that
 * can be added to an IEntity class.
 *
 * @file include/GQE/Entity/interfaces/IComponent.hpp
 * @author Jacob Dix
 * @date 20120417 - Initial Release
 * @date 20120618 - Use IEntity not Instance and changed AddPrototype to AddProperties
 * @date 20120620 - Drop ourselves from registered IEntity classes
 */
#ifndef ISYSTEM_HPP_INCLUDED
#define ISYSTEM_HPP_INCLUDED

#include <GQE/Core/interfaces/IApp.hpp>
#include <GQE/Entity/Entity_types.hpp>
#include <queue>
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
		const typeSystemID GetID(void) const;

		/**
		 * AddEntity is responsible for adding a new IEntity class to be managed by
     * this ISystem derived class.
		 * @param[in] theEntity to add to this ISystem class
		 * @return theEntityID of theEntity that was provided and added
		 */
		const typeEntityID AddEntity(IEntity* theEntity);
    
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
     * AddProperties is responsible for adding the properties used by this
     * ISystem derived class to the IEntity derived class provided.
     * @param[in] theEntity to add the properties too.
     */
    virtual void AddProperties(IEntity* theEntity) = 0;

		/**
		 * HandleInit is called at the begining of an update cycle.
		 */
		virtual void HandleInit(void);

		/**
		 * HandleEvents is responsible for letting each Instance class have a
		 * chance to handle theEvent specified.
		 * @param[in] theEvent to handle
		 */
		virtual void HandleEvents(sf::Event theEvent) = 0;

		/**
		 * UpdateFixed is called a specific number of times every game loop and
		 * this method will allow each Instance class a chance to have its
		 * UpdateFixed method called for each game loop iteration.
		 */
		virtual void UpdateFixed(void) = 0;

		/**
		 * UpdateVariable is called every time the game loop draws a frame and
		 * includes the elapsed time between the last UpdateVariable call for
		 * use with equations that use time as a variable. (e.g. physics velocity
		 * and acceleration equations).
		 */
		virtual void UpdateVariable(float theElapsedTime) = 0;

		/**
		 * Draw is called during the game loop after events and the fixed update
		 * loop calls are completed and depends largely on the speed of the
		 * computer to determine how frequently it will be called. This gives the
		 * EntityManager a chance to call the Draw method for each Instance
		 * class.
		 */
		virtual void Draw(void) = 0;


		/**
		 * HandleCleanup will be called at the end of the game loop, preferably after Draw().
		 */
		virtual void HandleCleanup(void);

		//static PrototypeManager* gPrototypeManager;
	protected:
		//Variables
		/////////////////////////////////////////////////////////////
		/// A linked list of all IEntity classes managed by this ISystem
		std::vector<IEntity*> mEntities;
		/// A queue of all IEntity classes to be added to this ISystem
		std::queue<IEntity*> mNewEntities;
		/// A queue of all IEntity classes to be removed from this ISystem
		std::queue<IEntity*> mDeadEntities;

		/// The ID for this System.
		typeSystemID mSystemID;
		/// The address to the IApp derived class
		IApp& mApp;
	};
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
