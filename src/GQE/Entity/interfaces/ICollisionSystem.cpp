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
		
			anMovableEntity->mProperties.Set<IShape>("CollisionShape",anMovingShape);
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
					anOtherShape.setRotation(anEntity->mProperties.Get<float>("fRotation"));
					anOtherShape.setScale(anEntity->mProperties.Get<sf::Vector2f>("vScale"));
					anEntity->mProperties.Set<IShape>("CollisionShape",anOtherShape);
          //Make sure we aren't handling two of the same entity.
					typeCollisionData anMovingData;
          if(anEntity!=anMovableEntity)
          {
						if(anMovingShape.Intersection(anOtherShape,anMovingData.MinimumTranslation))
						{
							anMovingData.Collision=true;
							EntityCollision(anMovableEntity,anEntity,anMovingData);
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
#if (SFML_VERSION_MAJOR < 2)
          sf::Shape anShape = sf::Shape::Rectangle(sf::Vector2f(0,0),
                                                   sf::Vector2f(anBoundingBox.GetWidth(),anBoundingBox.GetHeight()),
                                                   sf::Color(0,0,0));
          anShape.SetPosition(anBoundingBox.Left+anPosition.x,anBoundingBox.Top+anPosition.y);
          mApp.mWindow.Draw(anShape);
#else
          sf::RectangleShape anShape(sf::Vector2f((float)anBoundingBox.width,(float)anBoundingBox.height));
					anShape.setPosition(anPosition.x+anBoundingBox.left*anScale.x,anPosition.y+anBoundingBox.top*anScale.y);
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
