/**
 * Provides the StringLogger class that provides logging support to a string for
 * all GQE classes in the GQE namespace in the GQE core library.
 *
 * @file src/GQE/Core/loggers/StringLogger.cpp
 * @author Ryan Lindeman
 * @date 20110607 - Initial Release
 */

#include <GQE/Core/loggers/StringLogger.hpp>

namespace GQE
{
  StringLogger::StringLogger(int theExitCode) :
    ILogger(theExitCode)
  {
  }

  StringLogger::~StringLogger()
  {
  }

  std::string StringLogger::GetString(void)
  {
    if(IsActive())
    {
      return mStringStream.str();
    }
    else
    {
      return "";
    }
  }

  std::ostream& StringLogger::GetStream(void)
  {
    std::ostream* anResult = &gNullStream;
    if(IsActive())
    {
      anResult = &mStringStream;      
    }
    return *anResult;
  }

  std::ostream& StringLogger::GetStream(SeverityType theSeverity,
      const char* theSourceFile, int theSourceLine, int theExitCode)
  {
    std::ostream* anResult = &gNullStream;
    if(IsActive())
    {
      anResult = &mStringStream;
      WriteTag(mStringStream, theSeverity, theSourceFile, theSourceLine);
    }
    return *anResult;
  }

  void StringLogger::LogMessage(const char* theMessage)
  {
    if(IsActive())
    {
      mStringStream << theMessage << std::endl;
    }
  }

  void StringLogger::LogMessage(SeverityType theSeverity,
      const char* theSourceFile, int theSourceLine,
      const char* theMessage)
  {
    if(IsActive())
    {
      WriteTag(mStringStream, theSeverity, theSourceFile, theSourceLine);
      mStringStream << theMessage << std::endl;
    }
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
