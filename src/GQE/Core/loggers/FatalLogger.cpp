/**
 * Provides the FatalLogger class that provides logging of Fatal messages (see
 * FLOG, FLOGM, FLOG_IF, FLOGM_IF) used throughout the GQE namespace and the
 * GQE core library.
 *
 * @file src/GQE/Core/loggers/FatalLogger.cpp
 * @author Ryan Lindeman
 * @date 20110610 - Initial Release
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
    if(GQE::gLogger)
    {
      // Log the Fatal message string stream created
      GQE::gLogger->LogMessage(GetString().c_str());
    }

    // Call the FatalShutdown method defined in ILogger
    FatalShutdown();
  }
} // namespace GQE
