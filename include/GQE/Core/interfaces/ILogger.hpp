/**
 * Provides the ILogger class in the GQE namespace which is responsible for
 * providing the ILogger interface used by all loggers in the GQE core library.
 *
 * @file include/GQE/Core/interfaces/ILogger.hpp
 * @author Ryan Lindeman
 * @date 20110524 - Initial Release
 * @date 20110627 - Added docs and remove extra ;
 * @date 20110801 - Moved code to .cpp file due to circular dependencies
 * @date 20120426 - Change to ILogger::GetLogger call instead of gLogger
 */
#ifndef   CORE_ILOGGER_HPP_INCLUDED
#define   CORE_ILOGGER_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// Provides the base class interface for all Logger classes
  class GQE_API ILogger
  {
    public:
      /**
       * ILogger deconstructor
       */
      virtual ~ILogger();

      /**
       * GetInstance will return the most recent ILogger based class that was created
       * so it can be used to log messages or NULL if none has been created yet.
       * @return pointer to ILogger based class or NULL if none is available
       */
      static ILogger* GetLogger(void);

      /**
       * IsActive will return true if this logger is currently active
       * @return true if logger is active, false otherwise
       */
      bool IsActive(void);

      /**
       * SetActive is responsible for setting the active state for this
       * logger.
       * @param[in] theActive value to set this logger to
       */
      virtual void SetActive(bool theActive);

      /**
       * GetStream is responsible for returning the ostream necessary to log the
       * custom message that will follow without a prefix.
       * @return the ostream to use for logging the message
       */
      virtual std::ostream& GetStream(void) = 0;

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
          const char* theSourceFile, int theSourceLine, int theExitCode = StatusError) = 0;

      /**
       * LogMessage is responsible for logging the message provided using an
       * appropriate timestamp and File:Line tag in front.
       * @param[in] theMessage to log
       */
      virtual void LogMessage(const char* theMessage) = 0;

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
          const char* theMessage) = 0;

    protected:
      /**
       * ILogger constructor is protected because we do not allow copies of our
       * Singleton class except to those who derive from us.
       * @param[in] theExitCode to use if FatalShutdown is called
       */
      ILogger(int theExitCode = StatusError);

      /**
       * WriteTag will write an appropriate timestamp and File:Line tag in the
       * ostream provided.  The caller should make sure to check IsActive()
       * before calling this method to prevent the tag from being written to
       * the provided stream when the logger is not active.
       * @param[in,out] theOstream to use to write the prefix tag
       * @param[in] theSeverity for this message to be logged
       * @param[in] theSourceFile where the Log macro was called from
       * @param[in] theSourceLine number where the Log macro was called from
       */
      void WriteTag(std::ostream& theOstream, SeverityType theSeverity,
          const char* theSourceFile, int theSourceLine);

      /**
       * FatalShutdown is called when logging a Fatal severity after the message
       * has been logged and will handle gracefully exiting the application by
       * using the global gApp variable to call the applications Quit method.
       */
      void FatalShutdown(void);

    private:
      /// Instance variable assigned at construction time
      static ILogger* gInstance;

      /// Logger is currently active
      bool  mActive;
      /// The Exit value to use when FatalShutdown method is called
      int mExitCode;

      /**
       * Our copy constructor is private because we do not allow copies of
       * our Singleton class
       */
      ILogger(const ILogger&);  // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our Singleton class
       */
      ILogger& operator=(const ILogger&); // Intentionally undefined

  }; // class ILogger
} // namespace GQE

#endif // CORE_ILOGGER_HPP_INCLUDED

/**
 * @class GQE::ILogger
 * @ingroup Core
 * The ILogger class is at the heart of all logging classes in GQ Engines.
 * Its' purpose is to provide logging support during and after development of
 * any game based on the GQ Engines framework.
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
