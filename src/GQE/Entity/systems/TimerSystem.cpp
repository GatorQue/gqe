/**
* Provides the Timer class for handing Timed Events.
*
* @file include/GQE/Entity/systems/TimerSystem.cpp
* @author Jacob Dix
* @date 20140413 - Initial Release
*/
#include <SFML/Graphics.hpp>
#include <GQE/Entity/systems/TimerSystem.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>
namespace GQE
{
  TimerSystem::TimerSystem(GQE::IApp& theApp) :
    ISystem("TimerSystem", theApp)
  {
  }

  TimerSystem::~TimerSystem()
  {
  }

  void TimerSystem::AddProperties(GQE::IEntity* theEntity)
  {
    theEntity->mProperties.Add<typeTimerLists>("Timers", typeTimerLists());
  }

  void TimerSystem::HandleInit(GQE::IEntity* theEntity)
  {
    typeTimerLists anTimerList = theEntity->mProperties.Get<typeTimerLists>("Timers");
    typeTimerLists::iterator anIter;
    for (anIter = anTimerList.begin(); anIter != anTimerList.end(); ++anIter)
    {
      TimerData& anContext = anIter->second;
      anContext.Clock.restart();
      anTimerList[anIter->first] = anContext;
    }
    theEntity->mProperties.Set<typeTimerLists>("Timers", anTimerList);
  }

  void TimerSystem::HandleCleanup(GQE::IEntity* theEntity)
  {
  }
  void TimerSystem::EntityUpdateFixed(GQE::IEntity* theEntity)
  {
    typeTimerLists anTimerList = theEntity->mProperties.Get<typeTimerLists>("Timers");
    typeTimerLists::iterator anIter;
    for (anIter = anTimerList.begin(); anIter != anTimerList.end(); ++anIter)
    {
      TimerData& anContext = anIter->second;
      sf::Time anElapsedTime = anContext.Clock.getElapsedTime();
      if (anElapsedTime >= anContext.Time && anContext.Active)
      {
        anContext.Clock.restart();
        if (!anContext.Repeat)
          anContext.Active = false;
        if (anContext.Event != "")
        {
          GQE::PropertyManager anProperties;
          anProperties.Add<GQE::IEntity*>("Entity", theEntity);
          anProperties.Add<TimerData>("TimerData",anContext);
          mApp.mEventManager.DoEvent(anContext.Event, &anProperties);
        }
      }
    }
    theEntity->mProperties.Set<typeTimerLists>("Timers", anTimerList);
  }
}
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
