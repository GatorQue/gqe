/**
 * Provides a dynamic component to be added to an IEntity class which is used to
 * keep track of dynamic movement variables for this IEntity class.
 *
 * @file src/GQE/Entity/Components/DynamicComponent.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#include <GQE/Entity/Components/DynamicComponent.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  DynamicComponent::DynamicComponent(GQE::IApp& theApp) :
    IComponent("DynamicComponent",theApp)
  {

  }

  DynamicComponent::~DynamicComponent()
  {

  }

  void DynamicComponent::DoInit(IEntity* theEntity)
  {
    IComponent::DoInit(theEntity);
    theEntity->AddProperty<sf::Vector2f>("Velocity",sf::Vector2f(0,0));
    theEntity->AddProperty<sf::Vector2f>("Accelleration",sf::Vector2f(0,0));
    theEntity->AddProperty<float>("RotationVelocity",0);
  }

  void DynamicComponent::ReInit()
  {

  }

  void DynamicComponent::HandleEvents(sf::Event theEvent)
  {

  }

  void DynamicComponent::UpdateFixed()
  {

  }

  void DynamicComponent::UpdateVariable(float theElapstedTime)
  {
		sf::Vector2f anPosition=mEntity->GetProperty<sf::Vector2f>("Position");
		sf::Vector2f anVelocity=mEntity->GetProperty<sf::Vector2f>("Velocity");
		sf::Vector2f anAccelleration=mEntity->GetProperty<sf::Vector2f>("Accelleration");
		anVelocity+=anAccelleration*theElapstedTime;
		anPosition+=anVelocity*theElapstedTime;
		mEntity->SetProperty<sf::Vector2f>("Velocity",anVelocity);
		mEntity->SetProperty<sf::Vector2f>("Position",anPosition);

		float anRotationalVelocity=mEntity->GetProperty<float>("RotationVelocity");
		float anRotation=mEntity->GetProperty<float>("Rotation");
		anRotation+=anRotationalVelocity;
		mEntity->SetProperty<float>("Rotation",anRotation);
  }

  void DynamicComponent::Draw()
  {

  }

  void DynamicComponent::Cleanup(void)
  {
    IComponent::Cleanup();
  }

  GQE::IComponent* DynamicComponent::MakeClone()
  {
    return (new(std::nothrow) DynamicComponent(mApp));
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
