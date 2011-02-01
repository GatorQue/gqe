/**
 * Provides the ConfigReader class in the GQE namespace which is responsible
 * for providing the reading of configuration files.
 *
 * @file src/GQE/Core/classes/ConfigReader.cpp
 * @author Ryan Lindeman
 * @date 20110101 - Initial Release
 * @date 20110125 - Fix string compare problems
 * @date 20110127 - Moved to GQE Core library and src directory
 * @date 20110127 - Use new OS independent Uint/Int types
 * @date 20110128 - Fixed erase call in the DeleteXYZ methods.
 */
 
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <sstream>
#include "GQE/Core/classes/ConfigReader.hpp"
#include "GQE/Core/classes/App.hpp"
 
namespace GQE
{
  ConfigReader::ConfigReader() :
    mApp(NULL)
  {
  }
 
  ConfigReader::~ConfigReader()
  {
    // Output to log file
    if(NULL != mApp)
    {
      mApp->mLog << "ConfigReader::~ConfigReader() dtor called" << std::endl;
    }
 
    // Delete all section name, value maps
    std::map<const std::string, typeNameValue*>::iterator iter;
    iter = mSections.begin();
    while(iter != mSections.end())
    {
      typeNameValue* anMap = iter->second;
      mSections.erase(iter++);
      delete anMap;
    }
 
    // Clear pointers we don't need anymore
    mApp = NULL;
  }
 
  void ConfigReader::RegisterApp(App* theApp)
  {
    // Check that our pointer is good
    assert(NULL != theApp && "ConfigReader::RegisterApp() theApp pointer provided is bad");
 
    // Make a note of the pointer
    assert(NULL == mApp && "ConfigReader::RegisterApp() theApp pointer was already registered");
    mApp = theApp;
  }
 
  bool ConfigReader::IsSectionEmpty(const std::string theSection)
  {
    bool anResult = false;
 
    // Check if theSection really exists
    std::map<const std::string, typeNameValue*>::iterator iter;
    iter = mSections.find(theSection);
    if(iter != mSections.end())
    {
      typeNameValue* anMap = iter->second;
      if(NULL != anMap)
      {
        anResult = anMap->empty();
      }
    }
 
    // Return the result found above or the default value of false
    return anResult;
  }
 
  bool ConfigReader::GetBool(const std::string theSection,
    const std::string theName, const bool theDefault)
  {
    bool anResult = theDefault;
 
    // Check if theSection really exists
    std::map<const std::string, typeNameValue*>::iterator iter;
    iter = mSections.find(theSection);
    if(iter != mSections.end())
    {
      // Try to obtain the name, value pair
      typeNameValue* anMap = iter->second;
      if(NULL != anMap)
      {
        typeNameValueIter iterNameValue;
        iterNameValue = anMap->find(theName);
        if(iterNameValue != anMap->end())
        {
          anResult = ParseBool(iterNameValue->second, theDefault);
        }
      }
    }
 
    // Return the result found or theDefault assigned above
    return anResult;
  }
 
  std::string ConfigReader::GetString(const std::string theSection,
    const std::string theName, const std::string theDefault)
  {
    std::string anResult = theDefault;
 
    // Check if theSection really exists
    std::map<const std::string, typeNameValue*>::iterator iter;
    iter = mSections.find(theSection);
    if(iter != mSections.end())
    {
      // Try to obtain the name, value pair
      typeNameValue* anMap = iter->second;
      if(NULL != anMap)
      {
        typeNameValueIter iterNameValue;
        iterNameValue = anMap->find(theName);
        if(iterNameValue != anMap->end())
        {
          anResult = iterNameValue->second;
        }
      }
    }
 
    // Return the result found or theDefault assigned above
    return anResult;
  }
 
  Uint32 ConfigReader::GetUint32(const std::string theSection,
    const std::string theName, const Uint32 theDefault)
  {
    Uint32 anResult = theDefault;
 
    // Check if theSection really exists
    std::map<const std::string, typeNameValue*>::iterator iter;
    iter = mSections.find(theSection);
    if(iter != mSections.end())
    {
      // Try to obtain the name, value pair
      typeNameValue* anMap = iter->second;
      if(NULL != anMap)
      {
        typeNameValueIter iterNameValue;
        iterNameValue = anMap->find(theName);
        if(iterNameValue != anMap->end())
        {
          anResult = ParseUint32(iterNameValue->second, theDefault);
        }
      }
    }
 
    // Return the result found or theDefault assigned above
    return anResult;
  }
 
  sf::Color ConfigReader::GetColor(const std::string theSection,
    const std::string theName, const sf::Color theDefault)
  {
    sf::Color anResult = theDefault;
 
    // Check if theSection really exists
    std::map<const std::string, typeNameValue*>::iterator iter;
    iter = mSections.find(theSection);
    if(iter != mSections.end())
    {
      // Try to obtain the name, value pair
      typeNameValue* anMap = iter->second;
      if(NULL != anMap)
      {
        typeNameValueIter iterNameValue;
        iterNameValue = anMap->find(theName);
        if(iterNameValue != anMap->end())
        {
          anResult = ParseColor(iterNameValue->second, theDefault);
        }
      }
    }
 
    // Return the result found or theDefault assigned above
    return anResult;
  }
 
