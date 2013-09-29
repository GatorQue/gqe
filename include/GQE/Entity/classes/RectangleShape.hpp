/**
 * A basic class for rectangle collision objects
 *
 * @file include/GQE/Entity/classes/RectangleShape.hpp
 * @author Jacob Dix
 * @date 
 */
#ifndef RECTANGLE_COLLISION_HPP_INCLUDED
#define RECTANGLE_COLLISION_HPP_INCLUDED

#include <map>
#include <GQE/Core/interfaces/IShape.hpp>
#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
  /// Provides the RectangleShape class
  class GQE_API RectangleShape : public IShape
  {
    public:
      /**
       * RectangleShape default constructor
       */
      RectangleShape(GQE::Uint32 theWidth=0,GQE::Uint32 theHeight=0);

      /**
       * RectangleShape deconstructor
       */
      virtual ~RectangleShape();

			void GenerateRectangle(GQE::Uint32 theWidth=0,GQE::Uint32 theHeight=0);
    protected:

    private:
  };
} // namespace GQE

#endif // RECTANGLE_COLLISION_HPP_INCLUDED

/**
 * @class GQE::RectangleShape
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
