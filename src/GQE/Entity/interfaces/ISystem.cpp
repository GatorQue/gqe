/**
 * The ISystem interface class used for all derived ISystem entities that
 * can be added to an IEntity class.
 *
 * @file src/GQE/Entity/interfaces/ISystem.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120618 - Use IEntity not Instance and changed AddPrototype to AddProperties
 * @date 20120620 - Drop ourselves from registered IEntity classes
 */
#include <assert.h>
#include <GQE/Entity/interfaces/ISystem.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Entity/classes/Instance.hpp>
#include <GQE/Entity/classes/PrototypeManager.hpp>
namespace GQE
{
  ISystem::ISystem(const typeSystemID theSystemID, IApp& theApp) :
    mApp(theApp),
    mSystemID(theSystemID)
  {
    ILOG() << "ISystem::ctor(" << mSystemID << ")" << std::endl;
  }

  ISystem::~ISystem()
  {
    ILOG() << "ISystem::dtor(" << mSystemID << ")" << std::endl;

    // Make sure we drop ourselves from all registered IEntity classes
		std::vector<IEntity*>::iterator anEntityIter;

    // Start at the beginning of the list of IEntity classes
    anEntityIter = mEntities.begin();
    while(anEntityIter != mEntities.end())
    {
			IEntity* anEntity = (*anEntityIter);

      // Remove the IEntity from our list
      mEntities.erase(anEntityIter++);

      // Is this ISystem still registered with this IEntity class?
      if(anEntity->HasSystem(GetID()))
      {
        // Remove this ISystem from the IEntity
        anEntity->DropSystem(GetID());
      }
    }
  }

  const typeSystemID ISystem::GetID(void) const
  {
    return mSystemID;
  }

	const typeEntityID ISystem::AddEntity(IEntity* theEntity)
	{
		typeEntityID anResult = 0;

    // Make sure the caller didn't give us a bad pointer
		if(theEntity != NULL)
		{
      // Make sure this entity has the correct properties added for this system
      AddProperties(theEntity);

      // Now add this entity to the list of IEntities we currently process
			mEntities.push_back(theEntity);

      // Return the ID of this IEntity as a result
			anResult = theEntity->GetID();
		}
    else
    {
      ELOG() << "ISystem::AddEntity() Null Entity pointer provided!" << std::endl;
    }

    // Return the ID of this IEntity class or 0 if something went wrong
		return anResult;
	}

  bool ISystem::HasEntity(const typeEntityID theEntityID) const
  {
    bool anResult = false;
		std::vector<IEntity*>::const_iterator anEntityIter;

    // Loop through each IEntity class looking for the ID provided
		for(anEntityIter=mEntities.begin();
			anEntityIter!=mEntities.end();
			++anEntityIter)
		{
			IEntity* anEntity = (*anEntityIter);

      // Is this the IEntity we are looking for?
      if(anEntity != NULL && anEntity->GetID() == theEntityID)
      {
        // We found the IEntity
        anResult = true;

        // Exit the for loop, we found the IEntity we were looking for
        break;
      }
    }

    // Return anResult which will be true if IEntity was found, false otherwise
    return anResult;
  }

  void ISystem::DropEntity(const typeEntityID theEntityID)
  {
		std::vector<IEntity*>::iterator anEntityIter;

    // Loop through each IEntity class looking for the ID provided
		for(anEntityIter=mEntities.begin();
			anEntityIter!=mEntities.end();
			++anEntityIter)
		{
			IEntity* anEntity = (*anEntityIter);
      // Is this the IEntity we are looking for?
      if(anEntity != NULL && anEntity->GetID() == theEntityID)
      {
        // Make sure to deregister this system from this IEntity class
        if(anEntity->HasSystem(GetID()))
        {
          // Drop the ISystem from this IEntity class
          anEntity->DropSystem(GetID());
        }

        // Remove this IEntity and exit the loop and method
        mEntities.erase(anEntityIter);

        // Exit the for loop, we found the IEntity we were looking for
        break;
      }
    }
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
