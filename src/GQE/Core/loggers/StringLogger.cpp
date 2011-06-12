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
