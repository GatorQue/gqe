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
 * @date 20120707 - Fix pure-virtual crash which calls HandleCleanup in dtor
 * @date 20121107 - Fix warning: unused variable
 * @date 20121108 - Fix warning: type qualifiers ignored on function return type
 * @date 20121222 - Added Entity functions to ISystem for handling entities individually.
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

  const typeSystemID ISystem::GetID(void)
  {
    return mSystemID;
  }

  typeEntityID ISystem::AddEntity(IEntity* theEntity)
  {
    typeEntityID anResult = 0;

    // Make sure the caller didn't give us a bad pointer
    if(theEntity != NULL)
    {
      // This will be true if the IEntity was added
      //bool anAdded = false;

      // See if this IEntity is already in this z-order class
      std::deque<IEntity*>::iterator anFind;
      anFind = std::find(mEntities[theEntity->GetOrder()].begin(),
        mEntities[theEntity->GetOrder()].end(), theEntity);

        // If IEntity wasn't found in this z-order class then add it now
      if(anFind == mEntities[theEntity->GetOrder()].end())
      {
        // Add theEntity provided to this deque
        mEntities[theEntity->GetOrder()].push_back(theEntity);

        // Make sure this entity has the correct properties added for this system
        AddProperties(theEntity);

        // Add this system to this entity
        theEntity->AddSystem(this);

        // Perform any custom Initialization for this new IEntity before adding it
        HandleInit(theEntity);

        // Return the ID of this IEntity as a result
        anResult = theEntity->GetID();
      }
      else
      {
        WLOG() << "ISystem::AddEntity(" << theEntity->GetID()
          << ") Entity already exists!" << std::endl;
      }
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

    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::const_iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end() && anResult == false)
    {
      std::deque<IEntity*>::const_iterator anFind = anIter->second.begin();
      while(anFind != anIter->second.end())
      {
        // Is this theEntityID we are looking for? then note it and return
        if(theEntityID == (*anFind)->GetID())
        {
          // We found theEntityID
          anResult = true;

          // Exit while loop
          break;
        }

        // Increment find iterator
        anFind++;
      }

      // Increment map iterator
      anIter++;
    }

    // Return anResult which will be true if IEntity was found, false otherwise
    return anResult;
  }

  void ISystem::DropEntity(const typeEntityID theEntityID)
  {
    // This will be set to true if theEntityID was found and dropped
    bool anFound = false;

    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end() && anFound == false)
    {
      std::deque<IEntity*>::iterator anFind = anIter->second.begin();
      while(anFind != anIter->second.end())
      {
        // Is this theEntityID we are looking for? then note it and return
        if(theEntityID == (*anFind)->GetID())
        {
          // Erase the IEntity from our deque
          anFind = EraseEntity(anFind);

          // Set anFound to true
          anFound = true;

          // Exit while loop
          break;
        }

        // Increment find iterator
        anFind++;
      }

      // Increment map iterator
      anIter++;
    }
  }

  void ISystem::DropAllEntities(void)
  {
    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end())
    {
      std::deque<IEntity*>::iterator anFind = anIter->second.begin();
      while(anFind != anIter->second.end())
      {
        // Erase the IEntity from our deque
        anFind = EraseEntity(anFind);
      }

      // Increment map iterator
      anIter++;
    }

    // Last of all clear our list of deques
    mEntities.clear();
  }

  void ISystem::HandleCleanup(IEntity* theEntity)
  {
    // Do nothing
  }

  std::deque<IEntity*>::iterator ISystem::EraseEntity(std::deque<IEntity*>::iterator theEntityIter)
  {
    std::deque<IEntity*>::iterator anResult;

    // Get our IEntity reference first
    IEntity* anEntity = (*theEntityIter);

    anResult = mEntities[anEntity->GetOrder()].erase(theEntityIter);

    // Now handle any last minute cleanup for this IEntity
    HandleCleanup(anEntity);

    // Now use our IEntity reference to remove any ISystem references
    if(anEntity->HasSystem(GetID()))
    {
      // Cause IEntity to drop our reference
      anEntity->DropSystem(GetID());
    }

    // Return anResult iterator to caller which might be end()
    return anResult;
  }
  void ISystem::HandleEvents(sf::Event theEvent)
  {
    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end())
    {
      std::deque<IEntity*>::iterator anQueue = anIter->second.begin();
      while(anQueue != anIter->second.end())
      {
        // Get the IEntity address first
        GQE::IEntity* anEntity = *anQueue;

        // Increment the IEntity iterator second
        anQueue++;

        //Prosses this entities Events.
        EntityHandleEvents(anEntity,theEvent);
      }
      anIter++;
    }
  }
  void ISystem::UpdateFixed(void)
  {
    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end())
    {
      std::deque<IEntity*>::iterator anQueue = anIter->second.begin();
      while(anQueue != anIter->second.end())
      {
        // Get the IEntity address first
        GQE::IEntity* anEntity = *anQueue;

        // Increment the IEntity iterator second
        anQueue++;

        //Update this entity.
        EntityUpdateFixed(anEntity);
      }
      anIter++;
    }
  }
  void ISystem::UpdateVariable(float theElapsedTime)
  {
    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end())
    {
      std::deque<IEntity*>::iterator anQueue = anIter->second.begin();
      while(anQueue != anIter->second.end())
      {
        // Get the IEntity address first
        GQE::IEntity* anEntity = *anQueue;

        // Increment the IEntity iterator second
        anQueue++;

        //Update this entity.
        EntityUpdateVariable(anEntity,theElapsedTime);
      }
      anIter++;
    }
  }
  void ISystem::Draw(void)
  {
        // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end())
    {
      std::deque<IEntity*>::iterator anQueue = anIter->second.begin();
      while(anQueue != anIter->second.end())
      {
        // Get the IEntity address first
        GQE::IEntity* anEntity = *anQueue;

        // Increment the IEntity iterator second
        anQueue++;
        //Draw this entity.
        EntityDraw(anEntity);
      }
      anIter++;
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
