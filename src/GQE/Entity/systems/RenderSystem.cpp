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
 * @date 20130822 - Changed property set up for render system. Sprites are now just vertexarrays with 6 points.
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
		theEntity->mProperties.Add<sf::Texture*>("Texture",NULL);
		theEntity->mProperties.Add<sf::Color>("cColor",sf::Color(255,255,255,255));
		theEntity->mProperties.Add<sf::VertexArray>("VertexArray",sf::VertexArray());
		theEntity->mProperties.Add<sf::IntRect>("rTextureRect",sf::IntRect(0,0,0,0));
    theEntity->mProperties.Add<sf::Vector2f>("vOrigin",sf::Vector2f(0,0));
    theEntity->mProperties.Add<std::string>("sView","");
    theEntity->mProperties.Add<bool>("bVisible", true);
  }

  void RenderSystem::HandleInit(IEntity* theEntity)
  {
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

		if(theEntity!=NULL)
		{
			std::string anCurrentViewID=theEntity->mProperties.GetString("sView");
			const sf::View& anCurrentView=GetView(anCurrentViewID);
      mApp.mWindow.setView(anCurrentView);
			sf::Vector2f anViewSize=anCurrentView.getSize();
			sf::Vector2f anViewPosition=anCurrentView.getCenter();
			sf::FloatRect anViewRect(anViewPosition-sf::Vector2f(anViewSize.x/2,anViewSize.y/2),anViewSize);
			sf::Texture* anTexture=theEntity->mProperties.Get<sf::Texture*>("Texture");
			// See if this IEntity is visible, if so draw it now
			if(theEntity->mProperties.Get<bool>("bVisible") && anTexture!=NULL)
			{
				// Get the other RenderSystem properties now
				sf::Transformable anTransformable;
				sf::RenderStates anRenderStates;
				sf::VertexArray anVertexArray=theEntity->mProperties.Get<sf::VertexArray>("VertexArray");

				sf::Color anColor=theEntity->mProperties.Get<sf::Color>("cColor");
				sf::FloatRect anRect=sf::FloatRect(theEntity->mProperties.Get<sf::IntRect>("rTextureRect"));
				sf::Vector2f anOrigin=theEntity->mProperties.Get<sf::Vector2f>("vOrigin");
				anTransformable.setPosition(theEntity->mProperties.Get<sf::Vector2f>("vPosition"));
				anTransformable.setRotation(theEntity->mProperties.Get<float>("fRotation"));
				anTransformable.setScale(theEntity->mProperties.Get<sf::Vector2f>("vScale"));

				if(anRect.width==0)
				{
					anRect.width=(float)anTexture->getSize().x;
				}
				if(anRect.height==0)
				{
          anRect.height=(float)anTexture->getSize().y;
				}
				//if vertex array is empty. default to a sprite( four verties).
				if(anVertexArray.getVertexCount()==0)
				{
					anVertexArray.setPrimitiveType(sf::TrianglesStrip);
					anVertexArray.append(sf::Vertex(sf::Vector2f(0,0),anColor,sf::Vector2f(anRect.left,anRect.top)));
					anVertexArray.append(sf::Vertex(sf::Vector2f(0,anRect.height),anColor,sf::Vector2f(anRect.left,anRect.top+anRect.height)));
					anVertexArray.append(sf::Vertex(sf::Vector2f(anRect.width,0),anColor,sf::Vector2f(anRect.left+anRect.width,anRect.top)));
					anVertexArray.append(sf::Vertex(sf::Vector2f(anRect.width,anRect.height),anColor,sf::Vector2f(anRect.left+anRect.width,anRect.top+anRect.height)));

				}
				sf::FloatRect anBounds=anVertexArray.getBounds();
				anBounds.left+=anTransformable.getPosition().x;
				anBounds.top+=anTransformable.getPosition().y;
				anTransformable.setOrigin(sf::Vector2f(anOrigin.x*anRect.width,anOrigin.y*anRect.height));
				//This code ensures that offscreen entities will not be renderd.
				if(anViewRect.intersects(anBounds))
				{
					anRenderStates.texture=anTexture;
					anRenderStates.transform=anTransformable.getTransform();
					anRenderStates.shader=NULL;//TODO Add back Shader Support.
					mApp.mWindow.draw(anVertexArray,anRenderStates);
				}
			} // if(theEntity->mProperties.Get<bool>("bVisible"))
		}
  }

  void RenderSystem::HandleCleanup(IEntity* theEntity)
  {
    // Do nothing
  }
  void RenderSystem::SetView(std::string theViewID,sf::View theView)
  {
    mViews[theViewID]=theView;
  }
  sf::View RenderSystem::GetView(std::string theViewID)
  {
    if(mViews.find(theViewID)!=mViews.end())
    {
      return mViews[theViewID];
    }
    return mApp.mWindow.getDefaultView();
  }
	void RenderSystem::EventViewEntity(PropertyManager* theProperties)
	{
		IEntity* anEntity=theProperties->Get<IEntity*>("Entity");
		std::string anViewID=theProperties->GetString("sView");
	  sf::View anView=GetView(anViewID);
		anView.setCenter(anEntity->mProperties.Get<sf::Vector2f>("vPosition"));
		SetView(anViewID,anView);
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
