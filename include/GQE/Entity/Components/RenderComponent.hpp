/**
 * Provides a render component to be added to an IEntity class which is used to
 * render a texture representing the IEntity class to which it is registered.
 *
 * @file include/GQE/Entity/Components/RenderComponent.hpp>
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#ifndef RENDER_COMPONENT_HPP_INCLUDED
#define RENDER_COMPONENT_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <GQE/Core/assets/ImageAsset.hpp>
#include <GQE/Entity/Entity_types.hpp>
#include <GQE/Entity/interfaces/IComponent.hpp>

namespace GQE
{
  /// Provides RenderComponent component class for use with IEntity derived classes
  class GQE_API RenderComponent : public IComponent
  {
    public:
      /**
       * RenderComponent constructor
       * @param[in] theApp is the address to the App derived class
       * @param[in] theEntityManager to use for managing entities
       * @param[in] theAssetID to use for the Image for this component
       * @param[in] theLoadStyle (File,Mem,Network) to use when loading this asset
       * @param[in] theLoadTime (Now,Later) of when to load this asset
       */
      RenderComponent(IApp& theApp,
          const typeAssetID theAssetID,
          AssetLoadTime theLoadTime = AssetLoadLater,
          AssetLoadStyle theLoadStyle = AssetLoadFromFile,
          AssetDropTime theDropTime = AssetDropAtZero);

      /**
       * RenderComponent deconstructor
       */
      virtual ~RenderComponent();

      /**
       * DoInit is responsible for initializing this component.  HandleCleanup will
       * be called if mCleanup is true so Derived classes should always call
       * RenderComponent::DoInit() first before initializing their assets.
       * @param[in] theEntity that has this RenderComponent
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

      /**
       * MakeClone is responsible for cloning this RenderComponent and is used by
       * the Prototype class to create IEntity "templates" from which to clone to
       * create another IEntity class.
       * @return pointer to a cloned RenderComponent class
       */
      IComponent* MakeClone();

    protected:
      /**
       * Cleanup is responsible for performing any cleanup required before
       * this component is removed.
       */
      virtual void Cleanup(void);

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// The ImageAsset to use for this RenderComponent
      ImageAsset mImage;
      /// The SFML Sprite to use to represent this RenderComponent
      sf::Sprite mSprite;

      /**
       * Our copy constructor is private because we do not allow copies of
       * our Singleton class
       */
      RenderComponent(const RenderComponent&);  // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our Singleton class
       */
      RenderComponent& operator=(const RenderComponent&); // Intentionally undefined
  };
} // namespace GQE

#endif // RENDER_COMPONENT_HPP_INCLUDED

/**
 * @class GQE::RenderComponent
 * @ingroup Entity
 * The RenderComponent class provides rendering capabilities for an IEntity
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
