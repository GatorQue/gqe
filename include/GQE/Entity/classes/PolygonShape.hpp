/**
 * A basic class for Triangle collision objects
 *
 * @file include/GQE/Entity/classes/RectangleShape.hpp
 * @author Jacob Dix
 * @date
 */
#ifndef POLYGON_COLLISION_HPP_INCLUDED
#define POLYGON_COLLISION_HPP_INCLUDED

#include <map>
#include <GQE/Core/interfaces/IShape.hpp>
#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
  /// Provides the TriangleShape class
  class GQE_API PolygonShape : public IShape
  {
    public:
      /**
       * TriangleShape default constructor
       */
			PolygonShape(std::vector<sf::Vector2f> thePoints);

      /**
       * TriangleShape deconstructor
       */
      virtual ~PolygonShape();

    protected:

    private:
  };
} // namespace GQE

#endif // TRIANGLE_COLLISION_HPP_INCLUDED

/**
 * @class GQE::TriangleShape
 * @ingroup Entity
 *
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
