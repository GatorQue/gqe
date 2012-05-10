/**
 * Provides the SpaceDotsApp class which implements the SpaceDots example game
 * for the GQE library.
 *
 * @file src/SpaceDotsApp.cpp
 * @author Ryan Lindeman
 * @date 20120323 - Initial Release
 */
#include "SpaceDotsApp.hpp"
#include "GameState.hpp"
#include <GQE/Core/states/SplashState.hpp>

SpaceDotsApp::SpaceDotsApp(const std::string theTitle) :
  GQE::App(theTitle)
{
}

SpaceDotsApp::~SpaceDotsApp()
{
}

void SpaceDotsApp::Init(void)
{
  // Add Menu State as the next active state
  mStateManager.AddActiveState(new(std::nothrow) GameState(*this));
  mStateManager.AddActiveState(new(std::nothrow) GQE::SplashState(*this, "Splash"));
}
