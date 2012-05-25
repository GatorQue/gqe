/**
 * Provides a solid component to be added to an IEntity class.
 *
 * @file include/GQE/Entity/Components/SolidComponent.hpp>
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#ifndef SOLID_COMPONENT_HPP_INCLUDED
#define SOLID_COMPONENT_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <GQE/Entity/Entity_types.hpp>
#include <GQE/Entity/interfaces/IComponent.hpp>

namespace GQE
{
  /// Provides the SolidComponent derived class
  class GQE_API SolidComponent : public IComponent
  {
    public:
      /**
       * SolidComponent constructor
       * @param[in] theApp is the address to the App derived class
       */
      SolidComponent(IApp& theApp);

      /**
       * SolidComponent deconstructor
       */
      virtual ~SolidComponent();

      /**
       * DoInit is responsible for initializing this component.  HandleCleanup will
       * be called if mCleanup is true so Derived classes should always call
       * SolidComponent::DoInit() first before initializing their assets.
       */
      virtual void DoInit(IEntity* theEntity);

      /**
       * ReInit is responsible for Reseting this component.
       */
      virtual void ReInit(void);

      /**
       * HandleEvents is responsible for handling input events for this
       * component when it is the active State.
       * @param[in] theEvent to process from the App class Loop method
       */
      virtual void HandleEvents(sf::Event theEvent);

      /**
       * UpdateFixed is responsible for handling all State fixed update needs for
       * this component.
       */
      virtual void UpdateFixed(void);

      /**
       * UpdateVariable is responsible for handling all component variable update
       * needs for this component.
       * @param[in] theElapsedTime since the last Draw was called
       */
      virtual void UpdateVariable(float theElapsedTime);

      /**
       * Draw is responsible for handling all Drawing needs for this component
       */
      virtual void Draw(void);

      IComponent* MakeClone();

    protected:
      /**
       * Cleanup is responsible for performing any cleanup required before
       * this component is removed.
       */
      virtual void Cleanup(void);

    private:
      ///Variables
      static std::vector<IEntity*> mEntities;

      /**
       * Our copy constructor is private because we do not allow copies of
       * our Singleton class
       */
      SolidComponent(const SolidComponent&);  // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our Singleton class
       */
      SolidComponent& operator=(const SolidComponent&); // Intentionally undefined
  };
} // namespace GQE

#endif // SOLID_COMPONENT_HPP_INCLUDED

/**
 * @class GQE::SolidComponent
 * @ingroup Entity
 * The SolidComponent class provides ??? capabilities for an IEntity
 * derived class.
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

