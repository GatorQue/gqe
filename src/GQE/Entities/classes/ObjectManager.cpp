/**
 * Object Manager used for managing in game objects(any thing that inharets from the IObject class).
 * @file src/GQE/Entities/classes/ObjectManager.cpp
 * @author Jacob Dix.
 * @date 20110905 - Cleaned up and Commented Code.
 * @date 20110907 - Additional code and comment cleanup.
 */

#include <float.h>
#include <math.h>
#include <limits>
#include <iterator>
#include <algorithm>
#include <fstream>


#include <GQE/Entities/classes/ObjectManager.hpp>
#include <GQE/Entities/interfaces/IObject.hpp>

namespace GQE
{
  bool ZOrder (IObject* i,IObject* j)
  {
    return (i->GetPosition().z<j->GetPosition().z);
  }

  ObjectManager::ObjectManager()
  {

  }

  ObjectManager::~ObjectManager()
  {
    ClearObjects();
  }

  void ObjectManager::RemoveObject(IObject* theObject)
  {
    mRemoveList.push(theObject);
  }

  void ObjectManager::AddObject(IObject* theObject)
  {
    if(theObject!=NULL)
    {
      mAddList.push(theObject);
    }
  }

  IObject* ObjectManager::AddObjectFromFile(std::string theFileName)
  {
    return NULL;
  }

  void ObjectManager::CheckAllCollision(void)
  {
    typeCollisionResult anCollisionResult,anOtherResult;
    sf::FloatRect anSourceRect;
    sf::FloatRect anOtherRect;
    std::list<IObject*>::iterator anSourceObject;
    std::list<IObject*>::iterator anOtherObject;
    sf::Vector3f anSourceCenter, anOtherCenter;
    for(anSourceObject = mCollisionList.begin(); anSourceObject != mCollisionList.end(); ++anSourceObject)
    {
      anSourceRect=(*anSourceObject)->GetBoundingRect();
      anOtherObject=anSourceObject;
      for(++anOtherObject;anOtherObject != mCollisionList.end(); ++anOtherObject)
      {
        anOtherRect=(*anOtherObject)->GetBoundingRect();
        if(!(((*anSourceObject)->GetFlags() & ObjectStatic) && ((*anOtherObject)->GetFlags() & ObjectStatic)))
        {
          anCollisionResult=RectIntersect(anSourceRect, anOtherRect);
          if(anCollisionResult.Intersect)
          {
            (*anSourceObject)->Collision((*anOtherObject),anCollisionResult);
            (*anOtherObject)->Collision((*anSourceObject),anCollisionResult);
          }
        }
      }
    }
  }

  // Check if polygon A is going to collide with polygon B.
  // The last parameter is the *relative* velocity
  // of the polygons (i.e. velocityA - velocityB)
  typeCollisionResult ObjectManager::RectIntersect(sf::FloatRect theSourceRect, sf::FloatRect theOtherRect)
  {
    typeCollisionResult anResult;
    anResult.Intersect = false;
#if (SFML_VERSION_MAJOR < 2)
    if(theSourceRect.Intersects(theOtherRect, &anResult.Overlap))
#else
    if(theSourceRect.intersects(theOtherRect, anResult.Overlap))
#endif
    {
      anResult.Intersect=true;
    }
    return anResult;
  }

  void ObjectManager::UpdateFixed(void)
  {
    std::list<IObject*>::iterator it;
    for(it=mObjectList.begin(); it!=mObjectList.end(); it++)
    {
      if((*it)!=NULL)
      {
        (*it)->UpdateFixed();
      }
    }
  }

  void ObjectManager::UpdateVariable(float theElapsedTime)
  {
    std::list<IObject*>::iterator it;
    for(it=mObjectList.begin(); it!=mObjectList.end(); it++)
    {
      if((*it)!=NULL)
      {
        (*it)->UpdateVariable(theElapsedTime);
      }
    }
  }

  void ObjectManager::UpdateLists(void)
  {
    IObject* anTempObject;
    std::list<IObject*>::iterator it;
    while(!mAddList.empty())
    {
      anTempObject=mAddList.front();
      mAddList.pop();
      if(anTempObject!=NULL)
      {
        mObjectList.push_back(anTempObject);
        anTempObject->DoInit();
      }
      if(anTempObject->GetFlags() & ObjectSolid)
      {
        mCollisionList.push_back(anTempObject);
      }
      if(anTempObject->GetFlags() & ObjectVisible)
      {
        mRenderList.push_back(anTempObject);
      }
    }
    while(!mRemoveList.empty())
    {
      anTempObject=mRemoveList.front();
      mRemoveList.pop();
      if(anTempObject!=NULL)
      {
        if(anTempObject->GetFlags() & ObjectSolid)
        {
          it=std::find(mCollisionList.begin(),mCollisionList.end(),anTempObject);
          if(it!=mCollisionList.end())
          {
            mCollisionList.erase(it);
          }
        }
        if(anTempObject->GetFlags() & ObjectVisible)
        {
          it=std::find(mRenderList.begin(),mRenderList.end(),anTempObject);
          if(it!=mRenderList.end())
          {
            mRenderList.erase(it);
          }
        }
        it=std::find(mObjectList.begin(),mObjectList.end(),anTempObject);
        if(it!=mObjectList.end())
        {
          (*it)->DeInit();
          (*it)->HandleCleanup();
          mObjectList.erase(it);
          delete anTempObject;
        }
      }
    }
  }

  void ObjectManager::ClearObjects(void)
  {
    //Clean to add objects waiting to be added.
    UpdateLists();
    //Set All Objects to be removed.
    std::list<IObject*>::iterator it;
    for(it=mObjectList.begin(); it!=mObjectList.end(); it++)
    {
      RemoveObject(*it);
    }
    //Clean to remove all objects set to remove.
    UpdateLists();
  }

  void ObjectManager::RenderSort()
  {
    mRenderList.sort(ZOrder);
  }

  void ObjectManager::Draw(void)
  {
    std::list<IObject*>::iterator it;
    for(it=mRenderList.begin(); it != mRenderList.end(); it++)
    {
      (*it)->Draw();
    }

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
