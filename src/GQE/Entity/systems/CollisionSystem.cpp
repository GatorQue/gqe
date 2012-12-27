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
  theEntity->mProperties.Add<typeCollisionData>("CollisionData",typeCollisionData());
}

void CollisionSystem::HandleInit(IEntity* theEntity)
{
  if(theEntity->mProperties.Get<bool>("bMovable"))
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
        if(anEntity!=anMovableEntity)
        {
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
          typeCollisionData anData;
          anData.DistanceAbove=(anPositionB.y + anRectB.height - anPositionA.y);
          anData.DistanceBelow=(anPositionA.y + anRectA.height - anPositionB.y);
          anData.DistanceLeft=(anPositionB.x + anRectB.width - anPositionA.x);
          anData.DistanceRight=(anPositionA.x + anRectA.width - anPositionB.x);
          anData=CheckCollisionSide(anRectA,anRectB,anData);
          if(anData.Side!=COL_NONE)
  #endif
          {
            anMovableEntity->mProperties.Set<typeCollisionData>("CollisionData",anData);
            anEntity->mProperties.Set<typeCollisionData>("CollisionData",anData.Inverce());
            EntityCollision(anMovableEntity,anEntity);
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
typeCollisionData CollisionSystem::CheckCollisionSide(sf::IntRect theRectA, sf::IntRect theRectB, typeCollisionData theCollisionData) 
{
  typeCollisionData anData=theCollisionData;
  sf::IntRect anIntersectRect;
  bool anResult;
#if SFML_VERSION_MAJOR < 2
  anResult=theRectA.Intersects(theRectB,anIntersectRect);
#else
  anResult=theRectA.intersects(theRectB,anIntersectRect);
#endif
  if(!anResult)
  {
    anData.Side=COL_NONE;
  }
  //check each side, and return only if true
  else if (CheckCollisionAbove(theRectA, theRectB,theCollisionData)!=COL_NONE)
    anData.Side=COL_TOP;
  else if (CheckCollisionBelow(theRectA, theRectB,theCollisionData)!=COL_NONE)
    anData.Side=COL_BOTTOM;
  else if (CheckCollisionLeft(theRectA, theRectB,theCollisionData)!=COL_NONE)
    anData.Side=COL_LEFT;
  else if (CheckCollisionRight(theRectA, theRectB,theCollisionData)!=COL_NONE)
    anData.Side=COL_RIGHT;
  anData.IntersectRect=anIntersectRect;
  return anData;
}

GQE::Uint8 CollisionSystem::CheckCollisionAbove(sf::IntRect theRectA, sf::IntRect theRectB, typeCollisionData theCollisionData) {
#if SFML_VERSION_MAJOR < 2
  if (!theRectA.Intersects(theRectB))
#else
  if (!theRectA.intersects(theRectB))
#endif
  {
    return COL_NONE;
  }

  if (theCollisionData.DistanceAbove <= theCollisionData.DistanceBelow && theCollisionData.DistanceAbove <= std::min(theCollisionData.DistanceLeft, theCollisionData.DistanceRight))
    return COL_TOP;

  return COL_NONE;
}

GQE::Uint8 CollisionSystem::CheckCollisionBelow(sf::IntRect theRectA, sf::IntRect theRectB, typeCollisionData theCollisionData) {
#if SFML_VERSION_MAJOR < 2
  if (!theRectA.Intersects(theRectB))
#else
  if (!theRectA.intersects(theRectB))
#endif
  {
    return COL_NONE;
  }

  if (theCollisionData.DistanceBelow <= theCollisionData.DistanceAbove && theCollisionData.DistanceBelow <= std::min(theCollisionData.DistanceLeft, theCollisionData.DistanceRight))
    return COL_BOTTOM;

  return COL_NONE;
}

GQE::Uint8 CollisionSystem::CheckCollisionLeft(sf::IntRect theRectA, sf::IntRect theRectB, typeCollisionData theCollisionData) {
#if SFML_VERSION_MAJOR < 2
  if (!theRectA.Intersects(theRectB))
#else
  if (!theRectA.intersects(theRectB))
#endif
  {
    return COL_NONE;
  }

  if (theCollisionData.DistanceLeft<= theCollisionData.DistanceRight && theCollisionData.DistanceLeft <= std::min(theCollisionData.DistanceAbove, theCollisionData.DistanceBelow))
    return COL_LEFT;

  return COL_NONE;
}

GQE::Uint8 CollisionSystem::CheckCollisionRight(sf::IntRect theRectA, sf::IntRect theRectB, typeCollisionData theCollisionData) {
#if SFML_VERSION_MAJOR < 2
  if (!theRectA.Intersects(theRectB))
#else
  if (!theRectA.intersects(theRectB))
#endif
  {
    return COL_NONE;
  }

  if (theCollisionData.DistanceRight <= theCollisionData.DistanceLeft && theCollisionData.DistanceRight <= std::min(theCollisionData.DistanceAbove, theCollisionData.DistanceBelow))
    return COL_RIGHT;

  return COL_NONE;
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
