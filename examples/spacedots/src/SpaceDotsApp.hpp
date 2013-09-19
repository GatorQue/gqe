/**
 * Provides the SpaceDotsApp class which implements the SpaceDots example game
 * for the GQE library.
 *
 * @file src/SpaceDotsApp.hpp
 * @author Ryan Lindeman
 * @date 20120323 - Initial Release
 * @date 20120512 - Add new Init methods required by IApp base class
 */
#ifndef   SPACE_DOTS_APP_HPP_INCLUDED
#define   SPACE_DOTS_APP_HPP_INCLUDED

#include <GQE/Core/interfaces/IApp.hpp>

/// Provides the core game loop algorithm for the SpaceDots example.
class SpaceDotsApp : public GQE::IApp
{
  public:
    /**
     * SpaceDotsApp constructor
     * @param[in] theTitle is the title of the window
     */
    SpaceDotsApp(const std::string theTitle = "Dots");

    /**
     * SpaceDotsApp deconstructor
     */
    virtual ~SpaceDotsApp();

  protected:
    /**
      * InitAssetHandlers is responsible for registering custom IAssetHandler
      * derived classes for a specific game application.
      */
    virtual void InitAssetHandlers(void);

    /**
      * InitScreenFactory is responsible for initializing any IScreen derived
      * classes with the ScreenManager class that will be used to create new
      * IScreen derived classes as requested.
      */
    virtual void InitScreenFactory(void);

    /**
      * HandleCleanup is responsible for performing any custom last minute
      * Application cleanup steps before exiting the Application.
      */
    virtual void HandleCleanup(void);

  private:
}; // class SpaceDotsApp

#endif // SPACE_DOTS_APP_HPP_INCLUDED
/**
 * @class DotsApp
 * @ingroup Examples
 * The DotsApp class is the App class for the Dots example game
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
