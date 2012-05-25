/**
 * Provides a system for Managing Prototype and Instance IEntity derived
 * classes.
 *
 * @file src/GQE/Entity/classes/EntityManager.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#include <GQE/Entity/classes/EntityManager.hpp>
#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/classes/Instance.hpp>

namespace GQE
{
  EntityManager::EntityManager() :
    mNextID(0)
  {

  }

  EntityManager::~EntityManager()
  {
  }

  void EntityManager::AddPrototype(Prototype* thePrototype)
  {
    if(thePrototype==NULL)
      return;
    mPrototypeList[thePrototype->GetID()]=thePrototype;
  }

  typeInstanceID EntityManager::AddInstance(const typePrototypeID thePrototypeID)
  {
    typeInstanceID anResult = 0;
    if(mPrototypeList.find(thePrototypeID) != mPrototypeList.end())
    {
      Instance* anInstance =
        mPrototypeList.at(thePrototypeID)->MakeInstance(++mNextID);
      mInstanceList.push_back(anInstance);
      anResult = anInstance->GetID();
    }

    // Return anResult found/created or 0 if none was created
    return anResult;
  }

  Instance* EntityManager::GetInstance(const typeInstanceID theEntityID)
  {
    Instance* anResult = NULL;

    std::vector<Instance*>::iterator anInstanceIter;
    for(anInstanceIter=mInstanceList.begin();
        anInstanceIter!=mInstanceList.end();
        ++anInstanceIter)
    {
      Instance* anInstance = *anInstanceIter;
      if(anInstance->GetID() == theEntityID)
      {
        anResult = anInstance;
        break;
      }
    }

    // Return anResult found (which might be NULL if none was found)
    return anResult;
  }

  Prototype* EntityManager::GetPrototype(const typePrototypeID thePrototypeID)
  {
    Prototype* anResult = NULL;

    if(mPrototypeList.find(thePrototypeID)!=mPrototypeList.end())
    {
      anResult = mPrototypeList[thePrototypeID];
    }

    // Return anResult found (which might be NULL if none was found)
    return anResult;
  }

  void EntityManager::HandleEvents(sf::Event theEvent)
  {
    std::vector<Instance*>::iterator it;
    for(it=mInstanceList.begin(); it!=mInstanceList.end(); ++it)
    {
      (*it)->UpdateInfo();
      (*it)->HandleEvents(theEvent);
    }
  }

  void EntityManager::UpdateFixed()
  {
    std::vector<Instance*>::iterator it;
    for(it=mInstanceList.begin(); it!=mInstanceList.end(); ++it)
    {
      (*it)->UpdateFixed();
    }
  }

  void EntityManager::UpdateVariable(float theElapsdTime)
  {
    std::vector<Instance*>::iterator it;
    for(it=mInstanceList.begin(); it!=mInstanceList.end(); ++it)
    {
      (*it)->UpdateVariable(theElapsdTime);
    }
  }

  void EntityManager::Draw()
  {
    std::vector<Instance*>::iterator it;
    for(it=mInstanceList.begin(); it!=mInstanceList.end(); ++it)
    {
      (*it)->Draw();
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
