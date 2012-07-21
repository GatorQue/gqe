/**
 * Provides several string parsing and manipulation methods used throughout
 * the GQE namespace.
 *
 * @file src/GQE/Core/utils/StringUtil.cpp
 * @author Ryan Lindeman
 * @date 20110820 - Initial Release
 * @date 20110906 - Moved Util.cpp from Entities to here
 * @date 20120720 - Added several new Convert and Parse functions
 */

#include <sstream>
#include <algorithm>
#include <GQE/Core/utils/StringUtil.hpp>

namespace GQE
{
  std::string ConvertBool(const bool theBoolean)
  {
    // Use StringStream class to convert theBoolean to a string
    std::stringstream anResult;

    // Add theBoolean to the stringstream
    if(theBoolean)
    {
      anResult << "true";
    }
    else
    {
      anResult << "false";
    }

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertColor(const sf::Color theColor)
  {
    // Use StringStream class to convert theColor to a string
    std::stringstream anResult;

    // Add theColor to the stringstream
    anResult << theColor.r << ", ";
    anResult << theColor.g << ", ";
    anResult << theColor.b << ", ";
    anResult << theColor.a;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertDouble(const double theDouble)
  {
    // Use StringStream class to convert theDouble to a string
    std::stringstream anResult;

    // Add theDouble to the stringstream
    anResult << theDouble;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertFloat(const float theFloat)
  {
    // Use StringStream class to convert theFloat to a string
    std::stringstream anResult;

    // Add theFloat to the stringstream
    anResult << theFloat;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertInt8(const Int8 theNumber)
  {
    // Use StringStream class to convert theNumber to a string
    std::stringstream anResult;

    // Add theNumber to the stringstream
    anResult << theNumber;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertInt16(const Int16 theNumber)
  {
    // Use StringStream class to convert theNumber to a string
    std::stringstream anResult;

    // Add theNumber to the stringstream
    anResult << theNumber;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertInt32(const Int32 theNumber)
  {
    // Use StringStream class to convert theNumber to a string
    std::stringstream anResult;

    // Add theNumber to the stringstream
    anResult << theNumber;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertInt64(const Int64 theNumber)
  {
    // Use StringStream class to convert theNumber to a string
    std::stringstream anResult;

    // Add theNumber to the stringstream
    anResult << theNumber;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertIntRect(const sf::IntRect theRect)
  {
    // Use StringStream class to convert theRect to a string
    std::stringstream anResult;

    // Add theRect to the stringstream
#if (SFML_VERSION_MAJOR < 2)
    anResult << theColor.top << ", ";
    anResult << theColor.left << ", ";
    anResult << theColor.GetWidth() << ", ";
    anResult << theColor.GetHeight();
#else
    anResult << theRect.top << ", ";
    anResult << theRect.left << ", ";
    anResult << theRect.width << ", ";
    anResult << theRect.height;
#endif

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertUint8(const Uint8 theNumber)
  {
    // Use StringStream class to convert theNumber to a string
    std::stringstream anResult;

    // Add theNumber to the stringstream
    anResult << theNumber;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertUint16(const Uint16 theNumber)
  {
    // Use StringStream class to convert theNumber to a string
    std::stringstream anResult;

    // Add theNumber to the stringstream
    anResult << theNumber;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertUint32(const Uint32 theNumber)
  {
    // Use StringStream class to convert theNumber to a string
    std::stringstream anResult;

    // Add theNumber to the stringstream
    anResult << theNumber;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertUint64(const Uint64 theNumber)
  {
    // Use StringStream class to convert theNumber to a string
    std::stringstream anResult;

    // Add theNumber to the stringstream
    anResult << theNumber;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertVector2f(const sf::Vector2f theVector)
  {
    // Use StringStream class to convert theVector to a string
    std::stringstream anResult;

    // Add theVector to the stringstream
    anResult << theVector.x << ", ";
    anResult << theVector.y;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertVector2i(const sf::Vector2i theVector)
  {
    // Use StringStream class to convert theVector to a string
    std::stringstream anResult;

    // Add theVector to the stringstream
    anResult << theVector.x << ", ";
    anResult << theVector.y;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertVector2u(const sf::Vector2u theVector)
  {
    // Use StringStream class to convert theVector to a string
    std::stringstream anResult;

    // Add theVector to the stringstream
    anResult << theVector.x << ", ";
    anResult << theVector.y;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertVector3f(const sf::Vector3f theVector)
  {
    // Use StringStream class to convert theVector to a string
    std::stringstream anResult;

    // Add theVector to the stringstream
    anResult << theVector.x << ", ";
    anResult << theVector.y << ", ";
    anResult << theVector.z;

    // Return the string result created by stringstream
    return anResult.str();
  }

  std::string ConvertVector3i(const sf::Vector3i theVector)
  {
    // Use StringStream class to convert theVector to a string
    std::stringstream anResult;

    // Add theVector to the stringstream
    anResult << theVector.x << ", ";
    anResult << theVector.y << ", ";
    anResult << theVector.z;

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

    // Try to find the first comma
    size_t anComma1Offset = theValue.find_first_of(',');
    if(anComma1Offset != std::string::npos)
    {
      sf::Uint8 anRed = ParseUint8(theValue.substr(0,anComma1Offset), theDefault.r);
      // Try to find the next comma
      size_t anComma2Offset = theValue.find_first_of(',',anComma1Offset+1);
      if(anComma2Offset != std::string::npos)
      {
        sf::Uint8 anGreen = ParseUint8(
          theValue.substr(anComma1Offset+1,anComma2Offset), theDefault.g);
        // Try to find the next comma
        size_t anComma3Offset = theValue.find_first_of(',',anComma2Offset+1);
        if(anComma3Offset != std::string::npos)
        {
          sf::Uint8 anBlue = ParseUint8(
            theValue.substr(anComma2Offset+1,anComma3Offset), theDefault.b);
          sf::Uint8 anAlpha = ParseUint8(
            theValue.substr(anComma3Offset+1), theDefault.a);

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

  double ParseDouble(const std::string theValue, const double theDefault)
  {
    double anResult = theDefault;
    std::istringstream iss(theValue);

    // Convert the string to a double floating point number
    iss >> anResult;

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

  Int8 ParseInt8(const std::string theValue, const Int8 theDefault)
  {
    Int8 anResult = theDefault;
    std::istringstream iss(theValue);

    // Convert the string to a signed 8 bit integer
    iss >> anResult;

    // Return the result found or theDefault assigned above
    return anResult;
  }

  Int16 ParseInt16(const std::string theValue, const Int16 theDefault)
  {
    Int16 anResult = theDefault;
    std::istringstream iss(theValue);

    // Convert the string to a signed 16 bit integer
    iss >> anResult;

    // Return the result found or theDefault assigned above
    return anResult;
  }

  Int32 ParseInt32(const std::string theValue, const Int32 theDefault)
  {
    Int32 anResult = theDefault;
    std::istringstream iss(theValue);

    // Convert the string to a signed 32 bit integer
    iss >> anResult;

    // Return the result found or theDefault assigned above
    return anResult;
  }

  Int64 ParseInt64(const std::string theValue, const Int64 theDefault)
  {
    Int64 anResult = theDefault;
    std::istringstream iss(theValue);

    // Convert the string to a signed 64 bit integer
    iss >> anResult;

    // Return the result found or theDefault assigned above
    return anResult;
  }

  sf::IntRect ParseIntRect(const std::string theValue, const sf::IntRect theDefault)
  {
    sf::IntRect anResult = theDefault;

    // Try to find the first comma
    size_t anComma1Offset = theValue.find_first_of(',');
    if(anComma1Offset != std::string::npos)
    {
      sf::Int32 anLeft = ParseInt32(theValue.substr(0,anComma1Offset), theDefault.left);
      // Try to find the next comma
      size_t anComma2Offset = theValue.find_first_of(',',anComma1Offset+1);
      if(anComma2Offset != std::string::npos)
      {
        sf::Int32 anTop = ParseInt32(theValue.substr(anComma1Offset+1,anComma2Offset), theDefault.top);
        // Try to find the next comma
        size_t anComma3Offset = theValue.find_first_of(',',anComma2Offset+1);
        if(anComma3Offset != std::string::npos)
        {
#if (SFML_VERSION_MAJOR < 2)
          // Get the width and height values
          sf::Int32 anWidth = ParseInt32(theValue.substr(anComma2Offset+1,anComma3Offset), theDefault.GetWidth());
          sf::Int32 anHeight = ParseInt32(theValue.substr(anComma3Offset+1), theDefault.GetHeight());

          // Now that all 4 values have been parsed, return the color found
          anResult.left = anLeft;
          anResult.top = anTop;
          anResult.right = anLeft+anWidth;
          anResult.bottom = anTop+anHeight;
#else
          // Get the width and height values
          sf::Int32 anWidth = ParseInt32(theValue.substr(anComma2Offset+1,anComma3Offset), theDefault.width);
          sf::Int32 anHeight = ParseInt32(theValue.substr(anComma3Offset+1), theDefault.height);

          // Now that all 4 values have been parsed, return the color found
          anResult.left = anLeft;
          anResult.top = anTop;
          anResult.width = anWidth;
          anResult.height = anHeight;
#endif
        }
      }
    }

    // Return the result found or theDefault assigned above
    return anResult;
  }

  Uint8 ParseUint8(const std::string theValue, const Uint8 theDefault)
  {
    Uint8 anResult = theDefault;
    std::istringstream iss(theValue);

    // Convert the string to an unsigned 8 bit integer
    iss >> anResult;

    // Return the result found or theDefault assigned above
    return anResult;
  }

  Uint16 ParseUint16(const std::string theValue, const Uint16 theDefault)
  {
    Uint16 anResult = theDefault;
    std::istringstream iss(theValue);

    // Convert the string to an unsigned 16 bit integer
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

  Uint64 ParseUint64(const std::string theValue, const Uint64 theDefault)
  {
    Uint64 anResult = theDefault;
    std::istringstream iss(theValue);

    // Convert the string to an unsigned 64 bit integer
    iss >> anResult;

    // Return the result found or theDefault assigned above
    return anResult;
  }

  sf::Vector2f ParseVector2f(const std::string theValue, const sf::Vector2f theDefault)
  {
    sf::Vector2f anResult = theDefault;

    // Try to find the first comma
    size_t anCommaOffset = theValue.find_first_of(',');
    if(anCommaOffset != std::string::npos)
    {
      float anX = ParseFloat(theValue.substr(0,anCommaOffset), theDefault.x);
      float anY = ParseFloat(theValue.substr(anCommaOffset+1), theDefault.y);

      // Now that both values have been parsed, return the vector found
      anResult.x = anX;
      anResult.y = anY;
    }

    // Return the result found or theDefault assigned above
    return anResult;
  }

  sf::Vector2i ParseVector2i(const std::string theValue, const sf::Vector2i theDefault)
  {
    sf::Vector2i anResult = theDefault;

    // Try to find the first comma
    size_t anCommaOffset = theValue.find_first_of(',');
    if(anCommaOffset != std::string::npos)
    {
      Int32 anX = ParseInt32(theValue.substr(0,anCommaOffset), theDefault.x);
      Int32 anY = ParseInt32(theValue.substr(anCommaOffset+1), theDefault.y);

      // Now that both values have been parsed, return the vector found
      anResult.x = anX;
      anResult.y = anY;
    }

    // Return the result found or theDefault assigned above
    return anResult;
  }

  sf::Vector2u ParseVector2u(const std::string theValue, const sf::Vector2u theDefault)
  {
    sf::Vector2u anResult = theDefault;

    // Try to find the first comma
    size_t anCommaOffset = theValue.find_first_of(',');
    if(anCommaOffset != std::string::npos)
    {
      Uint32 anX = ParseUint32(theValue.substr(0, anCommaOffset), theDefault.x);
      Uint32 anY = ParseUint32(theValue.substr(anCommaOffset+1), theDefault.y);

      // Now that both values have been parsed, return the vector found
      anResult.x = anX;
      anResult.y = anY;
    }

    // Return the result found or theDefault assigned above
    return anResult;
  }

  sf::Vector3f ParseVector3f(const std::string theValue, const sf::Vector3f theDefault)
  {
    sf::Vector3f anResult = theDefault;

    // Try to find the first comma
    size_t anComma1Offset = theValue.find_first_of(',', 0);
    if(anComma1Offset != std::string::npos)
    {
      float anX = ParseFloat(theValue.substr(0, anComma1Offset), theDefault.x);

      // Try to find the next comma
      size_t anComma2Offset = theValue.find_first_of(',',anComma1Offset+1);
      if(anComma2Offset != std::string::npos)
      {
        float anY = ParseFloat(theValue.substr(anComma1Offset+1, anComma2Offset), theDefault.y);
        float anZ = ParseFloat(theValue.substr(anComma2Offset+1), theDefault.z);
        
        // Now that all 3 values have been parsed, return the Vector3f found
        anResult.x = anX;
        anResult.y = anY;
        anResult.z = anZ;
      }
    }

    // Return the result found or theDefault assigned above
    return anResult;
  }

  sf::Vector3i ParseVector3i(const std::string theValue, const sf::Vector3i theDefault)
  {
    sf::Vector3i anResult = theDefault;

    // Try to find the first comma
    size_t anComma1Offset = theValue.find_first_of(',', 0);
    if(anComma1Offset != std::string::npos)
    {
      Int32 anX = ParseInt32(theValue.substr(0, anComma1Offset), theDefault.x);

      // Try to find the next comma
      size_t anComma2Offset = theValue.find_first_of(',',anComma1Offset+1);
      if(anComma2Offset != std::string::npos)
      {
        Int32 anY = ParseInt32(theValue.substr(anComma1Offset+1, anComma2Offset), theDefault.y);
        Int32 anZ = ParseInt32(theValue.substr(anComma2Offset+1), theDefault.z);
        
        // Now that all 3 values have been parsed, return the Vector3f found
        anResult.x = anX;
        anResult.y = anY;
        anResult.z = anZ;
      }
    }

    // Return the result found or theDefault assigned above
    return anResult;
  }
} // namespace GQE

/**
 * Copyright (c) 2010-2012 Ryan Lindeman
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
