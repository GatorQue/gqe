/**
 * Provides a class that manages all properties for an IEntity. This enables
 * using the PropertyManager class in another class than IEntity if desired.
 *
 * @file include/GQE/Entity/classes/PropertyManager.hpp
 * @author Jacob Dix
 * @date 20120609 Initial Release
 */

#include <GQE/Entity/classes/PropertyManager.hpp>

namespace GQE
{
  PropertyManager::PropertyManager()
  {
  }

  PropertyManager::~PropertyManager()
  {
    // Make sure to remove all registered properties on desstruction
    std::map<const typePropertyID, IProperty*>::iterator anProptertyIter;
    for(anProptertyIter = mList.begin();
        anProptertyIter != mList.end();
        ++anProptertyIter)
    {
      IProperty* anProperty = (anProptertyIter->second);
      delete anProperty;
      anProperty = NULL;
    }
  }

  bool PropertyManager::HasID(const typePropertyID thePropertyID)
  {
    bool anResult = false;

    // See if thePropertyID was found in our list of properties
    anResult = (mList.find(thePropertyID) != mList.end());

    // Return true if thePropertyID was found above, false otherwise
    return anResult;
  }


  void PropertyManager::Add(IProperty* theProperty)
  {
    if(mList.find(theProperty->GetID()) == mList.end())
    {
      mList[theProperty->GetID()] = theProperty;
    }
    else
    {
      WLOG() << "PropertyManager:Add() property(" << theProperty->GetID()
        << ") already exists!" << std::endl;
    }
  }

  void PropertyManager::Clone(const PropertyManager& thePropertyManager)
  {
    // Make sure to remove all registered properties on desstruction
    std::map<const typePropertyID, IProperty*>::const_iterator anProptertyIter;
    for(anProptertyIter = thePropertyManager.mList.begin();
        anProptertyIter != thePropertyManager.mList.end();
        ++anProptertyIter)
    {
      IProperty* anProperty = (anProptertyIter->second);
      Add(anProperty->MakeClone());
    }
  }
} // namespace GQE

/**
 * Copyright (c) 2010-2012 Jacob Dix
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
