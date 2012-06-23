/**
 * The ISystem interface class used for all derived ISystem entities that
 * can be added to an IEntity class.
 *
 * @file src/GQE/Entity/interfaces/ISystem.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120618 - Use IEntity not Instance and changed AddPrototype to AddProperties
 * @date 20120620 - Drop ourselves from registered IEntity classes
 * @date 20120622 - Fix issues with dropping IEntity classes
 */
#include <GQE/Entity/interfaces/ISystem.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>

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

    // Make sure to drop all our entities
    DropAllEntities();
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

      // Perform any custom Initialization for this new IEntity before adding it
      HandleInit(theEntity);

      // Now add this entity to the list of IEntities we currently process
      mEntities.insert(std::pair<const typeEntityID, IEntity*>(theEntity->GetID(), theEntity));

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
    // Return anResult which will be true if IEntity was found, false otherwise
    return (mEntities.find(theEntityID) != mEntities.end());
  }

  void ISystem::DropEntity(const typeEntityID theEntityID)
  {
    std::map<const typeEntityID, IEntity*>::iterator anEntityIter;

    anEntityIter = mEntities.find(theEntityID);
    if(anEntityIter != mEntities.end())
    {
      // Erase the IEntity from our ISystem
      EraseEntity(anEntityIter);
    }
  }

  void ISystem::DropAllEntities(void)
  {
		// Make sure we drop ourselves from all registered IEntity classes
    std::map<const typeEntityID, IEntity*>::iterator anEntityIter;

    // Start at the beginning of the list of IEntity classes
    anEntityIter = mEntities.begin();
    while(anEntityIter != mEntities.end())
    {
      // Erase this IEntity and move to the next one
      EraseEntity(anEntityIter++);
    }

    // Last of all clear our list of entities
		mEntities.clear();
  }

  void ISystem::EraseEntity(std::map<const typeEntityID, IEntity*>::iterator theEntityIter)
  {
    // Get our IEntity reference first
    IEntity* anEntity = theEntityIter->second;

    // First remove the IEntity from our list
		mEntities.erase(theEntityIter);

    // Now handle any last minute cleanup for this IEntity
    HandleCleanup(anEntity);

    // Now use our IEntity reference to remove any ISystem references
    if(anEntity->HasSystem(GetID()))
    {
      // Cause IEntity to drop our reference
      anEntity->DropSystem(GetID());
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
