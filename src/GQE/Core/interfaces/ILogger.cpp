/**
 * Provides the ILogger class in the GQE namespace which is responsible for
 * providing the ILogger interface used by all loggers in the GQE core library.
 *
 * @file src/GQE/Core/interfaces/ILogger.cpp
 * @author Ryan Lindeman
 * @date 20110801 - Initial Release
 * @date 20120426 - Change to ILogger::GetLogger call instead of gLogger
 */
#include <ctime>
#include <ostream>

#include <GQE/Core/interfaces/ILogger.hpp>
#include <GQE/Core/classes/App.hpp>

namespace GQE
{

  /// Single instance of the most recently created ILogger class
  ILogger* ILogger::gInstance = NULL;

  ILogger::ILogger(int theExitCode) :
    mActive(false),
    mExitCode(StatusError)
  {
    // Make note of the current instance of the ILogger interface
    gInstance = this;

    SetActive(true);
  }

  ILogger::~ILogger()
  {
    if(mActive)
    {
      SetActive(false);
    }
  }

  ILogger* ILogger::GetLogger(void)
  {
    return gInstance;
  }

  bool ILogger::IsActive(void)
  {
    return mActive;
  }

  void ILogger::SetActive(bool theActive)
  {
    mActive = theActive;
  }


  void ILogger::WriteTag(std::ostream& theOstream, SeverityType theSeverity,
      const char* theSourceFile, int theSourceLine)
  {
    const struct std::tm* anTm; // Pointer to Time structure
    std::time_t anNow;

    // Get current time in seconds since Jan 1, 1970
    anNow = std::time(NULL);

    // Convert current time value into local time (with help from OS)
    anTm = std::localtime(&anNow);

    // Output time in timestamp format
    theOstream << anTm->tm_year + 1900 << "-";
    theOstream.fill('0');
    theOstream.width(2);
    theOstream << anTm->tm_mon + 1 << "-";
    theOstream.fill('0');
    theOstream.width(2);
    theOstream << anTm->tm_mday << " ";
    theOstream.fill('0');
    theOstream.width(2);
    theOstream << anTm->tm_hour + 1 << ":";
    theOstream.fill('0');
    theOstream.width(2);
    theOstream << anTm->tm_min << ":";
    theOstream.fill('0');
    theOstream.width(2);
    theOstream << anTm->tm_sec << " ";

    // Now print the log level as a single character
    switch(theSeverity)
    {
      case SeverityInfo:
        theOstream << "I";
        break;
      case SeverityWarning:
        theOstream << "W";
        break;
      case SeverityError:
        theOstream << "E";
        break;
      case SeverityFatal:
        theOstream << "F";
        break;
      default:
        theOstream << "U";
        break;
    }
    theOstream << " " << theSourceFile << ":" << theSourceLine << " ";
  }

  void ILogger::FatalShutdown(void)
  {
    // Does the gApp pointer exist? then call the Quit method
    if(NULL != GQE::gApp)
    {
      // Use the gApp pointer to call the Quit method and indicate an error
      // has occurred and the application needs to quit soon.
      gApp->Quit(mExitCode);
    }
    else
    {
      // In the event that the gApp pointer doesn't exist, perform a standard
      // exit() call to force the program to terminate.
      exit(mExitCode);
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
