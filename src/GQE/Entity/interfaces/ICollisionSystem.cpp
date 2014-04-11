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
#include <GQE/Core/interfaces/IShape.hpp>
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
    theEntity->mProperties.Add<IShape>("CollisionShape",IShape());
    theEntity->mProperties.Add<bool>("bDebugDraw",false);
		theEntity->mProperties.Add<sf::Vector2f>("vCollisionOffset",sf::Vector2f(0,0));
		theEntity->mProperties.Add<float>("fCollisionRotation",0.0f);

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
			IShape anMovingShape=anMovableEntity->mProperties.Get<IShape>("CollisionShape");
			anMovingShape.setPosition(anMovableEntity->mProperties.Get<sf::Vector2f>("vCollisionOffset")+anMovableEntity->mProperties.Get<sf::Vector2f>("vPosition")+anMovableEntity->mProperties.Get<sf::Vector2f>("vVelocity"));
			anMovingShape.setRotation(anMovableEntity->mProperties.GetFloat("fRotation")+anMovableEntity->mProperties.GetFloat("fCollisionRotation"));
			anMovingShape.setScale(anMovableEntity->mProperties.Get<sf::Vector2f>("vScale"));
			sf::Vector2f anOrigin=anMovableEntity->mProperties.Get<sf::Vector2f>("vOrigin");
			anOrigin.x*=anMovingShape.GetSize().x;
			anOrigin.y*=anMovingShape.GetSize().y;
			anMovingShape.setOrigin(anOrigin);
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

					EntityUpdateFixed(anEntity);

					IShape anOtherShape=anEntity->mProperties.Get<IShape>("CollisionShape");
					anOtherShape.setPosition(anEntity->mProperties.Get<sf::Vector2f>("vCollisionOffset")+anEntity->mProperties.Get<sf::Vector2f>("vPosition"));
          anOtherShape.setRotation(anEntity->mProperties.Get<float>("fRotation") + anEntity->mProperties.GetFloat("fCollisionRotation"));
					anOtherShape.setScale(anEntity->mProperties.Get<sf::Vector2f>("vScale"));
          sf::Vector2f anOrigin = anEntity->mProperties.Get<sf::Vector2f>("vOrigin");
          anOrigin.x *= anOtherShape.GetSize().x;
          anOrigin.y *= anOtherShape.GetSize().y;
          anOtherShape.setOrigin(anOrigin);
          //Make sure we aren't handling two of the same entity.
					CollisionContext anContext;
          if(anEntity!=anMovableEntity)
          {
            if (anMovingShape.Intersection(anOtherShape, anContext.MinimumTranslation))
						{
              anContext.MovingEntity = anMovableEntity;
              anContext.OtherEntity = anEntity;
              EntityCollision(anContext);
							anMovingShape.setPosition(anMovableEntity->mProperties.Get<sf::Vector2f>("vPosition"));
							anMovingShape.setRotation(anMovableEntity->mProperties.Get<float>("fRotation"));
							anMovingShape.setScale(anMovableEntity->mProperties.Get<sf::Vector2f>("vScale"));
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
          IShape anShape=anEntity->mProperties.Get<IShape>("CollisionShape");
          sf::ConvexShape anConvexShape = anShape.GetDrawableShape();
          anConvexShape.setFillColor(sf::Color::Green);
          mApp.mWindow.draw(anConvexShape);
        }
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
