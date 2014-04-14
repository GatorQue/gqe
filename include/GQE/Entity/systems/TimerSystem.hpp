/**
* Provides the Timer class for handing Timed Events.
*
* @file include/GQE/Entity/systems/TimerSystem.hpp
* @author Jacob Dix
* @date 20140413 - Initial Release
*/
#ifndef TIMER_SYSTEM_HPP_INCLUDED
#define TIMER_SYSTEM_HPP_INCLUDED

#include <GQE/Entity/interfaces/ISystem.hpp>
namespace GQE
{
  class GQE_API TimerSystem : public GQE::ISystem
  {
  public:
    /**
    * CollisionSystem Constructor.
    * @param[in] theApp is the current GQE app.
    */
    TimerSystem(GQE::IApp& theApp);

    /**
    * CollisionSystem Destructor.
    */
    virtual ~TimerSystem();

    /**
    * AddProperties is responsible for adding the properties used by this
    * ISystem derived class to the IEntity derived class provided.
    * @param[in] theEntity to add the properties to.
    */
    virtual void AddProperties(GQE::IEntity* theEntity);

  protected:
    /**
    * HandleInit is called to allow each derived ISystem to perform any
    * initialization steps when a new IEntity is added.
    */
    virtual void HandleInit(GQE::IEntity* theEntity);

    /**
    * HandleCleanup is called when the IEntity that was added is finally
    * dropped from this ISystem and gives the derived ISystem class a chance
    * to perform any custom work before the IEntity is deleted.
    */
    virtual void HandleCleanup(GQE::IEntity* theEntity);

    void EntityUpdateFixed(GQE::IEntity* theEntity);

    void EventExpireEntity(TimerContext* theContext);
  };
}
#endif

/**
* @class GQE::TimerSystem
* @ingroup Entity
* The TimerSystem class is used to make timed events. The properties provided
* by this System are as follows:
* - Timers: A map of TimeContext structure.
* This class is very simple. It Iterates through each entitys list of timers and checks to see if they are triggerd.
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
