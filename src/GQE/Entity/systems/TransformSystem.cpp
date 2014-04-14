/**
 * Provides the TransformSystem class for handing all entity movement in a game.
 *
 * @file src/GQE/Entity/systems/TransformSystem.cpp
 * @author Jacob Dix
 * @date 20120611 - Initial Release
 * @date 20120616 - Adjustments for new PropertyManager class
 * @date 20120618 - Use IEntity not Instance and changed AddPrototype to AddProperties
 * @date 20120622 - Small adjustments to implementation and Handle methods
 * @date 20120623 - Improved documentation and adjusted some properties
 * @date 20120630 - Improve ScreenWrap functionality using SpriteRect values
 * @date 20130722 - Renamed TransformSystem to TransformSystem. vPosition, fRotation and vScale now properties of TransformSystem.
 * @date 20130928 - changed fDrag to vDrag.
 */
#include <SFML/Graphics.hpp>
#include <GQE/Entity/systems/TransformSystem.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  TransformSystem::TransformSystem(IApp& theApp):
    ISystem("TransformSystem",theApp)
  {
  }
  TransformSystem::~TransformSystem()
  {
  }

  void TransformSystem::AddProperties(IEntity* theEntity)
  {
    theEntity->mProperties.Add<sf::Vector2f>("vScale",sf::Vector2f(1,1));
    theEntity->mProperties.Add<sf::Vector2f>("vPosition",sf::Vector2f(0,0));
    theEntity->mProperties.Add<float>("fRotation", 0.0f);
		theEntity->mProperties.Add<sf::Vector2f>("vVelocity",sf::Vector2f(0,0));
    theEntity->mProperties.Add<sf::Vector2f>("vAcceleration",sf::Vector2f(0,0));
		theEntity->mProperties.Add<sf::Vector2f>("vDrag",sf::Vector2f(0.0f,0.0f));
    theEntity->mProperties.Add<float>("fRotationalVelocity",0);
		theEntity->mProperties.Add<float>("fStopThreshold",.01f);
    theEntity->mProperties.Add<float>("fRotationalAcceleration",0);
    theEntity->mProperties.Add<bool>("bFixedMovement",true);
    theEntity->mProperties.Add<bool>("bScreenWrap",false);
    theEntity->mProperties.Add<bool>("bMovable",true);

  }

  void TransformSystem::HandleInit(IEntity* theEntity)
  {
    // Do nothing
  }
  void TransformSystem::EntityHandleEvents(IEntity* theEntity,sf::Event theEvent)
  {

	}
  void TransformSystem::EntityUpdateFixed(IEntity* theEntity)
  {
		// Are we using fixed movement mathematics?
        if(theEntity->mProperties.Get<bool>("bFixedMovement"))
        {
          // Get the RenderSystem properties
          sf::Vector2f anPosition = theEntity->mProperties.Get<sf::Vector2f>("vPosition");
          float anRotation = theEntity->mProperties.Get<float>("fRotation");

          // Get the TransformSystem properties
          sf::Vector2f anVelocity = theEntity->mProperties.Get<sf::Vector2f>("vVelocity");
          sf::Vector2f anAcceleration = theEntity->mProperties.Get<sf::Vector2f>("vAcceleration");
					sf::Vector2f anDrag=theEntity->mProperties.Get<sf::Vector2f>("vDrag");
					sf::Vector2f anVelocityReduction=sf::Vector2f(1-anDrag.x,1-anDrag.y);
          float anRotationalVelocity = theEntity->mProperties.Get<float>("fRotationalVelocity");
          float anRotationalAcceleration = theEntity->mProperties.Get<float>("fRotationalAcceleration");
					float anStopThreshold=theEntity->mProperties.Get<float>("fStopThreshold");
          // Now update the current movement properties

          anPosition += anVelocity;
          anVelocity += anAcceleration;

					anRotationalVelocity += anRotationalAcceleration;
          anRotation += anRotationalVelocity;
					//Apply Drag
					anVelocity=sf::Vector2f(anVelocity.x*anVelocityReduction.x,anVelocity.y*anVelocityReduction.y);
					if(abs(anVelocity.x)<=anStopThreshold)
					{
						anVelocity.x=0;
					}
					if(abs(anVelocity.y)<=anStopThreshold)
					{
						anVelocity.y=0;
					}
          // If ScreenWrap is true, account for screen wrapping
          if(theEntity->mProperties.Get<bool>("bScreenWrap"))
          {
            // Call our universal HandleScreenWrap method to wrap this IEntity
            HandleScreenWrap(theEntity, &anPosition);
          }

          // Now update the TransformSystem properties for this IEntity class
          theEntity->mProperties.Set<sf::Vector2f>("vVelocity",anVelocity);
          theEntity->mProperties.Set<float>("fRotationalVelocity",anRotationalVelocity);

          // Now update the RenderSystem properties of this IEntity class
          theEntity->mProperties.Set<sf::Vector2f>("vPosition",anPosition);
          theEntity->mProperties.Set<float>("fRotation",anRotation);
					//reset acceleration so its only applyed when needed.
					theEntity->mProperties.Set<sf::Vector2f>("vAcceleration",sf::Vector2f(0,0));
        } //if(theEntity->mProperties.Get<bool>("bFixedMovement"))
  }
  void TransformSystem::EntityUpdateVariable(IEntity* theEntity,float theElapsedTime)
  {
        // Are we NOT using fixed movement mathematics?
        if(theEntity->mProperties.Get<bool>("bFixedMovement") == false)
        {
          // Get the RenderSystem properties
          sf::Vector2f anPosition = theEntity->mProperties.Get<sf::Vector2f>("vPosition");
          float anRotation = theEntity->mProperties.Get<float>("fRotation");

          // Get the TransformSystem properties
          sf::Vector2f anVelocity = theEntity->mProperties.Get<sf::Vector2f>("vVelocity");
          sf::Vector2f anAccelleration = theEntity->mProperties.Get<sf::Vector2f>("vAcceleration");
          float anRotationalVelocity = theEntity->mProperties.Get<float>("fRotationalVelocity");
          float anRotationalAccelleration = theEntity->mProperties.Get<float>("fRotationalAcceleration");

          // Now update the current movement properties
          anVelocity += anAccelleration * theElapsedTime;
          anPosition += anVelocity * theElapsedTime;
          anRotationalVelocity += anRotationalAccelleration * theElapsedTime;
          anRotation += anRotationalVelocity * theElapsedTime;

          // If ScreenWrap is true, account for screen wrapping
          if(theEntity->mProperties.Get<bool>("bScreenWrap"))
          {
            // Call our universal HandleScreenWrap method to wrap this IEntity
            HandleScreenWrap(theEntity, &anPosition);
          }

          // Now update the TransformSystem properties for this IEntity class
          theEntity->mProperties.Set<sf::Vector2f>("vVelocity", anVelocity);
          theEntity->mProperties.Set<float>("fRotationalVelocity", anRotationalVelocity);

          // Now update the RenderSystem properties of this IEntity class
          theEntity->mProperties.Set<sf::Vector2f>("vPosition", anPosition);
          theEntity->mProperties.Set<float>("fRotation", anRotation);
        } //if(theEntity->mProperties.Get<bool>("bFixedMovement") == false)
	}
  void TransformSystem::EntityDraw(IEntity* theEntity)
  {
	}
  void TransformSystem::HandleCleanup(IEntity* theEntity)
  {
    // Do nothing
  }

  void TransformSystem::HandleScreenWrap(IEntity* theEntity, sf::Vector2f* thePosition)
  {
    // Get SpriteRect to see how many pixels to over shoot screen before wrapping
    sf::IntRect anSpriteRect = theEntity->mProperties.Get<sf::IntRect>("rSpriteRect");

#if (SFML_VERSION_MAJOR < 2)
    // If current x is bigger than screen width then wrap to barely showing sprite
    if(thePosition->x > (float)mApp.mWindow.GetWidth())
    {
      thePosition->x = -(float)anSpriteRect.GetWidth();
    }
    // If current x is less than width of sprite then wrap to barley showing sprite
    else if(thePosition->x < -(float)anSpriteRect.GetWidth())
    {
      thePosition->x = (float)mApp.mWindow.GetWidth();
    }
    // If current y is bigger than screen height then wrap to barely showing sprite
    if(thePosition->y > (float)mApp.mWindow.GetHeight())
    {
      thePosition->y = -(float)anSpriteRect.GetHeight();
    }
    // If current y is less than height of sprite then wrap to barley showing sprite
    else if(thePosition->y < -(float)anSpriteRect.GetHeight())
    {
      thePosition->y = (float)mApp.mWindow.GetHeight();
    }
#else
    // If current x is bigger than screen width then wrap to barely showing sprite
    if(thePosition->x > (float)mApp.mWindow.getSize().x)
    {
      thePosition->x = -(float)anSpriteRect.width;
    }
    // If current x is less than width of sprite then wrap to barley showing sprite
    else if(thePosition->x < -(float)anSpriteRect.width)
    {
      thePosition->x = (float)mApp.mWindow.getSize().x;
    }
    // If current y is bigger than screen height then wrap to barely showing sprite
    if(thePosition->y > (float)mApp.mWindow.getSize().y)
    {
      thePosition->y = -(float)anSpriteRect.height;
    }
    // If current y is less than height of sprite then wrap to barley showing sprite
    else if(thePosition->y < -(float)anSpriteRect.height)
    {
      thePosition->y = (float)mApp.mWindow.getSize().y;
    }
#endif
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
