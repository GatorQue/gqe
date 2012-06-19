/**
 * Provides the TicTacToeApp class which implements the TicTacToe example game
 * for the GQE library.
 *
 * @file examples/demo/TicTacToeApp.cpp
 * @author Ryan Lindeman
 * @date 20110704 - Initial Release
 * @date 20120512 - Add new Init methods required by IApp base class
 */
#include "TicTacToeApp.hpp"
#include "GameState.hpp"

TicTacToeApp::TicTacToeApp(const std::string theTitle) :
  GQE::IApp(theTitle)
{
}

TicTacToeApp::~TicTacToeApp()
{
}

void TicTacToeApp::InitAssetHandlers(void)
{
  // No custom asset handlers needed or provided
}

void TicTacToeApp::InitScreenFactory(void)
{
  // Add Game State as the next active state
  mStateManager.AddActiveState(new(std::nothrow) GameState(*this));
}

void TicTacToeApp::HandleCleanup(void)
{
  // No custom cleanup needed
}