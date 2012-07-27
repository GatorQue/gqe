/**
 * Provides a class that manages events for some class (e.g. IState). By
 * providing the EventManager class, other classes can easily add an Event
 * callback mechanism to their system if desired.
 *
 * @file include/GQE/Core/classes/EventManager.hpp
 * @author Ryan Lindeman
 * @date 20120630 - Initial Release
 */
#ifndef EVENT_MANAGER_HPP_INCLUDED
#define EVENT_MANAGER_HPP_INCLUDED

#include <map>
#include <typeinfo>
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/interfaces/IEvent.hpp>
#include <GQE/Core/interfaces/TEvent.hpp>

namespace GQE
{
  /// Provides the EventManager class for managing IEvent classes
  class GQE_API EventManager
  {
    public:
      /**
       * EventManager default constructor
       */
      EventManager();

      /**
       * EventManager deconstructor
       */
      virtual ~EventManager();

      /**
       * HasID returns true if theEventID specified exists in this
       * EventManager.
       * @param[in] theEventID to lookup in this EventManager
       * @return true if theEventID exists, false otherwise
       */
      bool HasID(const typeEventID theEventID) const;

      /**
       * DoEvents should be called to execute the DoEvent method for each
       * IEvent stored in this EventManager.
       * @param[in] theContext for the event, typically a different class
       */
      void DoEvents(void* theContext = NULL);

      /**
       * Get returns the IEvent that matches theEventID specified that was
       * previously added to the EventManager using the Add methods.
       * @param[in] theEventID to lookup in this EventManager
       * @return a pointer to the IEvent class or NULL otherwise
       */
      IEvent* Get(const typeEventID theEventID);

      /**
       * Add adds a new TEvent to be managed by the EventManager using the
       * class and member function specified without a context class specified.
       * @param[in] theEventID to use when creating the TEvent class
       * @param[in] theEventClass to use when calling theEventFunc
       * @param[in] theEventFunc to call in theEventClass
       */
      template<class TCLASS>
      void Add(const typeEventID theEventID, TCLASS& theEventClass,
        typename TEvent<TCLASS, void>::typeEventFunc theEventFunc)
      {
        // Only add the event if it doesn't already exist
        if(mList.find(theEventID) == mList.end())
        {
          TEvent<TCLASS,void>* anEvent =
            new(std::nothrow) TEvent<TCLASS,void>(theEventID, theEventClass, theEventFunc);
          if(anEvent != NULL)
          {
            mList.insert(std::pair<const typeEventID, IEvent*>(theEventID, anEvent));
          }
        }
      }

      /**
       * Add adds a new TEvent to be managed by the EventManager using the
       * class and member function and context class type specified.
       * @param[in] theEventID to use when creating the TEvent class
       * @param[in] theEventClass to use when calling theEventFunc
       * @param[in] theEventFunc to call in theEventClass
       */
      template<class TCLASS, class TCONTEXT>
      void Add(const typeEventID theEventID, TCLASS& theEventClass,
        typename TEvent<TCLASS, TCONTEXT>::typeEventFunc theEventFunc)
      {
        // Only add the event if it doesn't already exist
        if(mList.find(theEventID) == mList.end())
        {
          TEvent<TCLASS,TCONTEXT>* anEvent =
            new(std::nothrow) TEvent<TCLASS,TCONTEXT>(theEventID, theEventClass, theEventFunc);
          if(anEvent != NULL)
          {
            mList.insert(std::pair<const typeEventID, IEvent*>(theEventID, anEvent));
          }
        }
      }

      /**
       * Add will manage theEvent provided in this EventManager. Sometimes you might
       * want to create your own IEvent based classes instead of using the TEvent
       * class technique.
       * @param[in] theEvent is a pointer to a class that derives from IEvent
       */
      void Add(IEvent* theEvent);
    protected:

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// A map of all events available for this EventManager class
      std::map<const typeEventID, IEvent*> mList;

  }; // EventManager class
} // namespace GQE
#endif

/**
 * @class GQE::EventManager
 * @ingroup Core
 * The EventManager class is responsible for providing management for all
 * IEvent based classes used to provide Event calls to member functions of any
 * class provided.
 *
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
