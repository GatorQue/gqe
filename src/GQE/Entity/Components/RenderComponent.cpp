/**
 * Provides a render component to be added to an IEntity class which is used to
 * render a texture representing the IEntity class to which it is registered.
 *
 * @file src/GQE/Entity/Components/RenderComponent.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#include <GQE/Entity/Components/RenderComponent.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  RenderComponent::RenderComponent(IApp& theApp,
      const typeAssetID theAssetID, AssetLoadTime theLoadTime,
      AssetLoadStyle theLoadStyle, AssetDropTime theDropTime) :
    IComponent("RenderComponent",theApp),
    mImage(theAssetID, theLoadTime, theLoadStyle, theDropTime),
    mSprite(mImage.GetAsset())
  {
  }

  RenderComponent::~RenderComponent()
  {

  }

  void RenderComponent::DoInit(IEntity* theEntity)
  {
    IComponent::DoInit(theEntity);
    theEntity->AddProperty<std::string>("SpriteName", mImage.GetID());
    theEntity->AddProperty<sf::Vector2f>("Origin", sf::Vector2f(0.0f,0.0f));
		theEntity->AddProperty<sf::IntRect>("SubRect",sf::IntRect(0,0,mImage.GetAsset().getSize().x,mImage.GetAsset().getSize().y));
		theEntity->AddProperty<sf::Vector2f>("ImageSize",sf::Vector2f(mImage.GetAsset().getSize().x,mImage.GetAsset().getSize().y));
  }

  void RenderComponent::ReInit()
  {
  }

  void RenderComponent::HandleEvents(sf::Event theEvent)
  {
  }

  void RenderComponent::UpdateFixed()
  {

  }

  void RenderComponent::UpdateVariable(float theElapstedTime)
  {
		sf::Vector2f anPosition=mEntity->GetProperty<sf::Vector2f>("Position");
    float anRotation=mEntity->GetProperty<float>("Rotation");
    sf::Vector2f anScale=mEntity->GetProperty<sf::Vector2f>("Scale");
#if (SFML_VERSION_MAJOR < 2)
    mSprite.SetCenter(mEntity->GetProperty<sf::Vector2f>("Origin"));
    mSprite.SetPosition(anPosition);
    mSprite.SetRotation(anRotation);
    mSprite.SetScale(anScale);
#else
    mSprite.setOrigin(mEntity->GetProperty<sf::Vector2f>("Origin"));
    mSprite.setPosition(anPosition);
    mSprite.setRotation(anRotation);
    mSprite.setScale(anScale);
		mSprite.setTextureRect(mEntity->GetProperty<sf::IntRect>("SubRect"));
#endif
  }

  void RenderComponent::Draw()
  {
#if (SFML_VERSION_MAJOR < 2)
    mApp.mWindow.Draw(mSprite);
#else
   mApp.mWindow.draw(mSprite);
#endif
  }

  void RenderComponent::Cleanup(void)
  {
    IComponent::Cleanup();
  }

  IComponent* RenderComponent::MakeClone()
  {
    return (new(std::nothrow) RenderComponent(mApp,
          mImage.GetID(), mImage.GetLoadTime(), mImage.GetLoadStyle(), mImage.GetDropTime()));
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