  void ConfigReader::Read(const std::string theFilename)
  {
    char anLine[MAX_CHARS];
    std::string anSection;
    unsigned long anCount = 1;
 
    // Let the log know about the file we are about to read in
    if(NULL != mApp)
    {
      mApp->mLog << "ConfigReader::Read() opening file " << theFilename << std::endl;
    }
 
    // Attempt to open the file
    FILE* anFile = fopen(theFilename.c_str(), "r");
 
    // Read from the file if successful
    if(NULL != anFile)
    {
      // Keep reading from configuration file until we reach the end of file marker
      while(!feof(anFile))
      {
        // Get the first line from the file
        if(fgets(anLine, MAX_CHARS, anFile) == NULL)
        {
          // Log the failure to read a line from the file if not at the end of the file
          if(NULL != mApp && !feof(anFile))
          {
            mApp->mLog << "ConfigReader::Read() error reading line " << anCount << " from file " << theFilename << std::endl;
          }
          // Exit our while loop, were done!
          break;
        }
        else
        {
          // Parse the line
          anSection = ParseLine(anLine, anCount, anSection);
        }
 
        // Increment our Line counter
        anCount++;
      }
 
      // Don't forget to close the file
      fclose(anFile);
    }
    else
    {
      // Log the failure to open the file
      if(NULL != mApp)
      {
        mApp->mLog << "ConfigReader::Read() error opening file " << theFilename << std::endl;
      }
    }
  }
 
  bool ConfigReader::ParseBool(const std::string theValue,
    const bool theDefault)
  {
    bool anResult = theDefault;
    // Look for true results
    if(GQE_STRICMP(theValue.c_str(),"true") == 0 ||
       GQE_STRICMP(theValue.c_str(),"1") == 0 ||
       GQE_STRICMP(theValue.c_str(),"on") == 0)
       anResult = true;
    // Look for false results
    if(GQE_STRICMP(theValue.c_str(),"false") == 0 ||
       GQE_STRICMP(theValue.c_str(),"0") == 0 ||
       GQE_STRICMP(theValue.c_str(),"off") == 0)
       anResult = false;
 
    // Return the result found or theDefault assigned above
    return anResult;
  }
 
  sf::Color ConfigReader::ParseColor(const std::string theValue,
    const sf::Color theDefault)
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
 
  Uint32 ConfigReader::ParseUint32(const std::string theValue,
      const Uint32 theDefault)
  {
    Uint32 anResult = theDefault;
    std::istringstream iss(theValue);
 
    // Convert the string to an unsigned 32 bit integer
    iss >> anResult;
 
    // Return the result found or theDefault assigned above
    return anResult;
  }
 
