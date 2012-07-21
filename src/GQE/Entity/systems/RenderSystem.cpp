/**
 * Provides the RenderSystem class for handing all entity rendering in a game.
 *
 * @file src/GQE/Entity/systems/RenderSystem.cpp
 * @author Jacob Dix
 * @date 20120611 - Initial Release
 * @date 20120616 - Adjustments for new PropertyManager class
 * @date 20120622 - Small adjustments to implementation and Handle methods
 * @date 20120623 - Improved documentation and adjusted some properties
 */
#include <SFML/Graphics.hpp>
#include <GQE/Entity/systems/RenderSystem.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

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
    theEntity->mProperties.Add<sf::Sprite>("Sprite",sf::Sprite());
    theEntity->mProperties.Add<sf::IntRect>("SpriteRect",sf::IntRect(0,0,0,0));
    theEntity->mProperties.Add<sf::Vector2f>("Scale",sf::Vector2f(1,1));
    theEntity->mProperties.Add<sf::Vector2f>("Origin",sf::Vector2f(0,0));
    theEntity->mProperties.Add<sf::Vector2f>("Position",sf::Vector2f(0,0));
    theEntity->mProperties.Add<float>("Rotation", 0.0f);
    theEntity->mProperties.Add<bool>("Visible", true);
  }

  void RenderSystem::HandleInit(IEntity* theEntity)
  {
    // Do nothing
  }

  void RenderSystem::HandleEvents(sf::Event theEvent)
  {
  }

  void RenderSystem::UpdateFixed()
  {
  }

  void RenderSystem::UpdateVariable(float theElapsedTime)
  {
  }

  void RenderSystem::Draw()
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

        // See if this IEntity is visible, if so draw it now
        if(anEntity->mProperties.Get<bool>("Visible"))
        {
          // Get the other RenderSystem properties now
          sf::Sprite anSprite=anEntity->mProperties.Get<sf::Sprite>("Sprite");
#if SFML_VERSION_MAJOR<2
          anSprite.SetPosition(anEntity->mProperties.Get<sf::Vector2f>("Position"));
          anSprite.SetRotation(anEntity->mProperties.Get<float>("Rotation"));
          anSprite.SetSubRect(anEntity->mProperties.Get<sf::IntRect>("SpriteRect"));
          anSprite.SetCenter(anEntity->mProperties.Get<sf::Vector2f>("Origin"));
          mApp.mWindow.Draw(anSprite);
#else
          anSprite.setPosition(anEntity->mProperties.Get<sf::Vector2f>("Position"));
          anSprite.setRotation(anEntity->mProperties.Get<float>("Rotation"));
          anSprite.setTextureRect(anEntity->mProperties.Get<sf::IntRect>("SpriteRect"));
          anSprite.setOrigin(anEntity->mProperties.Get<sf::Vector2f>("Origin"));
          mApp.mWindow.draw(anSprite);
#endif
        } // if(anEntity->mProperties.Get<bool>("Visible"))
      } // while(anQueue != anIter->second.end())

      // Increment map iterator
      anIter++;
    } //while(anIter != mEntities.end())
  }

  void RenderSystem::HandleCleanup(IEntity* theEntity)
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
