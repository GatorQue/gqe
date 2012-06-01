/**
 * Provides a solid component to be added to an IEntity class which is used to
 * ???.
 *
 * @file src/GQE/Entity/Components/RenderComponent.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#include <GQE/Entity/Components/SolidComponent.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  std::vector<IEntity*> SolidComponent::mEntities;

  SolidComponent::SolidComponent(IApp& theApp) :
    IComponent("SolidComponent", theApp)
  {

  }

  SolidComponent::~SolidComponent()
  {

  }

  void SolidComponent::DoInit(IEntity* theEntity)
  {
    IComponent::DoInit(theEntity);
    mEntity->AddProperty<sf::FloatRect>("BoundingBox",sf::FloatRect(0,0,0,0));
    mEntities.push_back(theEntity);
  }

  void SolidComponent::ReInit()
  {

  }

  void SolidComponent::HandleEvents(sf::Event theEvent)
  {

  }

  void SolidComponent::UpdateFixed()
  {
    std::vector<IEntity*>::iterator it;
    std::vector<IEntity*>::iterator it2;
    //IEntity* anEntity;
    //IEntity* anOtherEntity;
    for(it=mEntities.begin();it!=mEntities.end();++it)
    {
      for(it2=it+1;it2!=mEntities.end();++it2)
      {
        sf::FloatRect anRectA=(*it)->GetProperty<sf::FloatRect>("BoundingBox");
        sf::Vector2f anPosA=(*it)->GetProperty<sf::Vector2f>("Position");
        sf::FloatRect anRectB=(*it2)->GetProperty<sf::FloatRect>("BoundingBox");
        sf::Vector2f anPosB=(*it2)->GetProperty<sf::Vector2f>("Position");
      }
    }
  }

  void SolidComponent::UpdateVariable(float theElapstedTime)
  {
  }

  void SolidComponent::Draw()
  {
  }

  void SolidComponent::Cleanup(void)
  {
    IComponent::Cleanup();
  }

  IComponent* SolidComponent::MakeClone()
  {
    return (new(std::nothrow) SolidComponent(mApp));
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
