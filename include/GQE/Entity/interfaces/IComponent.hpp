/**
 * The IComponent interface class used for all derived IComponent entities that
 * can be added to an IEntity class.
 *
 * @file include/GQE/Entity/interfaces/IComponent.hpp
 * @author Jacob Dix
 * @date 20120417 - Initial Release
 */
#ifndef ICOMPONENT_HPP_INCLUDED
#define ICOMPONENT_HPP_INCLUDED

#include <GQE/Core/interfaces/IApp.hpp>
#include <GQE/Entity/Entity_types.hpp>
#include <GQE/Entity/interfaces/TProperty.hpp>

namespace GQE
{
  /// The component interface class used by all IEntity derived classes
  class GQE_API IComponent
  {
    public:
      /**
       * IComponent constructor
       * @param[in] theComponentID to use for this component object
       * @param[in] theApp is the address to the App derived class
       */
      IComponent(const typeComponentID theComponentID, IApp& theApp);

      /**
       * IComponent deconstructor
       */
      virtual ~IComponent();

      /**
       * GetID will return the ID used to identify this component object.
       * @return GQE::typeCompomentID is the ID for this component object.
       */
      const typeComponentID GetID(void) const;

      /**
       * DoInit is responsible for initializing this component.  HandleCleanup will
       * be called if mCleanup is true so Derived classes should always call
       * IComponent::DoInit() first before initializing their assets.
       */
      virtual void DoInit(IEntity* theEntity);

      /**
       * ReInit is responsible for Reseting this component.
       */
      virtual void ReInit(void) = 0;

      /**
       * DeInit is responsible for marking this component to be cleaned up.
       */
      void DeInit(void);

      /**
       * IsInitComplete will return true if the DoInit method has been called
       * for this component.
       * @return true if DoInit has been called, false otherwise or if DeInit
       *         was called.
       */
      bool IsInitComplete(void);

      /**
       * CanRemove will return true if mRemoved is true;
       * @return true if the componet has been cleaned.
       */
      bool CanRemove(void);

      /**
       * HandleEvents is responsible for handling input events for this
       * component when it is the active State.
       * @param[in] theEvent to process from the App class Loop method
       */
      virtual void HandleEvents(sf::Event theEvent) = 0;

      /**
       * UpdateFixed is responsible for handling all State fixed update needs for
       * this component.
       */
      virtual void UpdateFixed(void) = 0;

      /**
       * UpdateVariable is responsible for handling all component variable update
       * needs for this component.
       * @param[in] theElapsedTime since the last Draw was called
       */
      virtual void UpdateVariable(float theElapsedTime) = 0;

      /**
       * Draw is responsible for handling all Drawing needs for this component
       */
      virtual void Draw(void) = 0;

      /**
       * HandleCleanup is responsible for calling Cleanup if this class has been
       * flagged to be cleaned up after it completes the game loop.
       */
      void HandleCleanup(void);

      /**
       * MakeClone is responsible for creating a clone of this IComponent
       * derived class and returning it as part of the Prototype and Instance
       * system. The values of the IComponent will also be copied into the
       * clone.
       * @return pointer to the IComponent derived class clone that was created
       */
      virtual IComponent* MakeClone()=0;
    protected:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Address to the App class
      IApp&   mApp;
      /// Pointer to the attached IEntity class
      IEntity* mEntity;

      /**
       * Cleanup is responsible for performing any cleanup required before
       * this component is removed.
       */
      virtual void Cleanup(void);
    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// The component ID
      const typeComponentID mComponentID;
      /// Boolean that indicates that DoInit has been called
      bool                  mInit;
      /// component needs to be cleaned up at the end of the next game loop
      bool                  mCleanup;
      ///component is cleaned and ready to be removed;
      bool                  mRemove;

      /**
       * Our copy constructor is private because we do not allow copies of
       * our Singleton class
       */
      IComponent(const IComponent&);  // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our Singleton class
       */
      IComponent& operator=(const IComponent&); // Intentionally undefined
  };
} // namespace GQE

#endif // ICOMPONENT_HPP_INCLUDED

/**
 * @class GQE::IComponent
 * @ingroup Entity
 * The IComponent class represents the interface used by all Components that
 * can be added to an IEntity derived class. Components enable IEntity classes
 * to differ from each other without requiring the game designer to create
 * complicated inheritance models for all their game entities.
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