  std::string ConfigReader::ParseLine(const char* theLine,
    const unsigned long theCount, const std::string theSection)
  {
    std::string anResult = theSection;
    size_t anLength = strlen(theLine);
    if(anLength > 0)
    {
      // Skip preceeding spaces at the begining of the line
      size_t anOffset = 0;
      while(anOffset < anLength && theLine[anOffset] == ' ')
      {
        anOffset++;
      }
 
      // Now check for comments
      if(theLine[anOffset] != '#' && theLine[anOffset] != ';')
      {
        // Next check for the start of a new section
        if(theLine[anOffset] == '[')
        {
          // Skip over the begin section marker '['
          anOffset++;
 
          // Skip preceeding spaces of section name
          while(anOffset < anLength && theLine[anOffset] == ' ')
          {
            anOffset++;
          }
 
          // Retrieve the section name while looking for the section end marker ']'
          size_t anIndex = 0;
          char anSection[MAX_CHARS] = {0};
          while((anOffset+anIndex) < anLength && theLine[anOffset+anIndex] != ']')
          {
            anSection[anIndex] = theLine[anOffset+anIndex++];
          }
          // Add null terminator
          anSection[anIndex] = '\0';
          // Remove trailing spaces
          while(anIndex > 0 && anSection[anIndex-1] == ' ')
          {
            // Put a null terminator at the end of the section name
            anSection[--anIndex] = '\0';
          }
 
          // Only update the current section name if we found the section end
          // marker before the end of the line
          if((anOffset+anIndex) < anLength && anIndex > 0)
          {
            // Change the return string to the newly parsed section name
            anResult = anSection;
          }
          else
          {
            // Log the failure to open the file
            if(NULL != mApp)
            {
              mApp->mLog << "ConfigReader::ParseLine() missing section end marker ']' on line "
                << theCount << std::endl;
            }
          }
        }
        // Just read the name=value pair into the current section
        else
        {
          // Retrieve the name and value while looking for the comment flags ';' or '#'
          size_t anNameIndex = 0;
          char anName[MAX_CHARS];
 
          // First retrieve the name while looking for either the '=' or ':' delimiter
          while((anOffset+anNameIndex) < anLength &&
            theLine[(anOffset+anNameIndex)] != '=' &&
            theLine[(anOffset+anNameIndex)] != ':')
          {
            // Grab the name
            anName[anNameIndex] = theLine[anOffset+anNameIndex++];
          }
          // Assign our starting offset value
          anOffset += anNameIndex;
          // Put a null terminator at the end of the name
          anName[anNameIndex] = '\0';
          // Remove trailing spaces from the name
          while(anNameIndex > 0 && anName[anNameIndex-1] == ' ')
          {
            // Put a null terminator at the end of the name
            anName[--anNameIndex] = '\0';
          }
 
          // Only search for the value if we found the '=' or ':' delimiter
          if(anOffset < anLength && anNameIndex > 0)
          {
            size_t anValueIndex = 0;
            char anValue[MAX_CHARS];
 
            // Skip over the delimiter between name and value '=' or ':'
            anOffset++;
 
            // Skip preceeding spaces
            while(anOffset < anLength && theLine[anOffset] == ' ')
            {
              anOffset++;
            }
            // Next retrieve the value while looking for comments flags ';' or '#'
            while((anOffset + anValueIndex) < anLength &&
              theLine[(anOffset+anValueIndex)] != '\r' &&
              theLine[(anOffset+anValueIndex)] != '\n' &&
              theLine[(anOffset+anValueIndex)] != ';' &&
              theLine[(anOffset+anValueIndex)] != '#')
            {
              // Grab the value
              anValue[anValueIndex] = theLine[anOffset+anValueIndex++];
            }
            // Put a null terminator at the end of the section name
            anValue[anValueIndex] = '\0';
            // Remove trailing spaces from the name
            while(anValueIndex > 0 && anValue[anValueIndex-1] == ' ')
            {
              // Put a null terminator at the end of the name
              anValue[--anValueIndex] = '\0';
            }
 
            // Store the name,value pair obtained into the current section
            StoreNameValue(theSection,anName,anValue);
          }
          else
          {
            // Log the failure to open the file
            if(NULL != mApp)
            {
              mApp->mLog << "ConfigReader::ParseLine() missing name or value delimiter of '=' or ':'"
                << " on line " << theCount << std::endl;
            }
          }
        }
      } // if(theLine[anOffset] != '#' && theLine[anOffset] != ';') // Not a comment
    } // if(anLength > 0)
 
    // Return either the previous section name or the new section name found
    return anResult;
  }
 
  void ConfigReader::StoreNameValue(const std::string theSection,
    const std::string theName, const std::string theValue)
  {
    // Check if the name, value map already exists for theSection
    std::map<const std::string, typeNameValue*>::iterator iterSection;
    iterSection = mSections.find(theSection);
    if(iterSection == mSections.end())
    {
      // First try to create a new name, value pair map for this new section
      typeNameValue* anMap = new (std::nothrow) typeNameValue;
 
      // Make sure we were able to create the map ok
      if(NULL != anMap)
      {
        // Log the failure to open the file
        if(NULL != mApp)
        {
          mApp->mLog << "ConfigReader::StoreNameValue() adding new section (" << theSection
            << ") the name, value pair (" << theName << "," << theValue << ")" << std::endl;
        }
 
        // Add the new name, value pair to this map
        anMap->insert(std::pair<const std::string, const std::string>(theName,theValue));
 
        // Add the new name, value pair map for this new section
        mSections.insert(std::pair<const std::string, typeNameValue*>(theSection, anMap));
      }
      else
      {
        // Log the failure to open the file
        if(NULL != mApp)
        {
          mApp->mLog << "ConfigReader::StoreNameValue() unable to create name, value map" << std::endl;
        }
      }
    }
    else
    {
      // Retrieve the existing name, value pair map
      typeNameValue* anMap = iterSection->second;
 
      // Make sure we were able to retrieve the map ok
      if(NULL != anMap)
      {
        // Make sure the name, value pair doesn't already in the map
        typeNameValueIter iterNameValue;
        iterNameValue = anMap->find(theName);
        if(iterNameValue == anMap->end())
        {
          // Log the failure to open the file
          if(NULL != mApp)
          {
            mApp->mLog << "ConfigReader::StoreNameValue() adding to existing section (" << theSection
              << ") the name, value pair (" << theName << "," << theValue << ")" << std::endl;
          }
 
          // Add the new name, value pair to this map
          anMap->insert(std::pair<const std::string, const std::string>(theName,theValue));
        }
        else
        {
          // Log the failure to open the file
          if(NULL != mApp)
          {
            mApp->mLog << "ConfigReader::StoreNameValue() name, value pair (" << theName
              << "," << theValue << ") already exists" << std::endl;
          }
        }
      }
    } // else(iterSection == mSections.end())
  }
 
}; // namespace GQE

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
