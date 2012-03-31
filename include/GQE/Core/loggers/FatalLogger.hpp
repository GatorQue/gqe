/**
 * Provides the FatalLogger class that provides logging of Fatal messages (see
 * FLOG, FLOGM, FLOG_IF, FLOGM_IF) used throughout the GQE namespace and the
 * GQE core library.
 *
 * @file include/GQE/Core/loggers/FatalLogger.hpp
 * @author Ryan Lindeman
 * @date 20110610 - Initial Release
 */
#ifndef   CORE_FATAL_LOGGER_HPP_INCLUDED
#define   CORE_FATAL_LOGGER_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/loggers/StringLogger.hpp>

namespace GQE
{
  /// Provides the class used by the Fatal logging macros
  class GQE_API FatalLogger : public StringLogger
  {
    public:

      /**
       * FatalLogger constructor will write a tag line to the string log using
       * the information provided.
       * @param[in] theSeverity for this message to be logged
       * @param[in] theSourceFile where the Log macro was called from
       * @param[in] theSourceLine number where the Log macro was called from
       * @param[in] theExitCode value to use when exiting the application
       */
      FatalLogger(SeverityType theSeverity,
          const char* theSourceFile, int theSourceLine, int theExitCode);

      /**
       * FatalLogger deconstructor
       */
      virtual ~FatalLogger();

    protected:

    private:
  }; // class FatalLogger
} // namespace GQE

#endif // CORE_FATAL_LOGGER_HPP_INCLUDED
/**
 * @class GQE::FatalLogger
 * @ingroup Core
 * The FatalLogger class provides logging of all fatal log messages.
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
