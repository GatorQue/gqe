/**
 * Provides a system for storing custom property values.
 *
 * @file include/GQE/Entity/interfaces/TProperty.hpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#ifndef TPROPERTY_HPP_INCLUDED
#define TPROPERTY_HPP_INCLUDED

#include <GQE/Entity/Entity_types.hpp>
#include <GQE/Entity/interfaces/IProperty.hpp>
#include <typeinfo>

namespace GQE
{
  /// The Template version of the IProperty class for custom property values
  template<class TYPE=Uint32>
    class TProperty : public IProperty
  {
    public:
      /**
       * TProperty default constructor
       * @param[in] thePropertyID to use for this property
       */
      TProperty(const typePropertyID thePropertyID) :
        IProperty(typeid(TYPE).name(), thePropertyID)
    {
    }

      /**
       * GetValue will return the property value
       * @return the property value
       */
      TYPE GetValue()
      {
        return mValue;
      }

      /**
       * SetValue will set the property value to the value
       * provided.
       */
      void SetValue(TYPE theValue)
      {
        mValue=theValue;
      }

      /**
       * Update will be called giving each property a chance to change itself
       * during the Update phase of the GameLoop (see IEntity::UpdateFixed)
       */
      void Update()
      {
      }

      /**
       * MakeClone is responsible for creating a clone of this IProperty
       * derived class and returning it as part of the Prototype and Instance
       * system. The value of the Property will also be copied into the clone.
       * @return pointer to the IProperty derived class clone that was created
       */
      IProperty* MakeClone()
      {
        TProperty<TYPE>* anProperty = new(std::nothrow) TProperty<TYPE>(GetID());

        // Make sure new didn't fail before setting the value for this property
        if(NULL != anProperty)
        {
          anProperty->SetValue(mValue);
        }

        // Return cloned anProperty or NULL if none was created
        return anProperty;
      }
    private:
      TYPE mValue;
  };
} // namespace GQE
#endif // TPROPERTY_HPP_INCLUDED

/**
 * @class GQE::TProperty
 * @ingroup Entity
 * The TProperty class is a templatized version of the IProperty class that
 * makes it easy to create your own IProperty derived classes using a custom
 * value.
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

