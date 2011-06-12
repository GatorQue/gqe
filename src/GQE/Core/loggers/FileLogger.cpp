/**
 * Provides the FileLogger class that provides logging support to a file for
 * all GQE classes in the GQE namespace in the GQE core library.
 *
 * @file src/GQE/Core/loggers/FileLogger.cpp
 * @author Ryan Lindeman
 * @date 20110524 - Initial Release
 */
 
#include <GQE/Core/loggers/FileLogger.hpp>
 
namespace GQE
{
  FileLogger::FileLogger(const char* theFilename)
  {
    mFileStream.open(theFilename);
  }  

  FileLogger::~FileLogger()
  {
    if(mFileStream.is_open())
    {
      mFileStream.close();
    }
  }

  std::ostream& FileLogger::GetStream(void)
  {
    std::ostream* anResult = &gNullStream;
    if(mFileStream.is_open() && IsActive())
    {
      anResult = &mFileStream;      
    }
    return *anResult;
  }

  std::ostream& FileLogger::GetStream(SeverityType theSeverity,
      const char* theSourceFile, int theSourceLine, int theExitCode)
  {
    std::ostream* anResult = &gNullStream;
    if(mFileStream.is_open() && IsActive())
    {
      anResult = &mFileStream;      
      WriteTag(mFileStream, theSeverity, theSourceFile, theSourceLine);
    }
    return *anResult;
  }

  void FileLogger::LogMessage(const char* theMessage)
  {
    if(mFileStream.is_open() && IsActive())
    {
      mFileStream << theMessage << std::endl;
    }
  }

  void FileLogger::LogMessage(SeverityType theSeverity,
      const char* theSourceFile, int theSourceLine,
      const char* theMessage)
  {
    if(mFileStream.is_open() && IsActive())
    {
      WriteTag(mFileStream, theSeverity, theSourceFile, theSourceLine);
      mFileStream << theMessage << std::endl;
    }
  }

} // namespace GQE
