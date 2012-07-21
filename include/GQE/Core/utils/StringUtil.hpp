/**
 * Provides several string parsing and manipulation methods used throughout
 * the GQE namespace.
 *
 * @file include/GQE/Core/utils/StringUtil.hpp
 * @author Ryan Lindeman
 * @date 20110820 - Initial Release
 * @date 20110906 - Moved Util.hpp from Entities to here
 * @date 20120720 - Added several new Convert and Parse functions
 */
#ifndef   CORE_STRING_UTIL_HPP_INCLUDED
#define   CORE_STRING_UTIL_HPP_INCLUDED

#include <string>
#include <GQE/Core/Core_types.hpp>
#include <SFML/Graphics.hpp>

namespace GQE
{
  ///////////////////////////////////////////////////////////////////////////
  // String Manipulation Methods
  ///////////////////////////////////////////////////////////////////////////
  /**
   * ConvertBool will convert the boolean value provided into a string.
   * @param[in] theBoolean to convert to a string.
   * @return the resulting string after converting theBoolean
   */
  std::string GQE_API ConvertBool(const bool theBoolean);

  /**
   * ConvertColor will convert theColor value provided into a string.
   * @param[in] theVector to convert to a string.
   * @return the resulting string after converting theVector
   */
  std::string GQE_API ConvertColor(const sf::Color theVector);

  /**
   * ConvertFloat will convert the float value provided into a string.
   * @param[in] theFloat to convert to a string.
   * @return the resulting string after converting theFloat
   */
  std::string GQE_API ConvertFloat(const bool theFloat);

  /**
   * ConvertInt32 will convert the 32 bit signed integer provided into a
   * string.
   * @param[in] theNumber to convert to a string.
   * @return the resulting string after converting theNumber
   */
  std::string GQE_API ConvertInt32(const Int32 theNumber);

  /**
   * ConvertUint32 will convert the 32 bit unsigned integer provided into a
   * string.
   * @param[in] theNumber to convert to a string.
   * @return the resulting string after converting theNumber
   */
  std::string GQE_API ConvertUint32(const Uint32 theNumber);

  /**
   * ConvertVector2f will convert theVector value provided into a
   * string.
   * @param[in] theVector to convert to a string.
   * @return the resulting string after converting theVector
   */
  std::string GQE_API ConvertVector2f(const sf::Vector2f theVector);

  /**
   * ConvertVector3f will convert theVector value provided into a
   * string.
   * @param[in] theVector to convert to a string.
   * @return the resulting string after converting theVector
   */
  std::string GQE_API ConvertVector3f(const sf::Vector3f theVector);

  ///////////////////////////////////////////////////////////////////////////
  // String Parse Methods
  ///////////////////////////////////////////////////////////////////////////
  /**
   * ParseBool will parse theValue string to obtain the boolean value to
   * return.  If the value is not one of the following (0,1,true,false,on,
   * off) then theDefault will be returned instead.
   * @param[in] theValue to parse for (0,1,true,false,on,off)
   * @param[in] theDefault value to return if not one of the above
   * @return the boolean value obtained
   */
  bool GQE_API ParseBool(std::string theValue, const bool theDefault);

  /**
   * ParseColor will parse theValue string to obtain the R,G,B,A color values
   * to produce an sf::Color object for the GetColor method above.
   * @param[in] theValue to parse for R,G,B,A color values
   * @param[in] theDefault color to use if the parser fails
   * @return the color object created with the values obtained
   */
  sf::Color GQE_API ParseColor(const std::string theValue, const sf::Color theDefault);

  /**
   * ParseFloat will parse theValue string to obtain the float value to
   * return or return theDefault instead.
   * @param[in] theValue to parse for the float value
   * @param[in] theDefault float value to use if the parser fails
   * @return the float value obtained or theDefault if not parsed
   */
  float GQE_API ParseFloat(const std::string theValue, const float theDefault);

  /**
   * ParseInt32 will parse theValue string to obtain a signed 32 bit value.
   * If the parser fails, then it will return theDefault instead.
   * @param[in] theValue to parse for the signed 32 bit value
   * @param[in] theDefault signed 32 bit value to use if the parser fails
   * @return the signed 32 bit value obtained
   */
  Int32 GQE_API ParseInt32(const std::string theValue, const Int32 theDefault);

  /**
   * ParseUint32 will parse theValue string to obtain an unsigned 32 bit
   * value.  If the parser fails, then it will return theDefault instead.
   * @param[in] theValue to parse for the unsigned 32 bit value
   * @param[in] theDefault unsigned 32 bit value to use if the parser fails
   * @return the unsigned 32 bit value obtained
   */
  Uint32 GQE_API ParseUint32(const std::string theValue, const Uint32 theDefault);

  /**
   * ParseVector2f will parse theValue string to obtain the X,Y vector values
   * to produce an sf::Vector2f object.
   * @param[in] theValue to parse for X,Y vector values
   * @param[in] theDefault color to use if the parser fails
   * @return the color object created with the values obtained
   */
  sf::Vector2f GQE_API ParseVector2f(const std::string theValue, const sf::Vector2f theDefault);

  /**
   * ParseVector3f will parse theValue string to obtain the X,Y,Z vector values
   * to produce an sf::Vector3f object.
   * @param[in] theValue to parse for X,Y,Z vector values
   * @param[in] theDefault color to use if the parser fails
   * @return the color object created with the values obtained
   */
  sf::Vector3f GQE_API ParseVector3f(const std::string theValue, const sf::Vector3f theDefault);

} // namespace GQE

#endif // CORE_STRING_UTIL_HPP_INCLUDED
/**
 * @ingroup Core
 * This file contains all the string parsing and manipulation methods that can
 * be used by any class in the GQE namespace.  The ConfigReader class in
 * particular makes frequent use of these methods.
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
