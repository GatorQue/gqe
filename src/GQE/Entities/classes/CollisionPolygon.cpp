/**
 * Class for making different polygon shapes for collision detection;
 * @file src/GQE/Entities/interfaces/CollisionPolygon.cpp
 * @author Jacob Dix.
 * @date 20110509 - Cleaned up and commented.
 * @date 20110907 - Additional code and comment cleanup.
 */
#include <GQE/Entities/classes/CollisionPolygon.hpp>

namespace GQE
{
  CollisionPolygon::CollisionPolygon() :
    mSides(0),
    mPoints(0),
    mPosition(0,0)
  {
  }

  CollisionPolygon::~CollisionPolygon()
  {
  }

  typeVectorList* CollisionPolygon::GetVectorList(void)
  {
    return &mVectorList;
  }

  int CollisionPolygon::GetSides(void)
  {
    return mSides;
  }

  int CollisionPolygon::GetPoints(void)
  {
    return mPoints;
  }

  sf::Vector2f CollisionPolygon::GetPosition(void)
  {
    return mPosition;
  }

  void CollisionPolygon::SetPosition(float theX, float theY)
  {
    mPosition = sf::Vector2f(theX,theY);
  }

  void CollisionPolygon::MakeLine(float x1, float y1, float x2, float y2)
  {
    Clear();
    mPoints=2;
    mSides=1;
    //Point 1
    AddPoint(x1,y1);
    //Point 2
    AddPoint(x2,y2);
  }

  void CollisionPolygon::MakeRectangle(float hw, float hh)
  {
    Clear();
    mPoints=4;
    mSides=4;
    AddPoint(-hw,-hh);
    AddPoint(hw,-hh);
    AddPoint(hw,hh);
    AddPoint(-hw,hh);
  }

  void CollisionPolygon::MakeEqualTriangle(float w)
  {
    Clear();
    mPoints=3;
    mSides=3;
    AddPoint(0,-sqrtf((w*w)-((w/2)*(w/2)))/2);
    AddPoint(-w/2,sqrtf((w*w)-((w/2)*(w/2)))/2);
    AddPoint(w/2,sqrtf((w*w)-((w/2)*(w/2)))/2);
  }

  void CollisionPolygon::AddPoint(float theX, float theY)
  {
    mOriVectorList.push_back(sf::Vector2f(theX,theY));
    mVectorList.push_back(sf::Vector2f(theX,theY));
  }

  void CollisionPolygon::Clear()
  {
    mOriVectorList.clear();
    mVectorList.clear();
    mSides=0;
    mPoints=0;
  }

  void CollisionPolygon::SetRotation(float angle)
  {
    mVectorList=mOriVectorList;
    for(int p=0; p<mPoints; p++)
    {
      mVectorList[p]=PointRotate(mVectorList[p], angle);
    }
  }

  void CollisionPolygon::Rotate(float angle)
  {
    for(int p=0; p<mPoints; p++)
    {
      mVectorList[p]=PointRotate(mVectorList[p], angle);
    }
  }

  sf::Shape CollisionPolygon::GetSFPolygon()
  {
    sf::Shape anPolygon;
    for(int p=0; p<mPoints; p++)
    {
      anPolygon.AddPoint(mVectorList[p].x+mPosition.x,mVectorList[p].y+mPosition.y, sf::Color(255,150,150));
    }
    return anPolygon;
  }
  sf::Vector2f CollisionPolygon::PointRotate(sf::Vector2f Point, float angle)
  {
    // convert angle to radians
    angle = -(angle * PI / 180.0f);
    float transformedX = Point.x * cos( angle ) - Point.y * sin( angle );
    float transformedY = Point.x * sin( angle ) + Point.y * cos( angle );
    // return coordinates relative to top left corner
    return sf::Vector2f(transformedX, transformedY);
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
