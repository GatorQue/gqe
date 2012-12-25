/**
 * Provides the CollisionSystem class for handing all entity movement in a game.
 *
 * @file src/GQE/Entity/systems/CollisionSystem.cpp
 * @author Jacob Dix
 * @date 20120611
 */
#include <SFML/Graphics.hpp>
#include <GQE/Entity/systems/CollisionSystem.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  CollisionSystem::CollisionSystem(IApp& theApp):
    ISystem("CollisionSystem",theApp)
  {
  }
  CollisionSystem::~CollisionSystem()
  {
  }

  void CollisionSystem::AddProperties(IEntity* theEntity)
  {
    theEntity->mProperties.Add<sf::IntRect>("rBoundingBox",sf::IntRect());
    theEntity->mProperties.Add<typeEventID>("sCollisionReaction","");
    theEntity->mProperties.Add<typeEventID>("sCollisionEffect","");
    theEntity->mProperties.Add<sf::IntRect>("rIntersectRect",sf::IntRect());
  }

  void CollisionSystem::HandleInit(IEntity* theEntity)
  {
    if(theEntity->HasSystem("MovementSystem"))
    {
      mMovables.push_back(theEntity);
    }
  }

  void CollisionSystem::HandleEvents(sf::Event theEvent)
  {
  }

  void CollisionSystem::UpdateFixed()
  {
    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    std::vector<IEntity*>::iterator anMovablesIter;
    anMovablesIter=mMovables.begin();
    while(anMovablesIter!=mMovables.end())
    {
      IEntity* anMovableEntity=(*anMovablesIter);
      anIter = mEntities.begin();
      while(anIter != mEntities.end())
      {
        std::deque<IEntity*>::iterator anQueue = anIter->second.begin();
        while(anQueue != anIter->second.end())
        {
          // Get the IEntity address first
          GQE::IEntity* anEntity = *anQueue;

          // Increment the IEntity iterator second
          anQueue++;
          //Detect Collision
          sf::Vector2f anPositionA=anMovableEntity->mProperties.Get<sf::Vector2f>("vPosition");
          sf::IntRect anRectA=anMovableEntity->mProperties.Get<sf::IntRect>("rBoundingBox");

          sf::Vector2f anPositionB=anEntity->mProperties.Get<sf::Vector2f>("vPosition");
          sf::IntRect anRectB=anEntity->mProperties.Get<sf::IntRect>("rBoundingBox");

          sf::IntRect anIntersectRect;
#if SFML_VERSION_MAJOR <2
          anRectA.left+=anPositionA.x;
          anRectA.right+=anPositionA.x;
          anRectA.top+=anPositionA.y;
          anRectA.bottom+=anPositionA.y;
          anRectB.left+=anPositionB.x;
          anRectB.right+=anPositionB.x;
          anRectB.top+=anPositionB.y;
          anRectB.bottom+=anPositionB.y;
          if(anRectA.Intersects(anRectB,anIntersectRect))
#else
          anRectA.left+=anPositionA.x;
          anRectA.top+=anPositionA.y;
          anRectB.left+=anPositionB.x;
          anRectB.top+=anPositionB.y;
          if(anRectA.intersects(anRectB,anIntersectRect))
#endif
          {
			  anMovableEntity->mProperties.Set<sf::IntRect>("rIntersectRect",anIntersectRect);
			  anEntity->mProperties.Set<sf::IntRect>("rIntersectRect",anIntersectRect);
			  typeEventID anCollisionReactionA = anMovableEntity->mProperties.Get<std::string>("sCollisionReaction");
			  typeEventID anCollisionReactionB = anEntity->mProperties.Get<std::string>("sCollisionReaction");
			  typeEventID anCollisionEffectA = anMovableEntity->mProperties.Get<std::string>("sCollisionEffect");
			  typeEventID anCollisionEffectB = anEntity->mProperties.Get<std::string>("sCollisionEffect");
			  if(mEventManager.HasID(anCollisionReactionA))
			  {
				  mEventManager.Get(anCollisionReactionA).DoEvent(anMovableEntity);
			  }
			  if(mEventManager.HasID(anCollisionReactionB))
			  {
				  mEventManager.Get(anCollisionReactionB).DoEvent(anEntity);
			  }
			  if(mEventManager.HasID(anCollisionEffectA))
			  {
				  mEventManager.Get(anCollisionEffectA).DoEvent(anEntity);
			  }
			  if(mEventManager.HasID(anCollisionEffectB))
			  {
				  mEventManager.Get(anCollisionEffectB).DoEvent(anMovableEntity);
			  }
			  
          }
        } // while(anQueue != anIter->second.end())

        // Increment map iterator
        anIter++;
      } //while(anIter != mEntities.end())
      anMovablesIter++;
    }
  }

  void CollisionSystem::UpdateVariable(float theElapsedTime)
  {
    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end())
    {
      std::deque<IEntity*>::iterator anQueue = anIter->second.begin();
      while(anQueue != anIter->second.end())
      {
        // Get the IEntity address first
        GQE::IEntity* anEntity = *anQueue;

        // Increment the IEntity iterator second
        anQueue++;

        // Are we NOT using fixed movement mathematics?
        if(anEntity->mProperties.Get<bool>("bFixedMovement") == false)
        {
          
        } //if(anEntity->mProperties.Get<bool>("bFixedMovement") == false)
      } // while(anQueue != anIter->second.end())

      // Increment map iterator
      anIter++;
    } //while(anIter != mEntities.end())
  }

  void CollisionSystem::Draw()
  {
  }

  void CollisionSystem::HandleCleanup(IEntity* theEntity)
  {
    // Do nothing
  }

} // namespace GQE

/**
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
