/**
 * Provides the AnimationSystem class for handing all IEntity animation in a game.
 *
 * @file include/GQE/Entity/systems/AnimationSystem.cpp
 * @author Jacob Dix
 * @date 20120623 - Initial Release
 */
#ifndef ANIMATION_SYSTEM_HPP_INCLUDED
#define ANIMATION_SYSTEM_HPP_INCLUDED

#include <GQE/Entity/interfaces/ISystem.hpp>
#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
  /// The AnimationSystem provides animation support for all IEntity classes in a game
  class GQE_API AnimationSystem : public ISystem
  {
    public:
      /**
       * AnimationSystem Constructor.
       * @param[in] theApp is the current GQE app.
       */
	    AnimationSystem(IApp& theApp);

      /**
       * AnimationSystem Destructor.
       */
	    virtual ~AnimationSystem();

      /**
       * AddProperties is responsible for adding the properties used by this
       * ISystem derived class to the IEntity derived class provided.
       * @param[in] theEntity to add the properties to.
       */
	    virtual void AddProperties(IEntity* theEntity);

	    /**
	     * HandleEvents is responsible for letting each Instance class have a
	     * chance to handle theEvent specified.
	     * @param[in] theEvent to handle
	     */
	    virtual void HandleEvents(sf::Event theEvent);

	    /**
	     * UpdateFixed is called a specific number of times every game loop and
	     * this method will allow each Instance class a chance to have its
	     * UpdateFixed method called for each game loop iteration.
	     */
	    virtual void UpdateFixed(void);

	    /**
	     * UpdateVariable is called every time the game loop draws a frame and
	     * includes the elapsed time between the last UpdateVariable call for
	     * use with equations that use time as a variable. (e.g. physics velocity
	     * and acceleration equations).
	     */
	    virtual void UpdateVariable(float theElapsedTime);

	    /**
	     * Draw is called during the game loop after events and the fixed update
	     * loop calls are completed and depends largely on the speed of the
	     * computer to determine how frequently it will be called. This gives the
	     * EntityManager a chance to call the Draw method for each Instance
	     * class.
	     */
	    virtual void Draw(void);

    protected:
      /**
       * HandleInit is called to allow each derived ISystem to perform any
       * initialization steps when a new IEntity is added.
       */
      virtual void HandleInit(GQE::IEntity* theEntity);

      /**
       * HandleCleanup is called when the IEntity that was added is finally
       * dropped from this ISystem and gives the derived ISystem class a chance
       * to perform any custom work before the IEntity is deleted.
       */
      virtual void HandleCleanup(GQE::IEntity* theEntity);
	  private:
  };
} // namespace GQE
#endif // ANIMATION_SYSTEM_HPP_INCLUDED

/**
 * @class GQE::AnimationSystem
 * @ingroup Entity
 * The AnimationSystem class is used to update the SpriteRect property of each
 * IEntity to cause its Sprite image to show animation. The properties provided
 * by this ISystem are as follows:
 * - FrameClock: The sf::Clock which keeps track of when IEntity was last animated
 * - FrameDelay: The float which determines in seconds when to animate next
 * - FrameModifier: The sf::Vector2u to determine the next animation frame to use
 * - FrameRect: The sf::IntRect which acounts for the animation boundaries
 * The AnimationSystem class makes use of the following properties provided by the
 * RenderSystem class:
 * - SpriteRect: The sf::IntRect currently being shown
 * The math involved is essentially this:
 * ImageRect.x += (ImageRect.Width*FrameModifier.x)
 * ImageRect.y += (ImageRect.Height*FrameModifier.y)
 * And the resulting ImageRect.x and ImageRect.y values are limited to
 * FrameRect.x and FrameRect.y. Your FrameModifier values will typically be set
 * to x=1,y=0 or vice-versa depending on how your animations are organized
 * (horizontal or vertical).
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
