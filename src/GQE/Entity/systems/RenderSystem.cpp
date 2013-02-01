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
    theEntity->mProperties.Add<sf::Shape*>("Shape",NULL);
    theEntity->mProperties.Add<sf::IntRect>("rSpriteRect",sf::IntRect(0,0,0,0));
    theEntity->mProperties.Add<sf::Vector2f>("vScale",sf::Vector2f(1,1));
    theEntity->mProperties.Add<sf::Vector2f>("vOrigin",sf::Vector2f(0,0));
    theEntity->mProperties.Add<sf::Vector2f>("vPosition",sf::Vector2f(0,0));
    theEntity->mProperties.Add<float>("fRotation", 0.0f);
    theEntity->mProperties.Add<bool>("bVisible", true);
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
        if(anEntity->mProperties.Get<bool>("bVisible"))
        {
          // Get the other RenderSystem properties now
          sf::Sprite anSprite=anEntity->mProperties.Get<sf::Sprite>("Sprite");
          sf::Shape* anShape=anEntity->mProperties.Get<sf::Shape*>("Shape");
#if SFML_VERSION_MAJOR<2
          anSprite.SetPosition(anEntity->mProperties.Get<sf::Vector2f>("vPosition"));
          anSprite.SetRotation(anEntity->mProperties.Get<float>("fRotation"));
          sf::IntRect anRect=anEntity->mProperties.Get<sf::IntRect>("rSpriteRect");
          if(anRect.GetWidth()==0)
          {
            anRect.right=anRect.left+anSprite.getTexture()->getSize().x;
          }
          if(anRect.GetHeight()==0)
          {
            anRect.bottom=anRect.top+anSprite.getTexture()->getSize().y;
          }
          anSprite.SetSubRect(anRect);
          anSprite.SetCenter(anEntity->mProperties.Get<sf::Vector2f>("vOrigin"));
          mApp.mWindow.Draw(anSprite);
#else
          anSprite.setPosition(anEntity->mProperties.Get<sf::Vector2f>("vPosition"));
          anSprite.setRotation(anEntity->mProperties.Get<float>("fRotation"));
          sf::IntRect anRect=anEntity->mProperties.Get<sf::IntRect>("rSpriteRect");
          if(anRect.width==0)
          {
            anRect.width=anSprite.getTexture()->getSize().x;
          }
          if(anRect.height==0)
          {
            anRect.height=anSprite.getTexture()->getSize().y;
          }
          anSprite.setTextureRect(anRect);
          anSprite.setOrigin(anEntity->mProperties.Get<sf::Vector2f>("vOrigin"));
          mApp.mWindow.draw(anSprite);
          if(anShape!=NULL)
          {
            anShape->setPosition(anSprite.getPosition());
            anShape->setRotation(anSprite.getRotation());
            anShape->setOrigin(anSprite.getOrigin());
            mApp.mWindow.draw(*anShape);
          }
          
#endif
        } // if(anEntity->mProperties.Get<bool>("bVisible"))
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
