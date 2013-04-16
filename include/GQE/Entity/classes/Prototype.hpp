/**
 * Provides the Prototype class that can be used as a template class to produce
 * Instance classes.
 *
 * @file include/GQE/Entity/classes/Prototype.hpp>
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120616 - Fixed MakeInstance method parameter comments
 * @date 20120620 - Use Prototype to manage and delete eventually all Instances created
 * @date 20120630 - Add new GetInstance method to retrieve specific instance
 * @date 20120630 - Add Destroy, DestroyInstance, and DropAllInstance methods
 * @date 20120702 - Add IState to constructor and HandleCleanup
 */
#ifndef PROTOTYPE_HPP_INCLUDED
#define PROTOTYPE_HPP_INCLUDED

#include <map>
#include <vector>
#include <GQE/Entity/Entity_types.hpp>
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
       * @param[in] theOrder to assign to this prototype and each Instance
       */
      Prototype(const typePrototypeID thePrototypeID, Uint32 theOrder = 0);

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
       * Destroy is responsible for removing this Prototype and all of its
       * Instances.
       */
      virtual void Destroy(void);

      /**
       * DestroyInstance is responsible for dropping and removing theEntityID
       * Instance specified and is usually called by the Instance class itself.
       */
      void DestroyInstance(const typeEntityID theEntityID);

      /**
       * GetInstance is responsible for returning the Instance class that
       * matches theEntityID provided. Each Prototype class keeps track of all
       * Instance classes that were created using the MakeInstance method below.
       * @param[in] theEntityID to find and return
       * @return a pointer to theInstance class found, NULL otherwise
       */
      Instance* GetInstance(const typeEntityID theEntityID) const;

      /**
       * MakeInstance is responsible for creating an Instance of this Prototype
       * class using the MakeClone methods provided by the IComponent and
       * IProperty derived classes.
       * @return a pointer to the Instance class created.
       */
      Instance* MakeInstance(void);

      virtual void Write(std::fstream& theFileStream);

      virtual void Read(std::fstream& theFileStream);
    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// The prototype ID assigned to this Prototype class
      const typePrototypeID mPrototypeID;
      /// A map of all Instance classes created by this Prototype
      std::map<const typeEntityID, Instance*> mInstances;
      /// A linked list of all Instance classes to destroy during HandleCleanup
      std::vector<Instance*> mCleanup;

      /**
       * DropAllInstances is responsible for dropping all Instance classes that
       * this Prototype is responsible for as part of the removal of this
       * Prototype class.
       */
      void DropAllInstances(void);

      /**
       * HandleCleanup is responsible for deleting all Instance classes from
       * our Cleanup list. Instances are placed in our Cleanup list by either
       * the DropAllInstances or DropInstance methods.
       */
      void HandleCleanup(void* theContext);
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

