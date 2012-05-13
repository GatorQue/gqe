/**
 * Provides the FatalLogger class that provides logging of Fatal messages (see
 * FLOG, FLOGM, FLOG_IF, FLOGM_IF) used throughout the GQE namespace and the
 * GQE core library.
 *
 * @file src/GQE/Core/loggers/FatalLogger.cpp
 * @author Ryan Lindeman
 * @date 20110610 - Initial Release
 * @date 20120426 - Change to ILogger::GetLogger call instead of gLogger
 */

#include <GQE/Core/loggers/FatalLogger.hpp>

namespace GQE
{
  FatalLogger::FatalLogger(SeverityType theSeverity,
      const char* theSourceFile, int theSourceLine, int theExitCode) :
    StringLogger(theExitCode)
  {
    // Create a tag for this fatal log message in our string stream
    WriteTag(GetStream(), theSeverity, theSourceFile, theSourceLine);
  }

  FatalLogger::~FatalLogger()
  {
    // Make sure our global logger is defined before using it
    if(GQE::ILogger::GetLogger())
    {
      // Log the Fatal message string stream created
      GQE::ILogger::GetLogger()->LogMessage(GetString().c_str());
    }

    // Call the FatalShutdown method defined in ILogger
    FatalShutdown();
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
