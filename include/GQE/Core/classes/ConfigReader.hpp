/**
 * Provides the ConfigReader class in the GQE namespace which is responsible
 * for providing the reading of configuration files.
 *
 * @file include/GQE/Core/classes/ConfigReader.hpp
 * @author Ryan Lindeman
 * @date 20110101 - Initial Release
 * @date 20110108 - Added GetFloat and ParseFloat
 * @date 20110118 - Fixed compiler problems.
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110127 - Use new OS independent Uint/Int types
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Added boolean result to Read method for success
 * @date 20110218 - Change to system include style
 * @date 20110627 - Removed extra ; from namespace
 * @date 20110810 - Added copy constructor and assignment operator
 * @date 20110820 - Moved private Parse methods to StringUtil.hpp/cpp
 * @date 20110820 - Changed Read to LoadFromFile to match SFML style
 * @date 20110820 - Removed GetColor, use GetString and ParseColor instead
 */
#ifndef   CORE_CONFIG_READER_HPP_INCLUDED
#define   CORE_CONFIG_READER_HPP_INCLUDED

#include <map>
#include <string>
#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// Provides basic configuration file reading
  class GQE_API ConfigReader
  {
    public:
      /**
       * ConfigReader constructor
       */
      ConfigReader();

      /**
       * ConfigReader copy constructor will duplicate the configuration
       * information that has been read from theCopy.
       * @param[in] theCopy to duplicate
       */
      ConfigReader(const ConfigReader& theCopy);

      /**
       * ConfigReader deconstructor
       */
      virtual ~ConfigReader();

      /**
       * IsSectionEmpty determines if theSection provided exists and has 1 or
       * more name, value pairs to retrieve.
       * @param[in] theSection to check
       * @return true if theSection provided exists
       */
      bool IsSectionEmpty(const std::string theSection) const;

      /**
       * GetBool will return the boolean value for theSection and theName
       * specified or theDefault(false) if the section or name does not
       * exist.  The value must be one of the following to return either
       * true or false (0,1,on,off,true,false).
       * @param[in] theSection to use for finding the value to return
       * @param[in] theName to use for finding the value to return
       * @param[in] theDefault to use if the value is not found (optional)
       * @return the value found or theDefault if not found or correct
       */
      bool GetBool(const std::string theSection, const std::string theName,
          const bool theDefault = false) const;

      /**
       * GetFloat will return a floating point number for theSection and
       * theName specified or theDefault(0.f) if theSection or theName does not
       * exist.
       * @param[in] theSection to use for finding theName
       * @param[in] theName to use for finding the value to return
       * @param[in] theDefault to use if the value is not found (optional)
       * @return the value found or theDefault if not found or correct
       */
      float GetFloat(const std::string theSection, const std::string theName,
          const float theDefault = 0.f) const;

      /**
       * GetString will return the string value for theSection and theName
       * specified or theDefault("") if the section or name does not exist.
       * @param[in] theSection to use for finding theName
       * @param[in] theName to use for finding the value to return
       * @param[in] theDefault to use if the value is not found (optional)
       * @return the value found or theDefault if not found
       */
      std::string GetString(const std::string theSection,
          const std::string theName, const std::string theDefault = "") const;

      /**
       * GetUint32 will return an unsigned 32 bit number for theSection and
       * theName specified or theDefault(0) if theSection or theName does not
       * exist.
       * @param[in] theSection to use for finding theName
       * @param[in] theName to use for finding the value to return
       * @param[in] theDefault to use if the value is not found (optional)
       * @return the value found or theDefault if not found
       */
      Uint32 GetUint32(const std::string theSection, const std::string theName,
          const Uint32 theDefault = 0) const;

      /**
       * LoadFromFile will open and read the configuration file specified into
       * internal maps that can be later retrieved using the Get* options above.
       * @param[in] theFilename to use as the configuration file to read
       * @result true if theFilename was found and opened successfully
       */
      bool LoadFromFile(const std::string theFilename);

      /**
       * Assignment operator will duplicate the information found in theRight
       * into this ConfigReader class.
       * @param[in] theRight hand side of = operator to duplicate from
       * @return the new left hand side copy
       */
      ConfigReader& operator=(const ConfigReader& theRight);

    private:
      // Constants
      ///////////////////////////////////////////////////////////////////////////
      static const unsigned short MAX_CHARS = 100;

      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Map to store all the sections and their corresponding name=value pairs
      std::map<const std::string, typeNameValue*> mSections;

      /**
       * ParseLine will parse the line provided by the Read method above.
       * @param theLine to be parsed.
       * @param theCount is the line number currently being processed
       * @param theSection to be used if name, value pair is parsed
       * @return string representing the new section name if section name was parsed
       */
      std::string ParseLine(const char* theLine, const unsigned long theCount,
          const std::string theSection);

      /**
       * StoreNameValue will store theName and theValue pair into theSection.
       * If theSection name value map doesn't yet exist, then it will be created.
       * @param theSection is used to find the name, value pair map
       * @param theName to be stored as the key for theValue below
       * @param theValue to be stored in the current section name
       */
      void StoreNameValue(const std::string theSection,
          const std::string theName, const std::string theValue);

  }; // class ConfigReader
} // namespace GQE

#endif // CORE_CONFIG_READER_HPP_INCLUDED

/**
 * @class GQE::ConfigReader
 * @ingroup Core
 * The ConfigReader class is used to read from configuration files those
 * settings needed for the program.  For now these files are basic .INI
 * type files.
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
