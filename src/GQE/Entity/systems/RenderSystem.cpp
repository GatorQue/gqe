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
		theEntity->mProperties.Add<GQE::Uint8>("RenderFlags",RENDER_SPRITE);
    theEntity->mProperties.Add<sf::Sprite>("Sprite",sf::Sprite());
		theEntity->mProperties.Add<sf::RectangleShape>("RectangleShape",sf::RectangleShape());
		theEntity->mProperties.Add<sf::VertexArray>("VertexArray",sf::VertexArray());
		theEntity->mProperties.Add<sf::IntRect>("rSpriteRect",sf::IntRect(0,0,0,0));
    theEntity->mProperties.Add<sf::Vector2f>("vScale",sf::Vector2f(1,1));
    theEntity->mProperties.Add<sf::Vector2f>("vOrigin",sf::Vector2f(0,0));
    theEntity->mProperties.Add<sf::Vector2f>("vPosition",sf::Vector2f(0,0));
    theEntity->mProperties.Add<sf::Color>("cColor",sf::Color(255,255,255,255));
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
		//This code renders offscreen entitys invisible.
		if(theEntity!=NULL)
		{
			const sf::View& anCurrentView=mApp.mWindow.getView();
			sf::Vector2f anViewCenter=anCurrentView.getCenter();
			sf::Vector2f anViewSize=anCurrentView.getSize();
			sf::FloatRect anViewRect(anViewCenter,anViewSize);
			// See if this IEntity is visible, if so draw it now
			if(theEntity->mProperties.Get<bool>("bVisible"))
			{
				GQE::Uint8 anRenderFlags=theEntity->mProperties.Get<GQE::Uint8>("RenderFlags");
				// Get the other RenderSystem properties now
				sf::Sprite anSprite=theEntity->mProperties.Get<sf::Sprite>("Sprite");
				sf::RectangleShape anShape=theEntity->mProperties.Get<sf::RectangleShape>("RectangleShape");
				sf::VertexArray anVertexArray=theEntity->mProperties.Get<sf::VertexArray>("VertexArray");
				anSprite.setPosition(theEntity->mProperties.Get<sf::Vector2f>("vPosition"));
				anSprite.setRotation(theEntity->mProperties.Get<float>("fRotation"));
				anSprite.setScale(theEntity->mProperties.Get<sf::Vector2f>("vScale"));
				anSprite.setColor(theEntity->mProperties.Get<sf::Color>("cColor"));
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
				if(anViewRect.intersects(anSprite.getGlobalBounds()))
				{
					if(anRenderFlags=anRenderFlags & RENDER_SPRITE)
					{
						mApp.mWindow.draw(anSprite);
					}
					if(anRenderFlags=anRenderFlags & RENDER_RECTANGLE && anShape.getSize().x!=0 && anShape.getSize().y!=0)
					{
						anShape.setPosition(anSprite.getPosition());
						anShape.setRotation(anSprite.getRotation());
						anShape.setOrigin(anSprite.getOrigin());
						mApp.mWindow.draw(anShape);
					}

					if(anRenderFlags=anRenderFlags & RENDER_VERTEX_ARRAY && anVertexArray.getVertexCount()>0)
					{
						sf::RenderStates anState(anSprite.getTexture());
						mApp.mWindow.draw(anVertexArray,anState);
					}
				}
			} // if(theEntity->mProperties.Get<bool>("bVisible"))
		}
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
