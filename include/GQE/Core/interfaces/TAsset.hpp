/**
 * Provides the TAsset template class for managing a single Asset and is used
 * by the AssetManager in the GQE namespace which is responsible for providing
 * the Asset management facilities for the App base class used in the GQE core
 * library.
 *
 * @file include/GQE/Core/interfaces/TAsset.hpp
 * @author Ryan Lindeman
 * @date 20100723 - Initial Release
 * @date 20110127 - Moved to GQE Core library and include directory
 * @date 20110127 - Use new OS independent Uint/Int types
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Change to system include style
 * @date 20110627 - Removed extra ; from namespace
 */
#ifndef   CORE_TASSET_HPP_INCLUDED
#define   CORE_TASSET_HPP_INCLUDED

#include <assert.h>
#include <string>
#include <stddef.h>
#include <GQE/Core/Core_types.hpp>

namespace GQE
{
  /// Provides base template class for all game assets
  template<class TYPE>
  class GQE_API TAsset
  {
  public:
    /**
     * TAsset constructor
     * @param[in] theFilename to use when loading this asset
     * @param[in] theStyle to use when loading this asset
     */
    TAsset(std::string theFilename,
		AssetLoadingStyle theStyle = AssetLoadStyleBackground) :
      mApp(NULL),
      mFilename(theFilename),
      mStyle(theStyle),
      mAsset(NULL),
      mLoaded(false),
      mReferences(0)
    {
      assert(AssetLoadStyleFirst < theStyle && AssetLoadStyleLast > theStyle &&
             "TAsset::TAsset() invalid style provided");
    }

    /**
     * TAsset deconstructor
     */
    virtual ~TAsset() {
      if(0 != mReferences)
      {
        // Log an error, since not all of our assets have had their FreeAsset
        // method called!
      }
    }

    /**
     * RegisterApp will register a pointer to the App class so it can be used
     * by the TAsset derived classes for error handling and log reporting.
     * @param[in] theApp is a pointer to the App (or App derived) class
     */
    void RegisterApp(App* theApp) {
      // Check that our pointer is good
      assert(NULL != theApp && "TAsset::RegisterApp() theApp pointer provided is bad");

      // Make a note of the pointer
      assert(NULL == mApp && "TAsset::RegisterApp() theApp pointer was already registered");
      mApp = theApp;
    }

    /**
     * IsLoaded will return true if the Asset has been loaded.
     * @return true if loaded, false otherwise
     */
    bool IsLoaded(void) const {
      return mLoaded;
    }

    /**
     * GetLoadingStyle will return the Loading Style for this asset.
     * @return LoadingStyle enumeration for this asset
     */
    AssetLoadingStyle GetLoadingStyle(void) const {
      return mStyle;
    }

    /**
     * SetLoadingStyle will set the Loading Style for this asset.
     * @param[in] theStyle to use when Loading this asset
     */
    void SetLoadingStyle(AssetLoadingStyle theStyle) {
      assert(AssetLoadStyleFirst < theStyle && AssetLoadStyleLast > theStyle &&
             "TAsset::SetLoadingStyle() invalid style provided");

      mStyle = theStyle;
    }

    /**
     * AddReference will increment the reference count.
     */
    void AddReference(void)
    {
      mReferences++;
    }

    /**
     * GetReferences will return the reference count.
     * @return number of entities referencing this Asset
     */
    const Uint16 GetReferences(void) const {
      return mReferences;
    }

    /**
     * DropReference will decrement the reference count.
	 * @param[in] theRemoveFlag indicates the asset should be removed when the
	 *            number of references for this asset reaches 0.
     */
    void DropReference(bool theRemoveFlag = true) {
      assert(0 != mReferences && "TAsset::DropReference() called more than AddReference()");
      mReferences--;
 
      // Delete asset for zero references and theRemoveFlag is set
      if(true == theRemoveFlag && 0 == mReferences)
      {
        UnloadAsset();
      }
    }

    /**
     * GetAsset will return the Asset if it is available.
     * @return pointer to the Asset or NULL if not available yet.
     */
    TYPE* GetAsset(void) const {
      return mAsset;
    }

    /**
     * LoadAsset is responsible for loading the Asset.
     */
    virtual void LoadAsset(void) = 0;

  protected:
    // Variables
    ///////////////////////////////////////////////////////////////////////////
    /// Pointer to the App class for error handling and logging
    App*                mApp;
    /// The filename associated with this asset
    const std::string   mFilename;
    /// The loading style to use with this asset
    AssetLoadingStyle   mStyle;
    /// Pointer to the loaded asset
    TYPE*               mAsset;
    /// True if the asset has been loaded or provided
    bool                mLoaded;

    /**
     * UnloadAsset is responsible for destroying or unloading the Asset and
     * is called by FreeAsset.
     */
    virtual void UnloadAsset(void) = 0;

  private:
    /// A counter for every time this asset is referenced
    Uint16    mReferences;

    /**
     * Our copy constructor is private because we do not allow copies of our
     * class
     */
    TAsset(const TAsset&);  // Intentionally undefined

    /**
     * Our assignment operator is private because we do not allow copies of our
     * class
     */
    TAsset& operator=(const TAsset&); // Intentionally undefined
  }; // class TAsset
} // namespace GQE

#endif // CORE_TASSET_HPP_INCLUDED

/**
 * @class GQE::TAsset
 * @ingroup Core
 * The TAsset template class is the template used to create a new
 * asset to be managed by the AssetManager class and provides reference
 * counting for each Asset being tracked.
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
