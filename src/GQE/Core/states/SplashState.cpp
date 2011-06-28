/**
 * Provides the SplashState in the GQE namespace which is typically
 * the first state to be loaded for an application.
 *
 * @file src/GQE/Core/states/SplashState.hpp
 * @author Ryan Lindeman
 * @date 20100710 - Initial Release
 * @date 20110125 - Use the new RemoveActiveState not DropActiveState.
 * @date 20110127 - Moved to GQE Core library and src directory
 * @date 20110218 - Added ReInit method
 * @date 20110218 - Change to system include style
 * @date 20110625 - Added UpdateVariable and changed Update to UpdateFixed
 * @date 20110627 - Removed extra ; from namespace
 */
#include <GQE/Core/assets/ImageAsset.hpp>
#include <GQE/Core/classes/App.hpp>
#include <GQE/Core/states/SplashState.hpp>

namespace GQE
{
  SplashState::SplashState(App* theApp) :
    IState("Splash",theApp),
    mSplashSprite(NULL)
  {
  }

  SplashState::~SplashState(void)
  {
  }

  void SplashState::HandleEvents(sf::Event theEvent)
  {
  }

  void SplashState::DoInit(void)
  {
    // First call our base class implementation
    IState::DoInit();
    
    // Check our App pointer
    assert(NULL != mApp && "SplashState::DoInit() bad app pointer");

    // Load our splash image
    mApp->mAssetManager.AddImage("Splash", "SplashImage.png", AssetLoadStyleImmediate);

    // Retrieve a sprite to the above image
    mSplashSprite = mApp->mAssetManager.GetSprite("Splash");
  }

  void SplashState::ReInit(void)
  {
    // Do nothing yet
  }

  void SplashState::UpdateFixed(void)
  {
    // Check our App pointer
    assert(NULL != mApp && "SplashState::UpdateFixed() bad app pointer, init must be called first");

    // Drop our state after 10 seconds have elapsed
    if(false == IsPaused() && GetElapsedTime() > 10.0f)
    {
      mApp->mStateManager.RemoveActiveState();
    }
  }

  void SplashState::UpdateVariable(float theElapsedTime)
  {
    // Check our App pointer
    assert(NULL != mApp && "SplashState::UpdateVariable() bad app pointer, init must be called first");
  }

  void SplashState::Draw(void)
  {
    // Check our App pointer
    assert(NULL != mApp && "SplashState::Draw() bad app pointer, init must be called first");

    // Draw our Splash sprite
    mApp->mWindow.Draw(*mSplashSprite);
  }

  void SplashState::Cleanup(void)
  {
    // Delete our sprite
    delete mSplashSprite;
    mSplashSprite = NULL;

    // Unload our image since we don't need it anymore
    mApp->mAssetManager.UnloadImage("Splash");

    // Last of all, call our base class implementation
    IState::Cleanup();
  }

} // namespace GQE

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
