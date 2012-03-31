/**
 * Class for making different polygon shapes for collision detection;
 * @file include/GQE/Entities/classes/CollisionPolygon.hpp
 * @author Jacob Dix.
 * @date 20110509 - Cleaned up and commented.
 * @date 20110907 - Additional code and comment cleanup.
 */
#ifndef ENTITIES_COLLISION_POLY_HPP_INCLUDED
#define ENTITIES_COLLISION_POLY_HPP_INCLUDED

#include <GQE/Entities/Entities_types.hpp>
#include <SFML/Graphics.hpp>

namespace GQE
{
  /// Provides the CollisionPolygon class for performing collision detection polygons.
  class CollisionPolygon
  {
    public:
      /**
       * CollisionPolygon constructor
       */
      CollisionPolygon();

      /**
       * CollisionPolygon deconstructor
       */
      virtual ~CollisionPolygon();

      /**
       * GetSides returns the list of points in the polygon
       *
       * @return The list of Points
       */
      typeVectorList* GetVectorList(void);

      /**
       * GetSides returns the value in mSides.
       *
       * @return The value of mSides.
       */
      int GetSides(void);

      /**
       * GetPoints returns the value in mPoints.
       *
       * @return The value of mPoints.
       */
      int GetPoints(void);

      /**
       * GetPosition returns a vector with the Collision Polygons Position.
       *
       * @return The value of mPosition.
       */
      sf::Vector2f GetPosition(void);

      /**
       * Sets the Position of the polygon.
       *
       * @param[in] theX Position;
       * @param[in] theY Position;
       */
      void SetPosition(float theX, float theY);

      /**
       * GetSFPolygon returns a SFML polygon that can be used to debug shapes.
       *
       * @return a sf::Shape Object.
       */
      sf::Shape GetSFPolygon(void);

      /**
       * Rotate Rotates the polygon relitive to its current rotation
       *
       * @param[in] theAngle in degrees.
       */
      void Rotate(float theAngle);

      /**
       * SetRotation Rotates the polygon relitive to is origional rotation
       *
       * @param[in] theAngle in degrees.
       */
      void SetRotation(float theAngle);

      /**
       * MakeLine Creates a line polygon from two points
       *
       * @param[in] theX1 of the first point.
       * @param[in] theY1 of the first point.
       * @param[in] theX2 of the second point.
       * @param[in] theY2 of the second point.
       */
      void MakeLine(float theX1, float theY1, float theX2, float theY2);

      /**
       * MakeRectangle Creates a Rectangle polygon from a half width and half height.
       *
       * @param[in] theHalfWidth of the rectangle.
       * @param[in] theHalfHeight of the rectangle.
       */
      void MakeRectangle(float theHalfWidth, float theHalfHeight);

      /**
       * MakeEqualTriandle Creates a Equalateral triangle polygon a base width.
       *
       * @param[in] theWidth of the bottom (as well as the length of each side).
       */
      void MakeEqualTriangle(float theWidth);

    protected:
      /**
       * Adds a point to the Vector List.
       *
       * @param[in] theX Position of the Point.
       * @param[in] theY Position of the Point.
       */
      void AddPoint(float theX, float theY);

      /**
       * Compleatly clears the Polygons points and sets mSides and mPoints to Zero
       *
       */
      void Clear(void);

      /**
       * Rotates a single point.
       * @param[in] thePoint is the starting vector
       * @param[in] theAngle the angle to rotate the point by.
       * @return The ajusted vector.
       **/
      sf::Vector2f PointRotate(sf::Vector2f thePoint, float theAngle);

      // Polygon Data
      /// Number of sides to the polygon
      int mSides;
      /// Number of points describing the polygon
      int mPoints;
      /// The vector position of the polygon
      sf::Vector2f mPosition;
      /// The original vectors of the polygon to be used for rotation code
      typeVectorList mOriVectorList;
      /// The current vector list of the polygon
      typeVectorList mVectorList;
  };
} // namespace GQE
#endif // ENTITIES_COLLISION_POLY_HPP_INCLUDED

/**
 * @class GQE::CollisionPolygon
 * @ingroup Entities
 * This class stores polygon data for collision detection. It can generate lines rectangles and triangles.
 *
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
