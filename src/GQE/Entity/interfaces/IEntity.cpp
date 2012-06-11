/**
 * Provides the IEntity interface class for all game entities in a game.
 *
 * @file src/GQE/Entity/interfaces/IEntity.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#include <GQE/Entity/interfaces/IEntity.hpp>
#include <GQE/Entity/interfaces/ISystem.hpp>
namespace GQE
{

  IEntity::IEntity()
  {
  }

  IEntity::~IEntity()
  {
    std::map<const typePropertyID, IProperty*>::iterator anProptertyIter;
    for(anProptertyIter=mPropertyList.begin();
        anProptertyIter!=mPropertyList.end();
        ++anProptertyIter)
    {
      IProperty* anProperty = (anProptertyIter->second);
      delete anProperty;
      anProperty = NULL;
    }
  }

  void IEntity::AddProperty(IProperty* theProperty)
  {
    if(mPropertyList.find(theProperty->GetID())!=mPropertyList.end())
    {
      ELOG() << "Entity:AddProperty() property(" << theProperty->GetID() << ") " << std::endl;
      return;
    }
    mPropertyList[theProperty->GetID()]=theProperty;
  }
	void IEntity::AddSystem(ISystem* theSystem)
	{
		if(mSystemList.find(theSystem->GetID())!=mSystemList.end())
    {
      ELOG() << "Entity:AddProperty() property(" << theSystem->GetID() << ") is already controling this entity." << std::endl;
      return;
    }
    mSystemList[theSystem->GetID()]=theSystem;
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
