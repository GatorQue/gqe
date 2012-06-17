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

	void RenderSystem::RegisterPrototype(Prototype* thePrototype)
	{
		thePrototype->mProperties.Add<sf::Vector2f>("Position",sf::Vector2f(0,0));
		thePrototype->mProperties.Add<sf::Vector2f>("Scale",sf::Vector2f(1,1));
		thePrototype->mProperties.Add<sf::Vector2f>("Orgin",sf::Vector2f(0,0));
		thePrototype->mProperties.Add<float>("Rotation",0);
		thePrototype->mProperties.Add<sf::Sprite>("Sprite",sf::Sprite());
		thePrototype->mProperties.Add<bool>("Visible",true);
		thePrototype->AddSystem(this);
		ISystem::RegisterPrototype(thePrototype);
	}

  void RenderSystem::InitInstance(Instance* theInstance)
	{
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
		Instance* anInstance=NULL;
		std::vector<Instance*>::iterator anInstanceIter;
		for(anInstanceIter=mInstanceList.begin();
			anInstanceIter!=mInstanceList.end();
			++anInstanceIter)
		{
			anInstance=(*anInstanceIter);
			if(anInstance!=NULL)
			{
					#if SFML_VERSION_MAJOR<2

					#else
					sf::Sprite anSprite=anInstance->mProperties.Get<sf::Sprite>("Sprite");
					anSprite.setPosition(anInstance->mProperties.Get<sf::Vector2f>("Position"));

					anSprite.setRotation(anInstance->GetProperty<float>("Rotation"));
					mApp.mWindow.draw(anSprite);
					#endif
			}
		}	
	}

  void RenderSystem::HandleCleanup()
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
