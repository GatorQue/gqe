/**
 * Provides the Instance class that is produced by the Prototype class.
 *
 * @file include/GQE/Entity/classes/Instance.hpp>
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120616 - Fixed constructor parameter comments
 * @date 20120618 - Move ID related stuff to IEntity base class
 */
#ifndef INSTANCE_HPP_INCLUDED
#define INSTANCE_HPP_INCLUDED

#include <GQE/Entity/interfaces/IEntity.hpp>
#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
  /// Provides the Instance class which is produced by the Prototype class
  class GQE_API Instance : public IEntity
  {
    public:
      /**
       * Instance default constructor
       * @param[in] thePrototype class address used to create this Instance class
       */
      Instance(Prototype& thePrototype);

      /**
       * Instance deconstructor
       */
      virtual ~Instance();

      /**
       * GetPrototype will return the Prototype class used to create this
       * Instance class, so you can create another Instance if desired.
       * @return a pointer to the Prototype class used to create this Instance
       */
      Prototype& GetPrototype();
    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// The address to the Prototype class used to create this Instance
      Prototype& mPrototype;
  };
}

#endif // INSTANCE_HPP_INCLUDED

/**
 * @class GQE::Instance
 * @ingroup Entity
 * The Instance class represents an IEntity derived class that was produced by
 * the Prototype class which also derives from IEntity. By making a Prototype
 * class and using its MakeInstance call you can create multiple copies of the
 * Prototype class.
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

