/**
 * The base class for a game object. This object includes Position, Velocity, Acceleration, Rotation,
 * and Scale. All of which are 3 dementional vectors(this will allow easy support for any 3d library
 * to use these objects)
 * @file src/GQE/Entities/interfaces/IObject.cpp
 * @author Jacob Dix
 * @date 20110907 - Split IObject.hpp into IObject.cpp (and changed GQE Coding Standard)
 * @date 20110907 - Additional code and comment cleanup.
 */

#include <GQE/Core/classes/App.hpp>
#include <GQE/Entities/interfaces/IObject.hpp>
#include <GQE/Core/classes/ConfigReader.hpp>
//#include <GQE/Core/utils/StringUtil.hpp>
#include <fstream>
namespace GQE
{
  IObject::IObject(App* theApp, typeObjectID theObjectID, unsigned char theFlags) :
    mApp(theApp),
    mObjectID(theObjectID),
    mPosition(sf::Vector3f(0,0,0)),
    mOldPosition(sf::Vector3f(0,0,0)),
    mVelocity(sf::Vector3f(0,0,0)),
    mAcceleration(sf::Vector3f(0,0,0)),
    mRotation(sf::Vector3f(0,0,0)),
    mScale(sf::Vector3f(1,1,1)),
    mSprite(NULL),
    mSpriteName(""),
    mInit(false),
    mCleanup(false),
    mFlags(theFlags)
  {
  }

  IObject::~IObject()
  {
  }

  void IObject::DoInit(void)
  {
    mInit = true;
    mCleanup = false;
  }

  void IObject::DeInit(void)
  {
    if(mInit)
    {
      mInit = false;
      mCleanup = true;
    }
  }

  void IObject::Rotate(float theX, float theY, float theZ)
  {
    mRotation.x += theX;
    mRotation.y += theY;
    mRotation.z += theZ;
  }

  typeObjectID IObject::GetID(void)
  {
    return mObjectID;
  }
  unsigned char IObject::GetFlags()
  {
    return mFlags;
  }
  sf::Vector3f IObject::GetPosition(void)
  {
    return mPosition;
  }

  sf::Vector3f IObject::GetRotation(void)
  {
    return mRotation;
  }

  sf::Vector3f IObject::GetScale(void)
  {
    return mScale;
  }

  void IObject::SetPosition(float theX, float theY, float theZ)
  {
    mPosition = sf::Vector3f(theX,theY,theZ);
  }

  void IObject::SetRotation(float theX, float theY, float theZ)
  {
    mRotation = sf::Vector3f(theX,theY,theZ);
  }

  void IObject::SetScale(float theX, float theY, float theZ)
  {
    mScale = sf::Vector3f(theX,theY,theZ);
  }

  void IObject::SetVelocity(float theX, float theY, float theZ)
  {
    mVelocity = sf::Vector3f(theX,theY,theZ);
  }

  sf::Vector3f IObject::GetVelocity(void)
  {
    return mVelocity;
  }

  void IObject::SetAcceleration(float theX, float theY, float theZ)
  {
    mAcceleration = sf::Vector3f(theX,theY,theZ);
  }

  sf::Vector3f IObject::GetAcceleration(void)
  {
    return mAcceleration;
  }

  void IObject::SetSprite(std::string theSpriteName)
  {
    mSpriteName=theSpriteName;
    mSprite = mApp->mAssetManager.GetSprite(theSpriteName);
    if(NULL != mSprite)
    {
#if (SFML_VERSION_MAJOR < 2)
      mSprite->SetCenter(mSprite->GetSize().x/2,mSprite->GetSize().y/2);
      mSprite->SetPosition(mPosition.x, mPosition.y);
      mSprite->SetRotation(mRotation.z);
#else
      mSprite->setOrigin(mSprite->getGlobalBounds().width/2, mSprite->getGlobalBounds().height/2);
      mSprite->setPosition(mPosition.x, mPosition.y);
      mSprite->setRotation(mRotation.z);
#endif
    }
  }
  sf::Sprite* IObject::GetSprite()
  {
    return mSprite;
  }
  void IObject::HandleEvents(sf::Event theEvent)
  {
  }

  void IObject::Collision(IObject* theOtherObject,
      typeCollisionResult theResult)
  {
  }

  void IObject::UpdateFixed(void)
  {
    mVelocity = mVelocity+mAcceleration;
    mOldPosition=mPosition;
    mPosition = mPosition+(mVelocity);
    mCurrentRect=mBoundRect;
#if (SFML_VERSION_MAJOR < 2)
    mCurrentRect.Offset(mPosition.x,mPosition.y);
#else
    mCurrentRect.left += mPosition.x;
    mCurrentRect.top += mPosition.y;
#endif
  }

  void IObject::UpdateVariable(float theElapsedTime)
  {

  }

  void IObject::Draw(void)
  {
    if(NULL != mSprite)
    {
#if (SFML_VERSION_MAJOR < 2)
      mSprite->SetPosition(mPosition.x, mPosition.y);
      mSprite->SetRotation(mRotation.z);
      mApp->mWindow.Draw(*mSprite);
#else
      mSprite->setPosition(mPosition.x, mPosition.y);
      mSprite->setRotation(mRotation.z);
      mApp->mWindow.draw(*mSprite);
#endif
    }

  }

  void IObject::SetBoundingRect(sf::FloatRect theRect)
  {
    mBoundRect=theRect;
  }

  sf::FloatRect IObject::GetBoundingRect(void)
  {
    return mCurrentRect;
  }

  /**
   * HandleCleanup is responsible for calling Cleanup if this class has been
   * flagged to be cleaned up after it completes the game loop.
   */
  void IObject::HandleCleanup(void)
  {
    if(mSprite!=NULL)
    {
      delete mSprite;
      mSprite=NULL;
    }
    Cleanup();
  }

  /**
   * Cleanup is responsible for performing any cleanup required before
   * this object is removed.
   */
  void IObject::Cleanup(void)
  {
  }
} // namespace GQE
/**
 * Copyright (c) 2011 Jacob Dix
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
