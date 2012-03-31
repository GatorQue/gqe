/**
 * Object Manager used for managing in game objects(any thing that inharets from the IObject class).
 * @file src/GQE/Entities/classes/ObjectManager.cpp
 * @author Jacob Dix.
 * @date 20110905 - Cleaned up and Commented Code.
 * @date 20110907 - Additional code and comment cleanup.
 */

#include <float.h>
#include <math.h>
#include <iterator>
#include <algorithm>
#include <GQE/Entities/classes/ObjectManager.hpp>
#include <GQE/Entities/classes/CollisionPolygon.hpp>
#include <GQE/Entities/interfaces/IObject.hpp>

namespace GQE
{
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
      theObject->DoInit();
      mAddList.push(theObject);
    }
  }

  void ObjectManager::CheckAllCollision(void)
  {
    typeCollisionResult anCollisionResult,anOtherResult;
    CollisionPolygon* anSourcePolygon;
    CollisionPolygon* anOtherPolygon;
    std::vector<IObject*>::iterator it;
    std::vector<IObject*>::iterator it2;
    for(it = mCollisionList.begin(); it != mCollisionList.end(); it++)
    {
      anSourcePolygon=(*it)->GetCollisionPolygon();
      for(it2 = mCollisionList.begin(); it2 != mCollisionList.end(); it2++)
      {
        if((*it)!=(*it2) && (it<it2))
        {
          anOtherPolygon=(*it2)->GetCollisionPolygon();
          anCollisionResult=CheckCollision(anSourcePolygon, anOtherPolygon);
          anOtherResult=CheckCollision(anOtherPolygon, anSourcePolygon);
          if(anCollisionResult.Intersects && anOtherResult.Intersects)
          {
            (*it)->Collision((*it2),anCollisionResult);
            (*it2)->Collision((*it),anCollisionResult);
          }
        }
      }
    }
  }

  typeCollisionResult ObjectManager::CheckCollision(CollisionPolygon* theSourcePolygon,
      CollisionPolygon* theOtherPolygon)
  {
    bool anReturn = true;

    int anSourceFaces = theSourcePolygon->GetSides();
    int anOtherFaces = theOtherPolygon->GetSides();

    typeVectorList* anSourceVectorList=theSourcePolygon->GetVectorList();
    typeVectorList* anOtherVectorList=theOtherPolygon->GetVectorList();

    //float anMinTrans;
    sf::Vector2f anTransAxis;

    //For every face in c1
    for(int i = 0; i < anSourceFaces; i++)
    {
      //float anTrans;
      //Grab a face (face x, face y)
      float anFaceX = anSourceVectorList->at(i).x - anSourceVectorList->at((i + 1) % anSourceFaces).x;
      float anFaceY = anSourceVectorList->at(i).y - anSourceVectorList->at((i + 1) % anSourceFaces).y;

      //Create a perpendicular axis to project on (axis x, axis y)
      float anAxisX = -anFaceX, anAxisY = anFaceY;

      //Normalize the axis
      float anLenV = sqrt(anAxisX * anAxisX + anAxisY * anAxisY);
      anAxisX /= anLenV;
      anAxisY /= anLenV;

      //Carve out the min and max values
      float anSourceMin = FLT_MAX, anSourceMax = -FLT_MAX;
      float anOtherMin = FLT_MAX, anOtherMax = -FLT_MAX;

      //Project every point in Source on the axis and store min and max
      for(int j = 0; j < anSourceFaces; j++)
      {
        float anSourceProj = (anAxisX * (anSourceVectorList->at(j).x + theSourcePolygon->GetPosition().x) + anAxisY * (anSourceVectorList->at(j).y + theSourcePolygon->GetPosition().y)) / (anAxisX * anAxisX + anAxisY * anAxisY);
        anSourceMin = std::min(anSourceProj, anSourceMin);
        anSourceMax = std::max(anSourceProj, anSourceMax);
      }

      //Project every point in Other on the axis and store min and max
      for(int j = 0; j < anOtherFaces; j++)
      {
        float anOtherProj = (anAxisX * (anOtherVectorList->at(j).x + theOtherPolygon->GetPosition().x) + anAxisY * (anOtherVectorList->at(j).y + theOtherPolygon->GetPosition().y)) / (anAxisX * anAxisX + anAxisY * anAxisY);
        anOtherMin = std::min(anOtherProj, anOtherMin);
        anOtherMax = std::max(anOtherProj, anOtherMax);
      }

      //Return if the projections do not overlap
      if(!(anSourceMax >= anOtherMin && anSourceMin <= anOtherMax))
      {
        anReturn = false; //return false
      }
    }
    typeCollisionResult anCollisionreturn;
    anCollisionreturn.Intersects=anReturn;
    anCollisionreturn.Depth=anTransAxis;
    return anCollisionreturn; //return true;
  }

  void ObjectManager::UpdateFixed(void)
  {
    std::vector<IObject*>::iterator it;
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
    std::vector<IObject*>::iterator it;
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
    std::vector<IObject*>::iterator it;
    while(!mAddList.empty())
    {
      anTempObject=mAddList.front();
      mAddList.pop();
      if(anTempObject!=NULL)
      {
        mObjectList.push_back(anTempObject);
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
        mObjectList.erase(it);
        delete anTempObject;
      }
    }
  }


  void ObjectManager::ClearObjects(void)
  {
    //Clean to add objects waiting to be added.
    UpdateLists();
    //Set All Objects to be removed.
    std::vector<IObject*>::iterator it;
    for(it=mObjectList.begin(); it!=mObjectList.end(); it++)
    {
      RemoveObject(*it);
    }
    //Clean to remove all objects set to remove.
    UpdateLists();
  }
  void ObjectManager::Draw(void)
  {
    std::vector<IObject*>::iterator it;
    for(it=mObjectList.begin(); it<mObjectList.end(); it++)
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
