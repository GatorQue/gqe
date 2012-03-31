/**
 * Provides the FileLogger class that provides logging support to a file for
 * all GQE classes in the GQE namespace in the GQE core library.
 *
 * @file include/GQE/Core/loggers/FileLogger.hpp
 * @author Ryan Lindeman
 * @date 20110524 - Initial Release
 */
#ifndef   CORE_FILE_LOGGER_HPP_INCLUDED
#define   CORE_FILE_LOGGER_HPP_INCLUDED

#include <fstream>
#include <string.h>
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/interfaces/ILogger.hpp>

namespace GQE
{
  /// Provides file logging capabilities
  class GQE_API FileLogger : public ILogger
  {
    public:

      /**
       * FileLogger constructor
       * @param[in] theFilename to open as log file
       */
      FileLogger(const char* theFilename);

      /**
       * FileLogger deconstructor
       */
      virtual ~FileLogger();

      /**
       * GetStream is responsible for returning the ostream necessary to log the
       * custom message that will follow without a prefix.
       * @return the ostream to use for logging the message
       */
      virtual std::ostream& GetStream(void);

      /**
       * GetStream is responsible for returning the ostream necessary to log the
       * custom message that will follow and prefix the custom message with an
       * appropriate timestamp and File:Line tag.
       * @param[in] theSeverity for this message to be logged
       * @param[in] theSourceFile where the Log macro was called from
       * @param[in] theSourceLine number where the Log macro was called from
       * @param[in] theExitCode value to use when FatalShutdown is called
       * @return the ostream to use for logging the message
       */
      virtual std::ostream& GetStream(SeverityType theSeverity,
          const char* theSourceFile, int theSourceLine, int theExitCode = StatusError);

      /**
       * LogMessage is responsible for logging the message provided using an
       * appropriate timestamp and File:Line tag in front.
       * @param[in] theMessage to log
       */
      virtual void LogMessage(const char* theMessage);

      /**
       * LogMessage is responsible for logging the message provided using an
       * appropriate timestamp and File:Line tag in front.
       * @param[in] theSeverity for this message to be logged
       * @param[in] theSourceFile where the Log macro was called from
       * @param[in] theSourceLine number where the Log macro was called from
       * @param[in] theMessage to log
       */
      virtual void LogMessage(SeverityType theSeverity,
          const char* theSourceFile, int theSourceLine,
          const char* theMessage);

    protected:      

    private:
      /// Output Logger file
      std::ofstream     mFileStream;

      /**
       * Copy constructor is private because we do not allow copies of
       * our Singleton class
       */
      FileLogger(const FileLogger&);             // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our Singleton class
       */
      FileLogger& operator=(const FileLogger&);  // Intentionally undefined
  }; // class FileLogger
} // namespace GQE

#endif // CORE_FILE_LOGGER_HPP_INCLUDED
/**
 * @class GQE::FileLogger
 * @ingroup Core
 * The FileLogger class provides a useful file logging facility for any game
 * engine that needs file logging for debug purposes.
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
