/**
 * Provides the MovementSystem class for handing all entity movement in a game.
 *
 * @file src/GQE/Entity/systems/MovementSystem.cpp
 * @author Jacob Dix
 * @date 20120611 - Initial Release
 * @date 20120616 - Adjustments for new PropertyManager class
 * @date 20120618 - Use IEntity not Instance and changed AddPrototype to AddProperties
 */
#include <SFML/Graphics.hpp>
#include <GQE/Entity/systems/MovementSystem.hpp>
#include <GQE/Core/assets/ImageAsset.hpp>
#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/classes/Instance.hpp>

namespace GQE
{
	MovementSystem::MovementSystem(IApp& theApp):
		ISystem("MovementSystem",theApp)
	{
	}
	MovementSystem::~MovementSystem()
	{
	}

  void MovementSystem::AddProperties(IEntity* theEntity)
	{
		theEntity->mProperties.Add<sf::Vector2f>("Velocity",sf::Vector2f(0,0));
		theEntity->mProperties.Add<sf::Vector2f>("Accelleration",sf::Vector2f(0,0));
		theEntity->mProperties.Add<float>("RotationalVelocity",0);
		theEntity->mProperties.Add<float>("RotationalAccelleration",0);
		theEntity->mProperties.Add<bool>("Wrap",true);
		theEntity->AddSystem(this);
	}
	
  void MovementSystem::HandleEvents(sf::Event theEvent)
	{
	}

  void MovementSystem::UpdateFixed()
	{
		std::vector<IEntity*>::iterator anEntityIter;
		for(anEntityIter=mEntities.begin();
			anEntityIter!=mEntities.end();
			++anEntityIter)
		{
			IEntity* anEntity = (*anEntityIter);
			sf::Vector2f anPosition=anEntity->mProperties.Get<sf::Vector2f>("Position");
			sf::Vector2f anVelocity=anEntity->mProperties.Get<sf::Vector2f>("Velocity");
			sf::Vector2f anAccelleration=anEntity->mProperties.Get<sf::Vector2f>("Accelleration");
			float anRotation=anEntity->mProperties.Get<float>("Rotation");
			float anRotationalVelocity=anEntity->mProperties.Get<float>("RotationalVelocity");
			float anRotationalAccelleration=anEntity->mProperties.Get<float>("RotationalAccelleration");
			anVelocity+=anAccelleration;
			anPosition+=anVelocity;
			anRotationalVelocity+=anRotationalAccelleration;
			anRotation+=anRotationalVelocity;
			if(anInstance->mProperties.Get<bool>("Wrap"))
			{
				if(anPosition.x>mApp.mWindow.getSize().x)
				{
					anPosition.x=0;
				}
				else if(anPosition.x<0)
				{
					anPosition.x=(float)mApp.mWindow.getSize().x;
				}
				if(anPosition.y>mApp.mWindow.getSize().y)
				{
					anPosition.y=0;
				}
				else if(anPosition.y<0)
				{
					anPosition.y=(float)mApp.mWindow.getSize().y;
				}
			}
			anEntity->mProperties.Set<sf::Vector2f>("Velocity",anVelocity);
			anEntity->mProperties.Set<sf::Vector2f>("Position",anPosition);
			anEntity->mProperties.Set<float>("Rotation",anRotation);
			anEntity->mProperties.Set<float>("RotationalVelocity",anRotationalVelocity);
		}
	}
	
  void MovementSystem::UpdateVariable(float theElaspedTime)
	{
  }

	void MovementSystem::Draw()
	{
	}

  void MovementSystem::HandleCleanup()
	{
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
