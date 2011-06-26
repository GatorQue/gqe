/**
 * Provides the StatManager class in the GQE namespace which is responsible
 * for collecting and providing Statistical information about the application.
 * This information includes, the current Updates per second and Frames per
 * second and other statistics.
 *
 * @file include/GQE/Core/classes/StatManager.hpp
 * @author Ryan Lindeman
 * @date 20110128 - Initial Release
 * @date 20110128 - Moved to GQE Core library and include directory
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Add SFML 2.0 support
 * @date 20110218 - Change to system include style
 * @date 20110625 - Added UpdateVariable and changed Update to UpdateFixed
 */
#ifndef   CORE_STAT_MANAGER_HPP_INCLUDED
#define   CORE_STAT_MANAGER_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
 
namespace GQE
{
  /// Provides collection and publishing of all game statistics
  class GQE_API StatManager
  {
  public:
 
    /**
     * StatManager constructor
     */
    StatManager();
 
    /**
     * StatManager deconstructor
     */
    virtual ~StatManager();
 
    /**
     * DoInit will reset all the statistics, load necessary fonts and
     * performing other useful things.
     */
    void DoInit(void);

    /**
     * DeInit will unload the fonts used and dump a summary of statistics
     * collected to the log file.
     */
    void DeInit(void);

    /**
     * IsShowing will return true if the current statistics are being displayed.
     * @return true if stats are being displayed, false otherwise
     */
    bool IsShowing(void) const;

    /**
     * SetShowing will either enable or disable the showing of the current
     * statistics being displayed.
     * @param[in] theShow is the new show value
     */
    void SetShow(bool theShow);

    /**
     * GetUpdate will return the current update number which is the number of
     * updates that have been called since the application started.
     * @return the number of updates since the application started.
     */
    Uint32 GetUpdates(void) const;

    /**
     * GetFrame will return the current frame number which is the number of
     * frames that have been displayed since the application started.
     * @return the number of frames since the application started.
     */
    Uint32 GetFrames(void) const;

    /**
     * RegisterApp will register a pointer to the App class so it can be used
     * by the StatManager for error handling and log reporting.
     * @param[in] theApp is a pointer to the App (or App derived) class
     */
    void RegisterApp(App* theApp);

    /**
     * UpdateFixed is responsible for updating game loop statistics like
     * Updates per second.
     */
    void UpdateFixed(void);
 
    /**
     * Draw is responsible for updating game loop statistics like Frames
     * per second and handling all Drawing needs for the StatManager.
     */
    void Draw(void);

  private:
    // Constants
    ///////////////////////////////////////////////////////////////////////////
 
    // Variables
    ///////////////////////////////////////////////////////////////////////////
    /// Pointer to the App class for error handling and logging
    App*        mApp;
    /// Allow the current statistics to be displayed?
    bool        mShow;
    /// Total number of frames drawn since DoInit was called
    Uint32      mFrames;
    /// Frame clock for displaying Frames per second value
    sf::Clock   mFrameClock;
#if (SFML_VERSION_MAJOR < 2)
    /// Debug string to display that shows the frames per second
    sf::String  mFPS;
#else
    sf::Text    mFPS;
#endif

    /// Total number of updates done since DoInit was called
    Uint32      mUpdates;
    /// Update clock for displaying Updates per second value
    sf::Clock   mUpdateClock;
#if (SFML_VERSION_MAJOR < 2)
    /// Debug string to display that shows the updates per second
    sf::String  mUPS;
#else
    sf::Text    mUPS;
#endif

    /**
     * StatManager copy constructor is private because we do not allow copies
     * of our class
     */
    StatManager(const StatManager&); // Intentionally undefined
 
    /**
     * Our assignment operator is private because we do not allow copies
     * of our class
     */
    StatManager& operator=(const StatManager&); // Intentionally undefined
 
  }; // class StatManager
}; // namespace GQE
 
#endif // CORE_STAT_MANAGER_HPP_INCLUDED

/**
 * @class GQE::StatManager
 * @ingroup Core
 * The StatManager class is used by the App class to collect and present
 * all statistics collected during the running of the Game Engine
 * application.  These statistics include but are not limited to:
 * Update() method calls per second, Draw() method calls per second,
 * Overall elapsed time, individual elapsed time per game state, etc.
 * These statistics can be reported back to the publisher or written to
 * a file for debug, development, or sale purposes.
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
