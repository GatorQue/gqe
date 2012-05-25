/**
 * Provides a coord component to be added to an IEntity class which is used to
 * keep track of coordinates for this IEntity class.
 *
 * @file src/GQE/Entity/Components/CoordComponent.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#include <GQE/Entity/Components/CoordComponent.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  CoordComponent::CoordComponent(IApp& theApp) :
    IComponent("CoordComponent",theApp)
  {

  }

  CoordComponent::~CoordComponent()
  {

  }

  void CoordComponent::DoInit(IEntity* theEntity)
  {
    IComponent::DoInit(theEntity);
    theEntity->AddProperty<sf::Vector2f>("Position",sf::Vector2f(0,0));
    theEntity->AddProperty<float>("Rotation",0);
    theEntity->AddProperty<sf::Vector2f>("Scale",sf::Vector2f(1,1));
  }

  void CoordComponent::ReInit()
  {

  }

  void CoordComponent::HandleEvents(sf::Event theEvent)
  {

  }

  void CoordComponent::UpdateFixed()
  {

  }

  void CoordComponent::UpdateVariable(float theElapstedTime)
  {

  }

  void CoordComponent::Draw()
  {

  }

  void CoordComponent::Cleanup(void)
  {
    IComponent::Cleanup();
  }

  IComponent* CoordComponent::MakeClone()
  {
    return (new(std::nothrow) CoordComponent(mApp));
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
