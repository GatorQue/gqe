/**
 * Provides the GQE Entities typedef's and forward class declarations.
 *
 * @file include/GQE/Entities/Entities_types.hpp
 * @author Jacob Dix
 * @date 20111108 - Initial Release
 * @date 20110209 - Code Cleaned for submission to the GQE code base.
 * @date 20110907 - Define other types needed for Entities
 */
#ifndef ENTITIES_TYPES_HPP_INCLUDED
#define ENTITIES_TYPES_HPP_INCLUDED

#define PI 3.141592f
#include <string>
#include <vector>
#include <GQE/Config.hpp>
#include <SFML/System.hpp>

namespace GQE
{
  /// Enumeration of all object types
  enum ObjectType
  {
    ObjectSolid   = 0x01,  ///< Solid object
    ObjectVisible = 0x02   ///< Visible object
  };

  // Forward declare GQE entity interfaces provided
  class IObject;

  // Forward declare GQE entity classes provided
  class ObjectManager;
  class CollisionPolygon;

  /// Declare Object ID typedef which is used for identifying objects
  typedef std::string typeObjectID;

  /// Declare VectorList typedef which is used for sf::Vector2f lists
  typedef std::vector<sf::Vector2f> typeVectorList;

  /// Declare CollisionResult typedef for Collision detection results
  typedef struct
  {
    sf::Vector2f Depth;
    bool Intersects;
  } typeCollisionResult;

} // namespace GQE
#endif // ENTITIES_TYPES_HPP_INCLUDED

/**
 * Copyright (c) 2010-2011 Jacob Dix
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

