/**
 * Provides the ScopeLogger class that provides logging the Entry and Exit of
 * the scope where this class is defined using the information provided at
 * construction time.  The Exit scope will be logged with the line number of
 * the where the ScopeLogger class was defined.
 *
 * @file include/GQE/Core/loggers/ScopeLogger.hpp
 * @author Ryan Lindeman
 * @date 20110610 - Initial Release
 * @date 20121107 - Padding ScopeLogger class
 */
#ifndef   CORE_SCOPE_LOGGER_HPP_INCLUDED
#define   CORE_SCOPE_LOGGER_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/interfaces/ILogger.hpp>

namespace GQE
{
  /// Provides a scope Entry/Result/Exit logging class
  class GQE_API ScopeLogger : public ILogger
  {
    public:

      /**
       * ScopeLogger constructor will log the Entry of this scope to the global
       * logger using the information provided and keep this information around
       * for logging the Exit of the scope (see ~ScopeLogger).
       * @param[in] theSeverity for this message to be logged
       * @param[in] theSourceFile where the Log macro was called from
       * @param[in] theSourceLine number where the Log macro was called from
       * @param[in] theScopeName to use to define this scope to log
       * @param[in] theExitCode value to use when FatalShutdown is called
       */
      ScopeLogger(SeverityType theSeverity,
          const char* theSourceFile, int theSourceLine,
          const char* theScopeName = "", int theExitCode = StatusError);

      /**
       * ScopeLogger deconstructor
       */
      virtual ~ScopeLogger();

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
      /// The severity level defined at construction time or by the last call to
      /// the GetStream() or LogMessage() methods (see SLOGR).
      SeverityType mSeverity;
      /// The source line defined at construction time or by the last call to the
      /// GetStream() or LogMessage() methods (see SLOGR).
      int mSourceLine;
      /// The source file defined at construction time or by the last call to the
      /// GetStream() or LogMessage() methods (see SLOGR).
      const char* mSourceFile;
      /// The scope name to use when logging Result (see SLOGR) or Exit messages
      const char* mScopeName;
  }; // class ScopeLogger
} // namespace GQE

#endif // CORE_SCOPE_LOGGER_HPP_INCLUDED
/**
 * @class GQE::ScopeLogger
 * @ingroup Core
 * The ScopeLogger class provides logging the Entry and Exit of the scope where
 * this class was defined.
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
