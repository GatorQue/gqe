/**
 * Provides the IEntity interface class for all game entities in a game.
 *
 * @file src/GQE/Entity/interfaces/IEntity.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120609 - Move property methods to new PropertyManager class
 * @date 20120618 - Moved ID from Instance class to this base class
 * @date 20120620 - Drop ourselves from registered ISystem classes
 * @date 20120622 - Fix issues with dropping ISystem classes
 * @date 20121108 - Fix warning: type qualifiers ignored on function return type
 */
#include <GQE/Entity/interfaces/IEntity.hpp>
#include <GQE/Entity/interfaces/ISystem.hpp>
namespace GQE
{
  typeEntityID IEntity::mNextID = 0; // Start with 0

  IEntity::IEntity(const Uint32 theOrder) :
    mEntityID(UseNextID()),
    mOrder(theOrder)
  {
    ILOG() << "IEntity::ctor(" << mEntityID << "," << mOrder << ")" << std::endl;
  }

  IEntity::~IEntity()
  {
    ILOG() << "IEntity::dtor(" << mEntityID << ")" << std::endl;

    // Make sure to drop all our systems
    DropAllSystems();
  }

  typeEntityID IEntity::GetID(void) const
  {
    return mEntityID;
  }

  typeEntityID IEntity::UseNextID()
  {
    // Pre-increment the ID so that an ID of 0 can be considered an "invalid" ID
    return ++mNextID;
  }

  Uint32 IEntity::GetOrder(void) const
  {
    return mOrder;
  }

  void IEntity::SetOrder(const Uint32 theOrder)
  {
    mOrder = theOrder;
  }

  void IEntity::AddSystem(ISystem* theSystem)
  {
    if(mSystems.find(theSystem->GetID()) == mSystems.end())
    {
      mSystems[theSystem->GetID()] = theSystem;
      // Make sure this entity has the correct properties added for this system
      theSystem->AddProperties(this);

    }
  }

  bool IEntity::HasSystem(const typeSystemID theSystemID) const
  {
    bool anResult=false;
    if(mSystems.find(theSystemID)!=mSystems.end())
    {
      anResult=true;
    }
    return anResult;
  }

  void IEntity::DropSystem(const typeSystemID theSystemID)
  {
    // Iterator for looping through each registered ISystem
    std::map<const typeSystemID, ISystem*>::iterator anSystemIter;

    // See if we can find theSystemID specified
    anSystemIter = mSystems.find(theSystemID);
    if(anSystemIter != mSystems.end())
    {
      // Call our EraseSystem variable
      EraseSystem(anSystemIter);
    }
    else
    {
      WLOG() << "IEntity::DropSystem(" << theSystemID
        << ") was not found!" << std::endl;
    }
  }

  void IEntity::DropAllSystems(void)
  {
    // Make sure we drop ourselves from all registered ISystem classes
    std::map<const typeSystemID, ISystem*>::iterator anSystemIter;

    // Start at the beginning of the list of ISystem classes
    anSystemIter = mSystems.begin();
    while(anSystemIter != mSystems.end())
    {
      EraseSystem(anSystemIter++);
    }

    // Last of all clear our list of systems
    mSystems.clear();
  }

  void IEntity::EraseSystem(std::map<const typeSystemID, ISystem*>::iterator theSystemIter)
  {
    // Get our ISystem reference first
    ISystem* anSystem = theSystemIter->second;

    // First remove the ISystem from our list
    mSystems.erase(theSystemIter);

    // Now use our ISystem reference to remove any IEntity references
    if(anSystem->HasEntity(GetID()))
    {
      // Cause ISystem to drop our reference
      anSystem->DropEntity(GetID());
    }
  }
  void IEntity::Write(std::fstream& theFileStream)
  {
  }
  void IEntity::Read(std::fstream& theFileStream)
  {

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
