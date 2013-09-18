/**
 * Provides the AnimationSystem class for handing all IEntity animation in a game.
 *
 * @file include/GQE/Entity/systems/AnimationSystem.cpp
 * @author Jacob Dix
 * @date 20120623 - Initial Release
 * @date 20120904 - Fix SFML v1.6 issues with Vector2u
 * @date 20120910 - Fix SFML v1.6 issue with frame edge check
 */

#include <SFML/System.hpp>
#include <GQE/Entity/systems/AnimationSystem.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  AnimationSystem::AnimationSystem(GQE::IApp& theApp) :
    ISystem("AnimationSystem",theApp)
  {
  }

  AnimationSystem::~AnimationSystem()
  {
  }

  void AnimationSystem::AddProperties(IEntity* theEntity)
  {
		theEntity->mProperties.Add<std::string>("CurrentAnimation","DefaultAnimation");
		theEntity->mProperties.Add<std::vector<sf::IntRect>>("DefaultAnimation",std::vector<sf::IntRect>());
		theEntity->mProperties.Add<sf::Clock>("AnimationClock",sf::Clock());
		theEntity->mProperties.Add<float>("FramesPerSecond",12);
		theEntity->mProperties.Add<GQE::Int32>("CurrentFrame",0);
	}

  void AnimationSystem::HandleInit(GQE::IEntity* theEntity)
  {
  }

  void AnimationSystem::HandleEvents(sf::Event theEvent)
  {
  }

  void AnimationSystem::EntityUpdateFixed(IEntity* theEntity)
  {
		//Variables
		sf::IntRect anTextureRect;
				
    // Get the AnimationSystem properties
		sf::Clock anClock=theEntity->mProperties.Get<sf::Clock>("AnimationClock");
		float anFPS=theEntity->mProperties.GetFloat("FramesPerSecond");
		GQE::Int32 anCurrentFrame=theEntity->mProperties.GetInt32("CurrentFrame");
		std::string anCurrentAnimation=theEntity->mProperties.GetString("CurrentAnimation");
		if(anClock.getElapsedTime().asMilliseconds()>1/anFPS)
		{
			anClock.restart();
			anCurrentFrame++;
			std::vector<sf::IntRect> anAnimation= theEntity->mProperties.Get<std::vector<sf::IntRect> >(anCurrentAnimation);
			if(anAnimation.size()>0)
			{
				if(anCurrentFrame>=anAnimation.size())
				{
					anCurrentFrame=0;
				}
				anTextureRect=anAnimation[anCurrentFrame];
				theEntity->mProperties.Set<sf::IntRect>("rTextureRect",anTextureRect);
				theEntity->mProperties.Set<sf::Clock>("AnimationClock",anClock);
				theEntity->mProperties.SetInt32("CurrentFrame",anCurrentFrame);
			}
		}
	}
  void AnimationSystem::UpdateVariable(float theElapsedTime)
  {
  }

  void AnimationSystem::Draw()
  {
  }

  void AnimationSystem::HandleCleanup(GQE::IEntity* theEntity)
  {
  }
}

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

