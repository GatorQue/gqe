/**
 * Provides a class that manages events for some class (e.g. IState). By
 * providing the EventManager class, other classes can easily add an Event
 * callback mechanism to their system if desired.
 *
 * @file src/GQE/Core/classes/EventManager.cpp
 * @author Ryan Lindeman
 * @date 20120630 - Initial Release
 */

#include <GQE/Core/classes/EventManager.hpp>

namespace GQE
{
  EventManager::EventManager()
  {
  }

  EventManager::~EventManager()
  {
    // Make sure to remove all registered properties on desstruction
    std::map<const typeEventID, IEvent*>::iterator anEventIter;
    for(anEventIter = mList.begin();
        anEventIter != mList.end();
        ++anEventIter)
    {
      IEvent* anEvent = (anEventIter->second);
      delete anEvent;
      anEvent = NULL;
    }
  }

  bool EventManager::HasID(const typeEventID theEventID) const
  {
    bool anResult = false;

    // See if theEventID was found in our list of properties
    anResult = (mList.find(theEventID) != mList.end());

    // Return true if theEventID was found above, false otherwise
    return anResult;
  }

  void EventManager::DoEvents(void* theContext)
  {
    std::map<const typeEventID, IEvent*>::const_iterator anIter =
      mList.begin();
    while(anIter != mList.end())
    {
      // Get the event to execute
      IEvent* anEvent = anIter->second;

      // Iterate to the next event
      anIter++;

      // Now call DoEvent for this event with theContext provided
      anEvent->DoEvent(theContext);
    }
  }
  void EventManager::DoEvent(typeEventID theEventID,void* theContext)
  {
    std::map<const typeEventID, IEvent*>::const_iterator anIter =
      mList.find(theEventID);
    if (anIter != mList.end())
    {
      // Get the event to execute
      IEvent* anEvent = anIter->second;
      // Now call DoEvent for this event with theContext provided
      anEvent->DoEvent(theContext);
    }
  }

  IEvent* EventManager::Get(const typeEventID theEventID)
  {
    IEvent* anResult = NULL;

    // Create an iterator to find the IEvent class
    std::map<const typeEventID, IEvent*>::const_iterator anIter;
    anIter = mList.find(theEventID);
    if(anIter != mList.end())
    {
      anResult = anIter->second;
    }

    // Return the event pointer or NULL otherwise if no event was found
    return anResult;
  }

  void EventManager::Add(IEvent* theEvent)
  {
    if(theEvent != NULL)
    {
      if(mList.find(theEvent->GetID()) == mList.end())
      {
        mList.insert(std::pair<const typeEventID, IEvent*>(theEvent->GetID(), theEvent));
      }
      else
      {
        ELOG() << "EventManager::Add(" << theEvent->GetID() << ") already exists!" << std::endl;
      }
    }
    else
    {
      ELOG() << "EventManager::Add() null IEvent pointer provided!" << std::endl;
    }
  }
} // namespace GQE

/**
 * Copyright (c) 2010-2012 Ryan Lindeman
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
