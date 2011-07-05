/**
 * Provides the TicTacToeApp class which implements the TicTacToe example game
 * for the GQE library.
 *
 * @file examples/demo/TicTacToeApp.hpp
 * @author Ryan Lindeman
 * @date 20110704 - Initial Release
 */
#ifndef   TIC_TAC_TOE_APP_HPP_INCLUDED
#define   TIC_TAC_TOE_APP_HPP_INCLUDED
 
#include <GQE/Core/classes/App.hpp>
 
/// Provides the core game loop algorithm for all game engines.
class TicTacToeApp : public GQE::App
{
public:
  /**
   * TicTacToeApp constructor
   * @param[in] theTitle is the title of the window
   */
  TicTacToeApp(const std::string theTitle = "TicTacToe");

  /**
   * TicTacToeApp deconstructor
   */
  virtual ~TicTacToeApp();

protected:
  /**
   * Init is responsible for initializing the Application.
   */
  virtual void Init(void);

private:
}; // class TicTacToeApp

#endif // TIC_TAC_TOE_APP_HPP_INCLUDED
/**
 * @class TicTacToeApp
 * @ingroup Examples
 * The TicTacToeApp class is the App class for the TicTacToe example game
 * in the GQE library.
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
