/**
 * This is the starting point for all new projects.  This file's purpose is
 * pretty small, but important.  In here we create our application and begin
 * the primary game loop.
 *
 * @file main.cpp
 * @author Ryan Lindeman
 * @date 20100707 - Initial Release
 */

#include <assert.h>
#include <stddef.h>
#include <GQE/Core.hpp>
#include "TicTacToeApp.hpp"

int main(int argc, char* argv[])
{
  // Default anExitCode to a specific value
  int anExitCode = GQE::StatusNoError;
 
  // Create our Logger first before creating our application
  GQE::gLogger = new(std::nothrow) GQE::FileLogger("output.txt");

  // Create our action application.
  GQE::App* anApp = new(std::nothrow) TicTacToeApp();
  assert(NULL != anApp && "main() Can't create Application");
 
  // Process command line arguments
  anApp->ProcessArguments(argc, argv);
 
  // Start the action application:
  // Initialize the action application
  // Enter the Game Loop where the application will remain until it is shutdown
  // Cleanup the action application
  // Exit back to here
  anExitCode = anApp->Run();
 
  // Cleanup ourselves by deleting the action application
  delete anApp;
 
  // Don't keep pointers to objects we have just deleted
  anApp = NULL;
 
  // Delete our Logger last before exiting
  delete GQE::gLogger;

  // Don't keep pointers to objects we have just deleted
  GQE::gLogger = NULL;

  // return our exit code
  return anExitCode;
}

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
