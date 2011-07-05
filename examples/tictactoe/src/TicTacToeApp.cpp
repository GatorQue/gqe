/**
 * Provides the TicTacToeApp class which implements the TicTacToe example game
 * for the GQE library.
 *
 * @file examples/demo/TicTacToeApp.cpp
 * @author Ryan Lindeman
 * @date 20110704 - Initial Release
 */
#include "TicTacToeApp.hpp"
#include "GameState.hpp"
 
TicTacToeApp::TicTacToeApp(const std::string theTitle) :
    GQE::App(theTitle)
{
}

TicTacToeApp::~TicTacToeApp()
{
}

void TicTacToeApp::Init(void)
{
  // Add Menu State as the next active state
  mStateManager.AddActiveState(new(std::nothrow) GameState(this));
}
