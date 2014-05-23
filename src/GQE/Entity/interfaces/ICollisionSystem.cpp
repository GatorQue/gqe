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
#include <GQE/Core/utils/MathUtil.hpp>
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
    theEntity->mProperties.Add<sf::ConvexShape>("CollisionShape",sf::ConvexShape());
    theEntity->mProperties.Add<bool>("bDebugDraw",false);
		theEntity->mProperties.Add<sf::Vector2f>("vCollisionOffset",sf::Vector2f(0,0));
		theEntity->mProperties.Add<float>("fCollisionRotation",0.0f);
    theEntity->mProperties.Add<bool>("bSolid",true);
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
			sf::ConvexShape anMovingShape=anMovableEntity->mProperties.Get<sf::ConvexShape>("CollisionShape");
      if (anMovingShape.getPointCount()>0)
      {
        anMovingShape.setPosition(anMovableEntity->mProperties.Get<sf::Vector2f>("vCollisionOffset") + anMovableEntity->mProperties.Get<sf::Vector2f>("vPosition") + anMovableEntity->mProperties.Get<sf::Vector2f>("vVelocity"));
        anMovingShape.setRotation(anMovableEntity->mProperties.GetFloat("fRotation") + anMovableEntity->mProperties.GetFloat("fCollisionRotation"));
        anMovingShape.setScale(anMovableEntity->mProperties.Get<sf::Vector2f>("vScale"));
        sf::Vector2f anOrigin = anMovableEntity->mProperties.Get<sf::Vector2f>("vOrigin");
        anOrigin.x *= anMovingShape.getGlobalBounds().width;
        anOrigin.y *= anMovingShape.getGlobalBounds().height;
        anMovingShape.setOrigin(anOrigin);
        anIter = mEntities.begin();
        while (anIter != mEntities.end())
        {
          std::deque<IEntity*>::iterator anQueue = anIter->second.begin();
          while (anQueue != anIter->second.end())
          {
            // Get the IEntity address first
            GQE::IEntity* anEntity = *anQueue;

            // Increment the IEntity iterator second
            anQueue++;

            EntityUpdateFixed(anEntity);

            sf::ConvexShape anOtherShape = anEntity->mProperties.Get<sf::ConvexShape>("CollisionShape");
            if (anOtherShape.getPointCount()>0)
            {
              anOtherShape.setPosition(anEntity->mProperties.Get<sf::Vector2f>("vCollisionOffset") + anEntity->mProperties.Get<sf::Vector2f>("vPosition") + anEntity->mProperties.Get<sf::Vector2f>("vVelocity"));
              anOtherShape.setRotation(anEntity->mProperties.GetFloat("fRotation") + anEntity->mProperties.GetFloat("fCollisionRotation"));
              anOtherShape.setScale(anEntity->mProperties.Get<sf::Vector2f>("vScale"));
              sf::Vector2f anOrigin = anEntity->mProperties.Get<sf::Vector2f>("vOrigin");
              anOrigin.x *= anOtherShape.getGlobalBounds().width;
              anOrigin.y *= anOtherShape.getGlobalBounds().height;
              anOtherShape.setOrigin(anOrigin);
              CollisionData anData;
              //Make sure we aren't handling two of the same entity.
              if (anEntity != anMovableEntity && anEntity->mProperties.GetBool("bSolid") && anMovableEntity->mProperties.GetBool("bSolid"))
              {
                if (Intersection(anMovingShape, anOtherShape, anData.MinimumTranslation))
                {
                  anData.MovingEntity = anMovableEntity;
                  anData.OtherEntity = anEntity;
                  EntityCollision(anData);
                  anMovingShape.setPosition(anMovableEntity->mProperties.Get<sf::Vector2f>("vPosition"));
                  anMovingShape.setRotation(anMovableEntity->mProperties.Get<float>("fRotation"));
                  anMovingShape.setScale(anMovableEntity->mProperties.Get<sf::Vector2f>("vScale"));
                }
              }
            }
          } // while(anQueue != anIter->second.end())

          // Increment map iterator
          anIter++;

        } //while(anIter != mEntities.end())
        anMovablesIter++;
      }
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
      } // while(anQueue != anIter->second.end())

      // Increment map iterator
      anIter++;
    } //while(anIter != mEntities.end())
  }

  void ICollisionSystem::HandleCleanup(IEntity* theEntity)
  {
    if (theEntity->mProperties.Get<bool>("bMovable"))
    {
      GQE::Uint32 anRemovedIndex = 0;
      bool anFound = false;
      GQE::Uint32 anIndex;
      for (anIndex = 0; anIndex != mMovables.size();++anIndex)
      {
        GQE::IEntity* anEntity = mMovables.at(anIndex);
        if (anEntity->GetID() == theEntity->GetID())
        {
          anFound = true;
          anRemovedIndex = anIndex;
        }
      }
      if(anFound)
        mMovables.erase(mMovables.begin() + anRemovedIndex);
    }
  }
  void ICollisionSystem::ProjectOntoAxis(const sf::Shape& theShape, const sf::Vector2f& theAxis, float& theMin, float& theMax)
	{
    sf::Vector2f anPoint=theShape.getTransform().transformPoint(theShape.getPoint(0));
  	GQE::Uint32 anPointCount=theShape.getPointCount();
		theMin = (anPoint.x*theAxis.x+anPoint.y*theAxis.y);
		theMax = theMin;
		for (int j = 1; j<anPointCount; j++)
		{
			anPoint=theShape.getTransform().transformPoint(theShape.getPoint(j));
			float Projection = (anPoint.x*theAxis.x+anPoint.y*theAxis.y);

			if (Projection<theMin)
				theMin=Projection;
			if (Projection>theMax)
				theMax=Projection;
		}
	}
	bool ICollisionSystem::Intersection(sf::Shape& theMovingShape, sf::Shape& theOtherShape, sf::Vector2f& theMinimumTranslation)
	{
		//Exit if either shape is empty;
		if(theMovingShape.getPointCount()==0 || theOtherShape.getPointCount()==0)
			return false;
		//Variables
		std::vector<sf::Vector2f> anAxes;
		Uint32 anIndex;
		sf::Vector2f anSmallestAxis;
		double anOverlap=std::numeric_limits<double>::max();
		Uint32 anMovingPointCount=theMovingShape.getPointCount();
		Uint32 anOtherPointCount=theOtherShape.getPointCount();
		sf::Vector2f anPointA, anPointB;
		//Axes for this object.
		for(anIndex=0;anIndex<anMovingPointCount-1;++anIndex)
		{
      anPointA=theMovingShape.getPoint(anIndex);
      anPointB=theMovingShape.getPoint(anIndex+1);
      anAxes.push_back(NormalizeVector(sf::Vector2f(anPointB.y - anPointA.y, -(anPointB.x - anPointA.x))));
		}
    anPointA=theMovingShape.getPoint(anMovingPointCount-1);
    anPointB=theMovingShape.getPoint(0);
		anAxes.push_back(NormalizeVector(sf::Vector2f(anPointB.y - anPointA.y, -(anPointB.x - anPointA.x))));
		//Axes for other object.
		for(anIndex=0;anIndex<theOtherShape.getPointCount()-1;++anIndex)
		{
      anPointA=theOtherShape.getPoint(anIndex);
      anPointB=theOtherShape.getPoint(anIndex+1);
      anAxes.push_back(NormalizeVector(sf::Vector2f(anPointB.y - anPointA.y, -(anPointB.x - anPointA.x))));
		}
    anPointA=theOtherShape.getPoint(anOtherPointCount-1);
    anPointB=theOtherShape.getPoint(0);
		anAxes.push_back(NormalizeVector(sf::Vector2f(anPointB.y - anPointA.y, -(anPointB.x - anPointA.x))));
		for(anIndex=0;anIndex<anAxes.size();++anIndex)
		{
			float anMinA, anMaxA, anMinB, anMaxB;
			// ... project the points of both OBBs onto the axis ...
			ProjectOntoAxis(theMovingShape,anAxes[anIndex], anMinA, anMaxA);
			ProjectOntoAxis(theOtherShape,anAxes[anIndex], anMinB, anMaxB);
			// ... and check whether the outermost projected points of both OBBs overlap.
			// If this is not the case, the Seperating Axis Theorem states that there can be no collision between the rectangles
			if(!((anMinB<=anMaxA)&&(anMaxB>=anMinA)))
			{
				return false;
			}
		  double o;
      if(anMinB<=anMaxA)
        o=anMaxA-anMinB;
      else
        o=anMaxB-anMinA;
      if(o<anOverlap)
      {
        anSmallestAxis=anAxes[anIndex];
        anOverlap=o;
      }
		}
		theMinimumTranslation=anSmallestAxis;
    theMinimumTranslation*=(float)anOverlap;
		return true;
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
