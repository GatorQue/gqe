/**
 * Provides the IEntity interface class for all game entities in a game.
 *
 * @file src/GQE/Entity/interfaces/IEntity.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120609 - Log output changes and improved AddProperty technique
 */
#include <GQE/Entity/interfaces/IEntity.hpp>
#include <GQE/Entity/interfaces/IComponent.hpp>

namespace GQE
{

  IEntity::IEntity()
  {
  }

  IEntity::~IEntity()
  {
    std::map<const typePropertyID, IProperty*>::iterator anProptertyIter;
    std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
    for(anProptertyIter=mPropertyList.begin();
        anProptertyIter!=mPropertyList.end();
        ++anProptertyIter)
    {
      IProperty* anProperty = (anProptertyIter->second);
      delete anProperty;
      anProperty = NULL;
    }
    for(anComponentIter=mComponentList.begin();
        anComponentIter!=mComponentList.end();
        ++anComponentIter)
    {
      IComponent* anComponent = (anComponentIter->second);
      anComponent->DeInit();
      anComponent->HandleCleanup();
      delete anComponent;
      anComponent=NULL;
    }
  }

  void IEntity::AddProperty(IProperty* theProperty)
  {
    if(mPropertyList.find(theProperty->GetID())==mPropertyList.end())
    {
      mPropertyList[theProperty->GetID()]=theProperty;
    }
    else
    {
      WLOG() << "IEntity:AddProperty() property(" << theProperty->GetID() << ") already exists!" << std::endl;
      return;
    }
  }

  void IEntity::AttachComponent(IComponent* theComponent)
  {
    if(NULL == theComponent)
    {
      ELOG() << "IEntity:AttachComponent() null component provided!" << std::endl;
      return;
    }
    if(mComponentList.find(theComponent->GetID())!=mComponentList.end())
    {
      ELOG() << "IEntity:AttachComponent() component(" << theComponent->GetID() << ") not found!" << std::endl;
      return;
    }
    mComponentList[theComponent->GetID()]=theComponent;
    theComponent->DoInit(this);
  }

  void IEntity::DetachComponent(const typeComponentID theComponentID)
  {
    if(mComponentList.find(theComponentID)==mComponentList.end())
    {
      ELOG() << "IEntity:DetachComponent() component(" << theComponentID << ") not found!" << std::endl;
      return;
    }
    IComponent* anComponent = mComponentList[theComponentID];
    anComponent->DeInit();
  }

  void IEntity::HandleEvents(sf::Event theEvent)
  {
    std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
    for(anComponentIter=mComponentList.begin();
        anComponentIter!=mComponentList.end();
        ++anComponentIter)
    {
      IComponent* anComponent = anComponentIter->second;
      anComponent->HandleEvents(theEvent);
    }
  }

  void IEntity::UpdateFixed()
  {
    std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
    for(anComponentIter=mComponentList.begin();
        anComponentIter!=mComponentList.end();
        ++anComponentIter)
    {
      IComponent* anComponent = anComponentIter->second;
      anComponent->UpdateFixed();
    }
  }

  void IEntity::UpdateVariable(float theElapstedTime)
  {
    std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
    for(anComponentIter=mComponentList.begin();
        anComponentIter!=mComponentList.end();
        ++anComponentIter)
    {
      IComponent* anComponent = anComponentIter->second;
      anComponent->UpdateVariable(theElapstedTime);
    }
  }

  void IEntity::Draw()
  {
    std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
    for(anComponentIter=mComponentList.begin();
        anComponentIter!=mComponentList.end();
        ++anComponentIter)
    {
      IComponent* anComponent = anComponentIter->second;
      anComponent->Draw();
    }
  }

  void IEntity::HandleCleanup()
  {
    std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
    for(anComponentIter=mComponentList.begin();
        anComponentIter!=mComponentList.end();
        ++anComponentIter)
    {
      IComponent* anComponent = anComponentIter->second;
      anComponent->HandleCleanup();
      if(anComponent->CanRemove())
      {
        mComponentList.erase(anComponentIter);
        delete anComponent;
        anComponent = NULL;
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
