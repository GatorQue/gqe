/**
 * Provides several math functions and methods used throughout
 * the GQE namespace.
 *
 * @file include/GQE/Core/utils/MathUtils.hpp
 * @author Jacob Dix
 * @date 20120809 - Initial Release
 */
#ifndef CORE_MATH_UTIL_HPP_INCLUDED
#define CORE_MATH_UTIL_HPP_INCLUDED
#define PI 3.14159265
#include <math.h>
#include <GQE/Core/Core_types.hpp>
namespace GQE
{
void GQE_API SetSeed(GQE::Uint32 theSeed);

float GQE_API Random(float theMin, float theMax);

int GQE_API Random(int theMin, int theMax);

GQE::Uint32 GQE_API Random(GQE::Uint32 theMin, GQE::Uint32 theMax);

sf::Vector2f GQE_API NormalizeVector(sf::Vector2f theVector);

float GQE_API dotProduct(sf::Vector2f theVectorA, sf::Vector2f theVectorB);

template <typename T> int GetSign(T theValue) {
    return (T(0) < theValue) - (theValue < T(0));
}

float GQE_API ToRadians(float theDegrees);

float GQE_API ToDegrees(float theRadians);


}

#endif // CORE_MATH_UTIL_HPP_INCLUDED
/**
 * @ingroup Core
 * Provides several math functions and methods used throughout
 * the GQE namespace.
 *
 *
 * Copyright (c) 2010-2011 Ryan Lindeman
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
