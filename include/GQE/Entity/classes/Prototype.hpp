/**
 * Provides the Prototype class that can be used as a template class to produce
 * Instance classes.
 *
 * @file include/GQE/Entity/classes/Prototype.hpp>
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120616 - Fixed MakeInstance method parameter comments
 * @date 20120620 - Use Prototype to manage and delete eventually all Instances created
 */
#ifndef PROTOTYPE_HPP_INCLUDED
#define PROTOTYPE_HPP_INCLUDED

#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  /// Provides Prototype class for creating Instance classes
  class GQE_API Prototype : public IEntity
  {
    public:
      /**
       * Prototype default constructor
       * @param[in] thePrototypeID to use for this prototype
       */
      Prototype(const typePrototypeID thePrototypeID);

      /**
       * Prototype destructor
       */
      virtual ~Prototype();

      /**
       * GetID will return the ID used for this prototype.
       * @return the prototype ID for this prototype
       */
      const typePrototypeID GetID(void) const;

      /**
       * MakeInstance is responsible for creating an Instance of this Prototype
       * class using the MakeClone methods provided by the IComponent and
       * IProperty derived classes.
       * @return a pointer to the Instance class created.
       */
      Instance* MakeInstance(void);
    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// The prototype ID assigned to this Prototype class
      const typePrototypeID mPrototypeID;
		  /// A linked list of all Instance classes created by this Prototype
		  std::vector<Instance*> mInstances;
  };
} // namespace GQE

#endif // PROTOTYPE_HPP_INCLUDED

/**
 * @class GQE::Prototype
 * @ingroup Entity
 * The Prototype class represents a template IEntity derived class that can be
 * used to create Instance classes. This makes creating similar classes easier
 * since it centralizes the creation of complicated IEntity derived classes
 * easier.
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

