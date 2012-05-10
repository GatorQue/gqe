/**
 * Provides several string parsing and manipulation methods used throughout
 * the GQE namespace.
 *
 * @file include/GQE/Core/utils/StringUtil.hpp
 * @author Ryan Lindeman
 * @date 20110820 - Initial Release
 * @date 20110906 - Moved Util.hpp from Entities to here
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
   * ConvertInt32 will convert the 32 bit signed integer provided into a
   * string.
   * @param[in] theNumber to convert to a string.
   * @return the resulting string after converting theNumber
   */
  std::string ConvertInt32(const Int32 theNumber);

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
  bool ParseBool(std::string theValue, const bool theDefault);

  /**
   * ParseColor will parse theValue string to obtain the R,G,B,A color values
   * to produce an sf::Color object for the GetColor method above.
   * @param[in] theValue to parse for R,G,B,A color values
   * @param[in] theDefault color to use if the parser fails
   * @return the color object created with the values obtained
   */
  sf::Color ParseColor(const std::string theValue, const sf::Color theDefault);

  /**
   * ParseFloat will parse theValue string to obtain the float value to
   * return or return theDefault instead.
   * @param[in] theValue to parse for the float value
   * @param[in] theDefault float value to use if the parser fails
   * @return the float value obtained or theDefault if not parsed
   */
  float ParseFloat(const std::string theValue, const float theDefault);

  /**
   * ParseUint32 will parse theValue string to obtain an unsigned 32 bit
   * value.  If the parser fails, then it will return theDefault instead.
   * @param[in] theValue to parse for the unsigned 32 bit value
   * @param[in] theDefault unsigned 32 bit value to use if the parser fails
   * @return the unsigned 32 bit value obtained
   */
  Uint32 ParseUint32(const std::string theValue, const Uint32 theDefault);
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
