/**
 * Provides the IEvent interface class for storing IEvent values.
 *
 * @file include/GQE/Core/interfaces/IEvent.hpp
 * @author Ryan Lindeman
 * @date 20120630 - Initial Release
 */
#ifndef IEVENT_HPP_INCLUDED
#define IEVENT_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// Provides the interface for all IEvent properties
  class GQE_API IEvent
  {
    public:
      /// Class that represents the type for this class
      class Type_t
      {
        private:
          std::string mName;
        public:
          explicit Type_t(std::string theName) : mName(theName) {}
          std::string Name() const
          {
            return mName;
          };
      };

      /**
       * IEvent default constructor
       * @param[in] theType of event this IEvent represents
       * @param[in] theEventID to use for this IEvent
       */
      IEvent(std::string theType, const typeEventID theEventID);

      /**
       * IEvent destructor
       */
      virtual ~IEvent();

      /**
       * GetType will return the Type_t type for this property
       * @return the Type_t class for this property
       */
      Type_t* GetType(void);

      /**
       * GetID will return the Entity ID used for this event.
       * @return the event ID for this event
       */
      const typeEventID GetID(void) const;

      /**
       * DoEvent will be called to perform the IEvent and will be defined by
       * the derived IEvent class.
       */
      virtual void DoEvent(void* theContext = NULL) = 0;

    protected:
      /**
       * SetType is responsible for setting the type of class this IEvent
       * class represents and is usually called by the IEvent derived class
       * to set theType.
       * @param[in] theType to set for this IEvent derived class
       */
      void SetType(std::string theType);

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// The type that represents this class
      Type_t mType;
      /// The event ID assigned to this IEvent derived class
      const typeEventID mEventID;
  }; // class IEvent
} // namespace GQE
#endif //IEVENT_HPP_INCLUDED

/**
 * @class GQE::IEvent
 * @ingroup Core
 * The IEvent class provides an interface for the EventManager class to store
 * all registered IEvent derived classes. The most common of which is the
 * TEvent class which calls the () operator of the registered class to perform
 * some event functionality. An example of using the EventManager class and its
 * associated IEvent classes can be found in the IState class.
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
