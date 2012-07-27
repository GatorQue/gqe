/**
 * Provides the MovementSystem class for handing all entity movement in a game.
 *
 * @file include/GQE/Entity/systems/MovementSystem.hpp
 * @author Jacob Dix
 * @date 20120611 - Initial Release
 * @date 20120616 - Adjustments for new PropertyManager class
 * @date 20120618 - Use IEntity not Instance and changed AddPrototype to AddProperties
 * @date 20120620 - Fix typo in destructor method description
 * @date 20120622 - Changed HandleInit and HandleCleanup
 * @date 20120623 - Improved documentation and adjusted some properties
 * @date 20120630 - Added SpriteRect to list of dependent properties
 */
#ifndef MOVEMENT_SYSTEM_HPP_INCLUDED
#define MOVEMENT_SYSTEM_HPP_INCLUDED

#include <SFML/System.hpp>
#include <GQE/Entity/interfaces/ISystem.hpp>
#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
  /// The MovementSystem for managing all IEntity movement in a game
  class GQE_API MovementSystem : public ISystem
  {
    public:
      /**
       * MovementSystem Constructor.
       * @param[in] theApp is the current GQE app.
       */
      MovementSystem(IApp& theApp);

      /**
       * MovementSystem Destructor.
       */
      virtual ~MovementSystem();

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
      virtual void HandleInit(IEntity* theEntity);

      /**
       * HandleCleanup is called when the IEntity that was added is finally
       * dropped from this ISystem and gives the derived ISystem class a chance
       * to perform any custom work before the IEntity is deleted.
       */
      virtual void HandleCleanup(IEntity* theEntity);

      /**
       * HandleScreenWrap is called by either UpdateFixed or UpdateVariable to
       * perform the screen wrapping calculations. The screen will wrap if the
       * IEntity has moved just beyond its SpriteRect width or height on left
       * or top or just beyond the screen width or height on right and bottom.
       * @param[in] theEntity to peform screen wrap calculation
       * @param[in] thePosition is the current position of the sprite
       */
      virtual void HandleScreenWrap(IEntity* theEntity, sf::Vector2f* thePosition);
    private:
  }; // class MovementSystem
} // namespace GQE

#endif // MOVEMENT_SYSTEM_HPP_INCLUDED

/**
 * @class GQE::MovementSystem
 * @ingroup Entity
 * The MovementSystem class is used to update the IElement position based on the
 * following properties it provides:
 * - bFixedMovement: A boolean indicating fixed movement math should be used
 * - bScreenWrap: A boolean indicating that the position should wrap from one side to the other
 * - fRotationalVelocity: The float representing the rotational velocity of this IEntity
 * - fRotationalAcceleration: The float representing the rotational velocity of this IEntity
 * - vAcceleration: The sf::Vector2f representing the acceleration of this IEntity
 * - vVelocity: The sf::Vector2f representing the velocity of this IEntity
 * The MovementSystem class makes use of the following properties provided by the
 * RenderSystem class:
 * - fRotation: The float representing the current IEntity rotation
 * - rSpriteRect: To determine when to perform screen wrap around
 * - vPosition: The sf::Vector2f representing the current IEntity position
 * If the FixedMovement property is true then the following math is used in the
 * UpdateFixed method:
 * vVelocity += vAcceleration
 * vPosition += vVelocity
 * fRotationalVelocity += fRotationalAcceleration
 * fRotation += fRotationalVelocity
 * If the FixedMovement property is false then the following math is used in
 * the UpdateVariable method:
 * vVelocity += vAcceleration * theElapsedTime
 * vPosition += vVelocity * theElapsedTime
 * fRotationalVelocity += fRotationalAcceleration * theElapsedTime
 * fRotation += fRotationalVelocity * theElapsedTime
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
