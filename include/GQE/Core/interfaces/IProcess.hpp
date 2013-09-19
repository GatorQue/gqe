/**
 * Provides a class that manages a single process thread that can be used to
 * create a class that runs in its own thread.
 *
 * @file include/GQE/Core/interfaces/IProcess.hpp
 * @author Ryan Lindeman
 * @date 20121124 - Initial Release
 * @date 20130112 - Fix warnings in VS2010 using this pointer by inheriting for SFML v1.6
 * @date 20130208 - Fix warnings in VS2010 using this pointer for SFML v2.0
 */
#ifndef IPROCESS_HPP_INCLUDED
#define IPROCESS_HPP_INCLUDED

#include <SFML/System.hpp>
#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// Provides the IProcess interface class to run in its own thread
  class GQE_API IProcess
#if (SFML_VERSION_MAJOR < 2)
     : private sf::Thread
#endif
  {
    public:
      /**
       * IProcess deconstructor
       */
      virtual ~IProcess();

      /**
       * IsRunning will return true if the process thread is currently
       * active and running.
       */
      bool IsRunning(void) const;

      /**
       * Start will begin the process thread if it is not already running
       * currently.
       */
      void Start(void);

      /**
       * Stop will stop the process thread by setting mRunning to false and
       * waiting for the process thread to exit gracefully (caller will be
       * blocked).
       */
      void Stop(void);

    protected:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Running flag
      bool mRunning;

      /**
       * IsReady will be called to determine if the Process has been
       * sufficiently configured to be able to start the thread. If this method
       * returns false then the caller has missed some key step before calling
       * the Start method. The derived class can override the default
       * implementation of returning true to define what should be checked.
       * @return true if process thread is ready to start, false otherwise
       */
      virtual bool IsReady(void);

      /**
       * OnStop will be called during the Stop method and gives the derived
       * class a chance to abort any blocking class so the Process thread can
       * exit gracefully. The default implementation does nothing.
       */
      virtual void OnStop(void);

      /**
       * Process method that will run in its own thread when the Start method
       * is called, the default implementation will cause the process thread
       * to immediately exit. The caller should set the mRunning variable to
       * false as the Process loop is exiting or call the parent Process
       * method on exit.
       */
      virtual void Process(void);

      /**
       * IProcess default constructor, only allow derived classes to create
       * instances.
       */
      IProcess();

    private:
#if (SFML_VERSION_MAJOR < 2)
      /**
       * Run will be called by our inherited sf::Thread class when the thread
       * is running (SFML v1.6).
       */
      virtual void Run(void);
#else
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Thread object for starting/stopping thread
      sf::Thread* mThread;
#endif
  }; // IProcess class
} // namespace GQE
#endif // IPROCESS_HPP_INCLUDED

/**
 * @class GQE::IProcess
 * @ingroup Core
 * The IProcess interface class is the common base class for all indepent
 * process threads throughout the GQE library. By providing a common base class
 * a pool of similar threads can be collected together and managed better.
 *
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
