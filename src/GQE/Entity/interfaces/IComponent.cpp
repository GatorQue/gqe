/**
 * The IComponent interface class used for all derived IComponent entities that
 * can be added to an IEntity class.
 *
 * @file src/GQE/Entity/interfaces/IComponent.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#include <assert.h>
#include <GQE/Entity/interfaces/IComponent.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
  IComponent::IComponent(const typeComponentID theComponentID , IApp& theApp) :
    mApp(theApp),
    mEntity(NULL),
    mComponentID(theComponentID),
    mInit(false),
    mCleanup(false),
    mRemove(false)
  {
    ILOG() << "IComponent::ctor(" << mComponentID << ")" << std::endl;
  }

  IComponent::~IComponent()
  {
    ILOG() << "IComponent::dtor(" << mComponentID << ")" << std::endl;
  }

  const typeComponentID IComponent::GetID(void) const
  {
    return mComponentID;
  }

  void IComponent::DoInit(IEntity* theEntity)
  {
    ILOG() << "IComponent::DoInit(" << mComponentID << ")" << std::endl;
    mEntity=theEntity;
    // If Cleanup hasn't been called yet, call it now!
    if(true == mCleanup)
    {
      HandleCleanup();
    }
    // Initialize if necessary
    if(false == mInit)
    {
      mInit = true;
    }
  }

  void IComponent::DeInit(void)
  {
    ILOG() << "IState::DeInit(" << mComponentID << ")" << std::endl;

    if(true == mInit)
    {
      mCleanup = true;
      mInit = false;
    }
  }

  bool IComponent::IsInitComplete(void)
  {
    return mInit;
  }

  bool IComponent::CanRemove(void)
  {
    return mRemove;
  }

  void IComponent::HandleCleanup(void)
  {
    if(true == mCleanup)
    {
      // Call cleanup
      Cleanup();

      // Clear our cleanup flag
      mCleanup = false;

      //Ready to be removed
      mRemove = true;
    }
  }

  void IComponent::Cleanup(void)
  {
    ILOG() << "IComponent::Cleanup(" << mComponentID << ")" << std::endl;
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
