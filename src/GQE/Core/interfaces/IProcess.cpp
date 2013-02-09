/**
 * Provides a class that manages a single process thread that can be used to
 * create a class that runs in its own thread.
 *
 * @file src/GQE/Core/interfaces/IProcess.cpp
 * @author Ryan Lindeman
 * @date 20121124 - Initial Release
 * @date 20130112 - Fix warnings in VS2010 using this pointer by inheriting for SFML v1.6
 * @date 20130208 - Fix warnings in VS2010 using this pointer for SFML v2.0
 */

#include <GQE/Core/interfaces/IProcess.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
  IProcess::IProcess() :
    mRunning(false)
#if (SFML_VERSION_MAJOR < 2)
    // sf::Thread is inherited so Run method will be called using SFML v1.6
#else
    ,mThread(NULL)
#endif
  {
#if (SFML_VERSION_MAJOR < 2)
    // Nothing here for SFML 1.6
#else
    mThread = new(std::nothrow) sf::Thread(&IProcess::Process, this);
#endif
  }

  IProcess::~IProcess()
  {
    if(mRunning)
    {
      Stop();
    }
#if (SFML_VERSION_MAJOR < 2)
    // Nothing here for SFML 1.6
#else
    delete mThread;
#endif
  }

  bool IProcess::IsRunning(void) const
  {
    return mRunning;
  }

  void IProcess::Start(void)
  {
    if(!IsReady() && !mRunning)
    {
      // Output error message since thread is not ready to start
      ELOG() << "IProcess::Start thread not ready to start" << std::endl;
    }
    else if(!mRunning)
    {
      // Indicate the process thread is now running
      mRunning = true;

      // Launch the process thread now
#if (SFML_VERSION_MAJOR < 2)
      Launch();
#else
      mThread->launch();
#endif
    }
    else
    {
      // Output warning message since thread is already started
      WLOG() << "IProcess::Start thread already started" << std::endl;
    }
  }

  void IProcess::Stop(void)
  {
    if(mRunning)
    {
      // Indicate the process thread should stop running
      mRunning = false;

      // Give the derived class a chance to exit gracefully
      OnStop();

      // Wait for the process thread to gracefully exit
#if (SFML_VERSION_MAJOR < 2)
      Wait();
#else
      mThread->wait();
#endif
    }
    else
    {
      // Output warning message since thread is already started
      WLOG() << "IProcess::Thread is already stopped" << std::endl;
    }
  }

  bool IProcess::IsReady(void)
  {
    return true;
  }

  void IProcess::OnStop(void)
  {
    // Do nothing, allow derived class to define default behavior
  }

  void IProcess::Process(void)
  {
    mRunning = false;
  }

#if (SFML_VERSION_MAJOR < 2)
  void IProcess::Run(void)
  {
    // Call the virtual Process method in this thread context
    Process();
  }
#endif
} // namespace GQE

/**
 * Copyright (c) 2010-2012 Ryan Lindeman
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
