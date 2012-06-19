/**
 * Provides the IProperty interface class for storing IEntity wide values.
 *
 * @file include/GQE/Entity/interfaces/IProperty.hpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#ifndef IPROPERTY_HPP_INCLUDED
#define IPROPERTY_HPP_INCLUDED

#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
  /// Provides the interface for all IEntity properties
  class GQE_API IProperty
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
       * IProperty default constructor
       * @param[in] theType of property this property represents
       * @param[in] thePropertyID to use for this property
       */
      IProperty(std::string theType, const typePropertyID thePropertyID);

      /**
       * IProperty destructor
       */
      virtual ~IProperty();

      /**
       * GetType will return the Type_t type for this property
       * @return the Type_t class for this property
       */
      Type_t* GetType(void);

      /**
       * GetID will return the Property ID used for this property.
       * @return the property ID for this property
       */
      const typePropertyID GetID(void) const;

      /**
       * Update will be called for each IProperty registered with IEntity and
       * enable each IProperty derived class to perform Update related tasks
       * (e.g. frame counter, timer update, decreate in shields, etc).
       */
      virtual void Update() = 0;

      /**
       * MakeClone is responsible for creating a clone of this IProperty
       * derived class and returning it as part of the Prototype and Instance
       * system. The value of the Property will also be copied into the clone.
       * @return pointer to the IProperty derived class clone that was created
       */
      virtual IProperty* MakeClone() = 0;

    protected:
      /**
       * SetType is responsible for setting the type of class this IProperty
       * class represents and is usually called by the IProperty derived class
       * to set theType.
       * @param[in] theType to set for this IProperty derived class
       */
      void SetType(std::string theType);

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// The type that represents this class
      Type_t mType;

      /// The property ID assigned to this IProperty derived class
      const typePropertyID mPropertyID;
  };
} // namespace GQE
#endif //IPROPERTY_HPP_INCLUDED

/**
 * @class GQE::IProperty
 * @ingroup Entity
 * The IProperty class is represents the base class for all properties stored
 * in the IEntity derived classes. This enables creating custom properties
 * (e.g. hit points, shields, etc) that can be assigned to any IEntity derived
 * classes.
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

