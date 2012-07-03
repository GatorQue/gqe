/**
 * Provides a system for storing custom Event capable classes.
 *
 * @file include/GQE/Core/interfaces/TEvent.hpp
 * @author Ryan Lindeman
 * @date 20120630 - Initial Release
 */
#ifndef TEVENT_HPP_INCLUDED
#define TEVENT_HPP_INCLUDED

#include <typeinfo>
#include <GQE/Core/interfaces/IEvent.hpp>
#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// The Template version of the IEvent class for custom event capable classes
  template<class TCLASS, class TCONTEXT>
    class TEvent : public IEvent
  {
    public:
      /// Declare the event function typedef for people to use to pass functions
      typename typedef void (TCLASS::*typeEventFunc)(TCONTEXT* theContext);

      /**
       * TEvent default constructor
       * @param[in] theEventID to use for this event
       */
      TEvent(const typeEventID theEventID, TCLASS& theEventClass, typeEventFunc theEventFunc) :
        IEvent(typeid(TCLASS).name(), theEventID),
        mEventClass(theEventClass),
        mEventFunc(theEventFunc)
      {
      }

      /**
       * DoEvent will be called to perform the IEvent and will be defined by
       * the derived IEvent class.
       */
      virtual void DoEvent(void* theContext = NULL)
      {
        TCONTEXT* anContext = static_cast<TCONTEXT*>(theContext);
        // Call the member method provided of the event class provided
        (mEventClass.*mEventFunc)(anContext);
      }

    private:
      /// The address to the event class to use when calling the event function
      TCLASS&       mEventClass;
      /// The function to call in the event class
      typeEventFunc mEventFunc;
  }; // class TEvent
} // namespace GQE
#endif // TEVENT_HPP_INCLUDED

/**
 * @class GQE::TEvent
 * @ingroup Core
 * The TEvent class is a templatized version of the IEvent class that
 * makes it easy to create your own custom event capable class (one that
 * provides the operator() method that will be called by the DoEvent method).
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
