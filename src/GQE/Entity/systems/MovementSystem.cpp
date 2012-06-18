/**
 * Provides the MovementSystem class for handing all entity movement in a game.
 *
 * @file src/GQE/Entity/systems/MovementSystem.cpp
 * @author Jacob Dix
 * @date 20120611 - Initial Release
 * @date 20120616 - Adjustments for new PropertyManager class
 */
#include <GQE/Entity/systems/MovementSystem.hpp>
#include <GQE/Core/assets/ImageAsset.hpp>
#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/classes/Instance.hpp>
#include <SFML/Graphics.hpp>

namespace GQE
{
	MovementSystem::MovementSystem(IApp& theApp):
		ISystem("MovementSystem",theApp)
	{
		
	}
	MovementSystem::~MovementSystem()
	{

	}
	void MovementSystem::RegisterPrototype(Prototype* thePrototype)
	{
		thePrototype->mProperties.Add<sf::Vector2f>("Velocity",sf::Vector2f(0,0));
		thePrototype->mProperties.Add<sf::Vector2f>("Accelleration",sf::Vector2f(0,0));
		thePrototype->mProperties.Add<float>("RotationalVelocity",0);
		thePrototype->mProperties.Add<float>("RotationalAccelleration",0);
		thePrototype->mProperties.Add<bool>("Wrap",false);
		thePrototype->AddSystem(this);
		ISystem::RegisterPrototype(thePrototype);
	}
	
  void MovementSystem::InitInstance(Instance* theInstance)
	{
	}
	
  void MovementSystem::HandleEvents(sf::Event theEvent)
	{
	}

  void MovementSystem::UpdateFixed()
	{
		Instance* anInstance=NULL;
		std::vector<Instance*>::iterator anInstanceIter;
		sf::Vector2f anPosition;
		sf::Vector2f anVelocity;
		sf::Vector2f anAccelleration;
		float anRotation;
		float anRotationalVelocity;
		float anRotationalAccelleration;

		for(anInstanceIter=mInstanceList.begin();
			anInstanceIter!=mInstanceList.end();
			++anInstanceIter)
		{
			anInstance=(*anInstanceIter);
			anPosition=anInstance->mProperties.Get<sf::Vector2f>("Position");
			anVelocity=anInstance->mProperties.Get<sf::Vector2f>("Velocity");
			anAccelleration=anInstance->mProperties.Get<sf::Vector2f>("Accelleration");
			anRotation=anInstance->mProperties.Get<float>("Rotation");
			anRotationalVelocity=anInstance->mProperties.Get<float>("RotationalVelocity");
			anRotationalAccelleration=anInstance->mProperties.Get<float>("RotationalAccelleration");
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
			anInstance->mProperties.Set<sf::Vector2f>("Velocity",anVelocity);
			anInstance->mProperties.Set<sf::Vector2f>("Position",anPosition);
			anInstance->mProperties.Set<float>("Rotation",anRotation);
			anInstance->mProperties.Set<float>("RotationalVelocity",anRotationalVelocity);
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
