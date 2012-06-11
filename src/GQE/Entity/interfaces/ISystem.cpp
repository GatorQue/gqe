/**
 * The ISystem interface class used for all derived ISystem entities that
 * can be added to an IEntity class.
 *
 * @file src/GQE/Entity/interfaces/ISystem.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#include <assert.h>
#include <GQE/Entity/interfaces/ISystem.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Entity/classes/Instance.hpp>
#include <GQE/Entity/classes/PrototypeManager.hpp>
namespace GQE
{
	typeInstanceID ISystem::mNextID=0;
	PrototypeManager* ISystem::gPrototypeManager=NULL;
  ISystem::ISystem(const typeSystemID theSystemID , IApp& theApp) :
    mApp(theApp),
    mSystemID(theSystemID)
  {
    ILOG() << "ISystem::ctor(" << mSystemID << ")" << std::endl;
		if(gPrototypeManager==NULL)
		{
			gPrototypeManager=new PrototypeManager();
		}
  }

  ISystem::~ISystem()
  {
    ILOG() << "ISystem::dtor(" << mSystemID << ")" << std::endl;
  }
	typeInstanceID ISystem::AddInstance(Instance* theInstance)
	{
		int anResult=0;
		if(theInstance!=NULL)
		{
			mInstanceList.push_back(theInstance);
			anResult=theInstance->GetID();
		}
		return anResult;
	}
	typeInstanceID ISystem::UseNextID()
	{
		mNextID++;
		return mNextID;
	}

  const typeSystemID ISystem::GetID(void) const
  {
    return mSystemID;
  }
	void ISystem::RegisterPrototype(Prototype* thePrototype)
	{
		gPrototypeManager->AddPrototype(thePrototype);
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
