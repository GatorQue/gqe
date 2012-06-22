/**
 * Provides the RenderSystem class for handing all entity rendering in a game.
 *
 * @file src/GQE/Entity/systems/RenderSystem.cpp
 * @author Jacob Dix
 * @date 20120611 - Initial Release
 * @date 20120616 - Adjustments for new PropertyManager class
 */
#include <GQE/Entity/systems/RenderSystem.hpp>
#include <GQE/Core/assets/ImageAsset.hpp>
#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/classes/Instance.hpp>
#include <SFML/Graphics.hpp>

namespace GQE
{
	RenderSystem::RenderSystem(IApp& theApp):
		ISystem("RenderSystem",theApp)
	{
	}

  RenderSystem::~RenderSystem()
	{
	}

	void RenderSystem::AddProperties(IEntity* theEntity)
	{
		theEntity->mProperties.Add<sf::Vector2f>("Position",sf::Vector2f(0,0));
		theEntity->mProperties.Add<sf::Vector2f>("Scale",sf::Vector2f(1,1));
		theEntity->mProperties.Add<sf::Vector2f>("Orgin",sf::Vector2f(0,0));
		theEntity->mProperties.Add<float>("Rotation",0);
		theEntity->mProperties.Add<sf::Sprite>("Sprite",sf::Sprite());
		theEntity->mProperties.Add<bool>("Visible",true);
		theEntity->mProperties.Add<sf::IntRect>("SubRect",sf::IntRect(0,0,0,0));
		theEntity->AddSystem(this);
	}
	
	void RenderSystem::HandleInit(void)
	{
		ISystem::HandleInit();
	}

  void RenderSystem::HandleEvents(sf::Event theEvent)
	{
	}

  void RenderSystem::UpdateFixed()
	{
		
	}

  void RenderSystem::UpdateVariable(float theElaspedTime)
	{
	}

  void RenderSystem::Draw()
	{
		IEntity* anEntity=NULL;
		std::vector<IEntity*>::iterator anEntityIter;
		for(anEntityIter=mEntities.begin();
			anEntityIter!=mEntities.end();
			++anEntityIter)
		{
			anEntity=(*anEntityIter);
			if(anEntity->mProperties.Get<bool>("Visible"))
			{
				if(anEntity!=NULL)
				{
					sf::Sprite anSprite=anEntity->mProperties.Get<sf::Sprite>("Sprite");
#if SFML_VERSION_MAJOR<2
					anSprite.SetPosition(anEntity->mProperties.Get<sf::Vector2f>("Position"));
					anSprite.SetRotation(anEntity->mProperties.Get<float>("Rotation"));
					anSprite.SetSubRect(anEntity->mProperties.Get<sf::IntRect>("SubRect"));
					mApp.mWindow.Draw(anSprite);
#else
					anSprite.setPosition(anEntity->mProperties.Get<sf::Vector2f>("Position"));
					anSprite.setRotation(anEntity->mProperties.Get<float>("Rotation"));
					anSprite.setTextureRect(anEntity->mProperties.Get<sf::IntRect>("SubRect"));
					mApp.mWindow.draw(anSprite);
#endif
			}
			}
		}	
	}

  void RenderSystem::HandleCleanup()
	{
		ISystem::HandleCleanup();
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
