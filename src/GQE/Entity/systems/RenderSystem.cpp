/**
 * Provides the RenderSystem class for handing all entity rendering in a game.
 *
 * @file src/GQE/Entity/systems/RenderSystem.cpp
 * @author Jacob Dix
 * @date 20120611 - Initial Release
 * @date 20120616 - Adjustments for new PropertyManager class
 * @date 20120622 - Small adjustments to implementation and Handle methods
 * @date 20120623 - Improved documentation and adjusted some properties
 * @date 20130202 - Fix SFML v1.6 compiler issues
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

  void RenderSystem::EntityHandleEvents(IEntity* theEntity,sf::Event theEvent)
  {
  }

  void RenderSystem::EntityUpdateFixed(IEntity* theEntity)
  {
  }

  void RenderSystem::EntityUpdateVariable(IEntity* theEntity,float theElapsedTime)
  {
  }

  void RenderSystem::EntityDraw(IEntity* theEntity)
  {
    // See if this IEntity is visible, if so draw it now
    if(theEntity->mProperties.Get<bool>("bVisible"))
    {
      // Get the other RenderSystem properties now
      sf::Sprite anSprite=theEntity->mProperties.Get<sf::Sprite>("Sprite");
      sf::Shape* anShape=theEntity->mProperties.Get<sf::Shape*>("Shape");
#if SFML_VERSION_MAJOR<2
      anSprite.SetPosition(theEntity->mProperties.Get<sf::Vector2f>("vPosition"));
      anSprite.SetRotation(theEntity->mProperties.Get<float>("fRotation"));
      sf::IntRect anRect=theEntity->mProperties.Get<sf::IntRect>("rSpriteRect");
      if(anRect.GetWidth()==0)
      {
        anRect.Right=anRect.Left+anSprite.GetImage()->GetWidth();
      }
      if(anRect.GetHeight()==0)
      {
        anRect.Bottom=anRect.Top+anSprite.GetImage()->GetHeight();
      }
      anSprite.SetSubRect(anRect);
      anSprite.SetCenter(theEntity->mProperties.Get<sf::Vector2f>("vOrigin"));
      mApp.mWindow.Draw(anSprite);
      if(anShape!=NULL)
      {
        anShape->SetPosition(anSprite.GetPosition());
        anShape->SetRotation(anSprite.GetRotation());
        anShape->SetCenter(anSprite.GetCenter());
        mApp.mWindow.Draw(*anShape);
      }
#else
      anSprite.setPosition(theEntity->mProperties.Get<sf::Vector2f>("vPosition"));
      anSprite.setRotation(theEntity->mProperties.Get<float>("fRotation"));
      sf::IntRect anRect=theEntity->mProperties.Get<sf::IntRect>("rSpriteRect");
      if(anRect.width==0)
      {
        const sf::Texture* anTexture=anSprite.getTexture();
        if(anTexture!=NULL)
        {
          anRect.width=anTexture->getSize().x;
        }
      }
      if(anRect.height==0)
      {
        const sf::Texture* anTexture=anSprite.getTexture();
        if(anTexture!=NULL)
        {
          anRect.height=anTexture->getSize().y;
        }
      }
      anSprite.setTextureRect(anRect);
      anSprite.setOrigin(theEntity->mProperties.Get<sf::Vector2f>("vOrigin"));
      mApp.mWindow.draw(anSprite);
      if(anShape!=NULL)
      {
        anShape->setPosition(anSprite.getPosition());
        anShape->setRotation(anSprite.getRotation());
        anShape->setOrigin(anSprite.getOrigin());
        mApp.mWindow.draw(*anShape);
      }
#endif
    } // if(theEntity->mProperties.Get<bool>("bVisible"))
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
