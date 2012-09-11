/**
 * Provides the AnimationSystem class for handing all IEntity animation in a game.
 *
 * @file include/GQE/Entity/systems/AnimationSystem.cpp
 * @author Jacob Dix
 * @date 20120623 - Initial Release
 * @date 20120904 - Fix SFML v1.6 issues with Vector2u
 * @date 20120910 - Fix SFML v1.6 issue with frame edge check
 */

#include <SFML/System.hpp>
#include <GQE/Entity/systems/AnimationSystem.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

// Define Vector2u here since it was not defined for SFML 1.6
#if (SFML_VERSION_MAJOR < 2)
namespace sf
{
  typedef Vector2<unsigned int> Vector2u;
}
#endif

namespace GQE
{
  AnimationSystem::AnimationSystem(GQE::IApp& theApp) :
    ISystem("AnimationSystem",theApp)
  {
  }

  AnimationSystem::~AnimationSystem()
  {
  }

  void AnimationSystem::AddProperties(IEntity* theEntity)
  {
    theEntity->mProperties.Add<sf::Clock>("FrameClock",sf::Clock());	
    theEntity->mProperties.Add<float>("fFrameDelay",0.0f);
#if (SFML_VERSION_MAJOR < 2)
    theEntity->mProperties.Add<sf::Vector2u>("wFrameModifier",sf::Vector2u(0,0));	
#else
    theEntity->mProperties.Add<sf::Vector2u>("wFrameModifier",sf::Vector2u(0,0));
#endif
    theEntity->mProperties.Add<sf::IntRect>("rFrameRect",sf::IntRect(0,0,0,0));
  }

  void AnimationSystem::HandleInit(GQE::IEntity* theEntity)
  {
  }

  void AnimationSystem::HandleEvents(sf::Event theEvent)
  {
  }

  void AnimationSystem::UpdateFixed()
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

        // Get the AnimationSystem properties
        sf::Clock anFrameClock = anEntity->mProperties.Get<sf::Clock>("FrameClock");
        float anFrameDelay = anEntity->mProperties.Get<float>("fFrameDelay");

        // Is it time to update to the next frame?
#if (SFML_VERSION_MAJOR < 2)
        if(anFrameClock.GetElapsedTime() > anFrameDelay)
        {
          // Get the RenderSystem properties
          sf::IntRect anSpriteRect = anEntity->mProperties.Get<sf::IntRect>("rSpriteRect");

          // Get some additional AnimationSystem properties
          sf::Vector2u anFrameModifier = anEntity->mProperties.Get<sf::Vector2u>("wFrameModifier");
          sf::IntRect anFrameRect = anEntity->mProperties.Get<sf::IntRect>("rFrameRect");

          // Are we using a horizontal row of animation images?
          if(anFrameModifier.x > 0)
          {
            anSpriteRect.Offset(anSpriteRect.GetWidth()*anFrameModifier.x, 0);
            if(anSpriteRect.Left >= anFrameRect.Left+anFrameRect.GetWidth())
            {
              // Do Right first since GetWidth will change size if Left is done first
              anSpriteRect.Right = anFrameRect.Left + anSpriteRect.GetWidth();
              anSpriteRect.Left = anFrameRect.Left;
            }
          }
          // Are we using a vertical row of animation images?
          if(anFrameModifier.y > 0)
          {
            anSpriteRect.Offset(0,anSpriteRect.GetHeight()*anFrameModifier.y);
            if(anSpriteRect.Top >= anFrameRect.Top+anFrameRect.GetHeight())
            {
              // Do Bottom first since GetHeight will change size if Top is done first
              anSpriteRect.Bottom = anFrameRect.Top + anSpriteRect.GetHeight();
              anSpriteRect.Top = anFrameRect.Top;
            }
          }

          // Restart our animation frame clock
          anFrameClock.Reset();

          // Now update our AnimationSystem FrameClock property
          anEntity->mProperties.Set<sf::Clock>("FrameClock",anFrameClock);

          // Update our RenderSystem SpriteRect property
          anEntity->mProperties.Set<sf::IntRect>("rSpriteRect",anSpriteRect);
        } // if(anFrameClock > anFrameDelay)
#else
        if(anFrameClock.getElapsedTime().asSeconds() > anFrameDelay)
        {
          // Get the RenderSystem properties
          sf::IntRect anSpriteRect = anEntity->mProperties.Get<sf::IntRect>("rSpriteRect");

          // Get some additional AnimationSystem properties
          sf::Vector2u anFrameModifier = anEntity->mProperties.Get<sf::Vector2u>("wFrameModifier");
          sf::IntRect anFrameRect = anEntity->mProperties.Get<sf::IntRect>("rFrameRect");

          // Are we using a horizontal row of animation images?
          if(anFrameModifier.x > 0)
          {
            anSpriteRect.left += anSpriteRect.width*anFrameModifier.x;
            if(anSpriteRect.left >= anFrameRect.left+anFrameRect.width)
            {
              anSpriteRect.left = anFrameRect.left;
            }
          }
          // Are we using a vertical row of animation images?
          if(anFrameModifier.y > 0)
          {
            anSpriteRect.top += anSpriteRect.height*anFrameModifier.y;
            if(anSpriteRect.top >= anFrameRect.top+anFrameRect.height)
            {
              anSpriteRect.top = anFrameRect.top;
            }
          }

          // Restart our animation frame clock
          anFrameClock.restart();

          // Now update our AnimationSystem FrameClock property
          anEntity->mProperties.Set<sf::Clock>("FrameClock",anFrameClock);

          // Update our RenderSystem SpriteRect property
          anEntity->mProperties.Set<sf::IntRect>("rSpriteRect",anSpriteRect);
        } // if(anFrameClock > anFrameDelay)
#endif
      } // while(anQueue != anIter->second.end())

      // Increment map iterator
      anIter++;
    } //while(anIter != mEntities.end())
  }

  void AnimationSystem::UpdateVariable(float theElapsedTime)
  {
  }

  void AnimationSystem::Draw()
  {
  }

  void AnimationSystem::HandleCleanup(GQE::IEntity* theEntity)
  {
  }
}

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

