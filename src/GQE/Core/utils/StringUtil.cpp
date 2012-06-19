/**
 * Provides several string parsing and manipulation methods used throughout
 * the GQE namespace.
 *
 * @file src/GQE/Core/utils/StringUtil.cpp
 * @author Ryan Lindeman
 * @date 20110820 - Initial Release
 * @date 20110906 - Moved Util.cpp from Entities to here
 */

#include <sstream>
#include <algorithm>
#include <GQE/Core/utils/StringUtil.hpp>

namespace GQE
{
  std::string ConvertInt32(const Int32 theNumber)
  {
    // Use StringStream class to convert theNumber to a string
    std::stringstream anResult;

    // Add theNumber to the stringstream
    anResult << theNumber;

    // Return the string result created by stringstream
    return anResult.str();
  }

  bool ParseBool(std::string theValue, const bool theDefault)
  {
    bool anResult = theDefault;

    // First convert theValue provided into lower case
    std::transform(theValue.begin(), theValue.end(), theValue.begin(), tolower);

    // Look for true/1/on results
    if(theValue == "true" || theValue == "1" || theValue == "on")
    {
      anResult = true;
    }

    // Look for false results
    if(theValue == "false" || theValue == "0" || theValue == "off")
    {
      anResult = false;
    }

    // Return the result found or theDefault assigned above
    return anResult;
  }

  sf::Color ParseColor(const std::string theValue, const sf::Color theDefault)
  {
    sf::Color anResult = theDefault;

    // Try to find the first value: Red
    size_t anRedOffset = theValue.find_first_of(',');
    if(anRedOffset != std::string::npos)
    {
      sf::Uint8 anRed = (sf::Uint8)atoi(theValue.substr(0,anRedOffset).c_str());
      // Try to find the next value: Green
      size_t anGreenOffset = theValue.find_first_of(',',anRedOffset+1);
      if(anGreenOffset != std::string::npos)
      {
        sf::Uint8 anGreen = (sf::Uint8)atoi(theValue.substr(anRedOffset+1,anGreenOffset).c_str());
        // Try to find the next value: Blue
        size_t anBlueOffset = theValue.find_first_of(',',anGreenOffset+1);
        if(anBlueOffset != std::string::npos)
        {
          sf::Uint8 anBlue = (sf::Uint8)atoi(theValue.substr(anGreenOffset+1,anBlueOffset).c_str());
          sf::Uint8 anAlpha = (sf::Uint8)atoi(theValue.substr(anBlueOffset+1).c_str());
          // Now that all 4 values have been parsed, return the color found
          anResult.r = anRed;
          anResult.g = anGreen;
          anResult.b = anBlue;
          anResult.a = anAlpha;
        }
      }
    }

    // Return the result found or theDefault assigned above
    return anResult;
  }

  float ParseFloat(const std::string theValue, const float theDefault)
  {
    float anResult = theDefault;
    std::istringstream iss(theValue);

    // Convert the string to a floating point number
    iss >> anResult;

    // Return the result found or theDefault assigned above
    return anResult;
  }

  Uint32 ParseUint32(const std::string theValue, const Uint32 theDefault)
  {
    Uint32 anResult = theDefault;
    std::istringstream iss(theValue);

    // Convert the string to an unsigned 32 bit integer
    iss >> anResult;

    // Return the result found or theDefault assigned above
    return anResult;
  }
} // namespace GQE

/**
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
