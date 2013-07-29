/**
 * Provides the ICollisionSystem class for handing all entity movement in a game.
 *
 * @file src/GQE/Entity/systems/ICollisionSystem.cpp
 * @author Jacob Dix
 * @date 20120611 Initial release
 */
#include <SFML/Graphics.hpp>
#include <GQE/Entity/interfaces/ICollisionSystem.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  ICollisionSystem::ICollisionSystem(IApp& theApp) :
    ISystem("CollisionSystem",theApp)
  {
  }

  ICollisionSystem::~ICollisionSystem()
  {
  }

  void ICollisionSystem::AddProperties(IEntity* theEntity)
  {
    theEntity->mProperties.Add<sf::IntRect>("rBoundingBox",sf::IntRect());
    theEntity->mProperties.Add<typeEventID>("sCollisionReaction","");
    theEntity->mProperties.Add<typeEventID>("sCollisionEffect","");
    theEntity->mProperties.Add<typeCollisionData>("CollisionData",typeCollisionData());
    theEntity->mProperties.Add<bool>("bDebugDraw",false);
  }

  void ICollisionSystem::HandleInit(IEntity* theEntity)
  {
    if(theEntity->mProperties.Get<bool>("bMovable"))
    {
      mMovables.push_back(theEntity);
    }
  }

  void ICollisionSystem::HandleEvents(sf::Event theEvent)
  {
  }

  void ICollisionSystem::UpdateFixed()
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
          if(anEntity!=anMovableEntity)
          {
            //Detect Collision
            sf::Vector2f anPositionA=anMovableEntity->mProperties.Get<sf::Vector2f>("vPosition");
            sf::IntRect anRectA=anMovableEntity->mProperties.Get<sf::IntRect>("rBoundingBox");

            sf::Vector2f anPositionB=anEntity->mProperties.Get<sf::Vector2f>("vPosition");
            sf::IntRect anRectB=anEntity->mProperties.Get<sf::IntRect>("rBoundingBox");

            sf::IntRect anIntersectRect;
#if (SFML_VERSION_MAJOR < 2)
            anRectA.Left+=anPositionA.x;
            anRectA.Right+=anPositionA.x;
            anRectA.Top+=anPositionA.y;
            anRectA.Bottom+=anPositionA.y;
            anRectB.Left+=anPositionB.x;
            anRectB.Right+=anPositionB.x;
            anRectB.Top+=anPositionB.y;
            anRectB.Bottom+=anPositionB.y;
            if(anRectA.Intersects(anRectB,&anIntersectRect))
#else
            anRectA.left+=anPositionA.x;
            anRectA.top+=anPositionA.y;
            anRectB.left+=anPositionB.x;
            anRectB.top+=anPositionB.y;

            if(anRectA.intersects(anRectB,anIntersectRect))
#endif
            {
              typeCollisionData anData;
              anData.Collision = true;
              anData.IntersectRect = anIntersectRect;
							anData.MovingEntity = anMovableEntity;
							anData.OtherEntity = anEntity;
              anMovableEntity->mProperties.Set<typeCollisionData>("CollisionData",anData);
              anEntity->mProperties.Set<typeCollisionData>("CollisionData",anData);
              EntityCollision(anMovableEntity,anEntity,anData);
            }
          }
        } // while(anQueue != anIter->second.end())

        // Increment map iterator
        anIter++;
      } //while(anIter != mEntities.end())
      anMovablesIter++;
    }
  }

  void ICollisionSystem::UpdateVariable(float theElapsedTime)
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

  void ICollisionSystem::Draw()
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

        // Are we drawing the bounding box
        if(anEntity->mProperties.Get<bool>("bDebugDraw"))
        {
          sf::IntRect anBoundingBox = anEntity->mProperties.Get<sf::IntRect>("rBoundingBox");
          sf::Vector2f anPosition = anEntity->mProperties.Get<sf::Vector2f>("vPosition");
					sf::Vector2f anOrigin = anEntity->mProperties.Get<sf::Vector2f>("vOrigin");
					sf::Vector2f anScale = anEntity->mProperties.Get<sf::Vector2f>("vScale");
					anPosition-=anEntity->mProperties.Get<sf::Vector2f>("vOrigin");
#if (SFML_VERSION_MAJOR < 2)
          sf::Shape anShape = sf::Shape::Rectangle(sf::Vector2f(0,0),
                                                   sf::Vector2f(anBoundingBox.GetWidth(),anBoundingBox.GetHeight()),
                                                   sf::Color(0,0,0));
          anShape.SetPosition(anBoundingBox.Left+anPosition.x,anBoundingBox.Top+anPosition.y);
          mApp.mWindow.Draw(anShape);
#else
          sf::RectangleShape anShape(sf::Vector2f(anBoundingBox.width,anBoundingBox.height));
          anShape.setPosition(anPosition.x,anPosition.y);
					//anShape.setOrigin(anOrigin);
					anShape.setScale(anScale);
          mApp.mWindow.draw(anShape);
#endif
        } //if(anEntity->mProperties.Get<bool>("bFixedMovement") == false)
      } // while(anQueue != anIter->second.end())

      // Increment map iterator
      anIter++;
    } //while(anIter != mEntities.end())
  }

  void ICollisionSystem::HandleCleanup(IEntity* theEntity)
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
