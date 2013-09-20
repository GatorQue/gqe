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
		theEntity->mProperties.Add<std::string>("sCurrentAnimation","DefaultAnimation");
		theEntity->mProperties.Add<typeRectList>("DefaultAnimation",typeRectList());
		theEntity->mProperties.Add<sf::Clock>("AnimationClock",sf::Clock());
		theEntity->mProperties.Add<Uint32>("uFramesPerSecond",6);
		theEntity->mProperties.Add<Uint32>("uCurrentFrame",0);
	}

  void AnimationSystem::HandleInit(IEntity* theEntity)
  {
  }

  void AnimationSystem::HandleEvents(sf::Event theEvent)
  {
  }

  void AnimationSystem::EntityUpdateFixed(IEntity* theEntity)
  {
    // Get the AnimationSystem properties
		sf::Clock anClock=theEntity->mProperties.Get<sf::Clock>("AnimationClock");
		Uint32 anFPS=theEntity->mProperties.GetUint32("uFramesPerSecond");
		Uint32 anCurrentFrame=theEntity->mProperties.GetUint32("uCurrentFrame");
		std::string anCurrentAnimation=theEntity->mProperties.GetString("sCurrentAnimation");
		if(anClock.getElapsedTime().asMilliseconds()>1000/anFPS)
		{
			anClock.restart();
			anCurrentFrame++;
			typeRectList anAnimation= theEntity->mProperties.Get<typeRectList>(anCurrentAnimation);
			if(anAnimation.size()>0)
			{
				if(anCurrentFrame>=anAnimation.size())
				{
					anCurrentFrame=0;
				}
				theEntity->mProperties.Set<sf::Clock>("AnimationClock",anClock);
				theEntity->mProperties.SetUint32("uCurrentFrame",anCurrentFrame);
			}
		}
	}
  void AnimationSystem::UpdateVariable(float theElapsedTime)
  {
  }

  void AnimationSystem::EntityDraw(IEntity* theEntity)
  {
		std::string anAnimationID=theEntity->mProperties.GetString("sCurrentAnimation");
		if(anAnimationID!="")
		{
			typeRectList anAnimation=theEntity->mProperties.Get<typeRectList>(anAnimationID);
			Uint32 anCurrentFrame=theEntity->mProperties.GetUint32("uCurrentFrame");
			if(!anAnimation.empty() && anCurrentFrame<anAnimation.size())
			{
				sf::IntRect anTextureRect=anAnimation[anCurrentFrame];
				theEntity->mProperties.Set<sf::IntRect>("rTextureRect",anTextureRect);
			}
		}

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

