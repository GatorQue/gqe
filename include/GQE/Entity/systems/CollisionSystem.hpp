/**
 * Provides the CollisionSystem class for handing AABB based Collisions between Entities.
 *
 * @file include/GQE/Entity/systems/CollisionSystem.hpp
 * @author Jacob Dix
 * @date
 */
#ifndef COLLISION_SYSTEM_HPP_INCLUDED
#define COLLISION_SYSTEM_HPP_INCLUDED

#include <SFML/System.hpp>
#include <GQE/Entity/interfaces/ISystem.hpp>
#include <GQE/Entity/Entity_types.hpp>
#include <GQE/Core/classes/EventManager.hpp>
namespace GQE
{
  /// The CollisionSystem for managing all IEntity movement in a game
  class GQE_API CollisionSystem : public ISystem
  {
    public:
		EventManager mEventManager;
      /**
       * CollisionSystem Constructor.
       * @param[in] theApp is the current GQE app.
       */
      CollisionSystem(IApp& theApp);

      /**
       * CollisionSystem Destructor.
       */
      virtual ~CollisionSystem();

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
			 * DestroyEvent
			 * 
			 */
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
      std::vector<IEntity*> mMovables;
  }; // class CollisionSystem
} // namespace GQE

#endif // COLLISION_SYSTEM_HPP_INCLUDED

/**
 * @class GQE::CollisionSystem
 * @ingroup Entity
 * The CollisionSystem class is used to detect and react to collisions based on the
 * following properties it provides:
 * - rBoundingBox: a rectangle for the size of the entity.
 * The MovementSystem class makes use of the following properties provided by the
 * RenderSystem class:
 * - vPosition: The sf::Vector2f representing the current IEntity position
 * The MovementSystem class makes use of the following properties provided by the
 * MovementSystem class:
 * * - bFixedMovement: A boolean indicating fixed movement math should be used
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
