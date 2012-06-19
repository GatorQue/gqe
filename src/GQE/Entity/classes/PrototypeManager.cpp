/**
 * Provides a system for managing Prototype and Instance IEntity derived
 * classes.
 *
 * @file src/GQE/Entity/classes/PrototypeManager.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120618 - Changed name from mPrototypeList to mPrototypes
 */
#include <GQE/Entity/classes/PrototypeManager.hpp>
#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/classes/Instance.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
  PrototypeManager::PrototypeManager()
  {
    ILOG() << "PrototypeManager::ctor()" << std::endl;
  }

  PrototypeManager::~PrototypeManager()
  {
    ILOG() << "PrototypeManager::dtor()" << std::endl;
  }

  void PrototypeManager::AddPrototype(Prototype* thePrototype)
  {
    // Make sure a NULL prototype pointer wasn't provided
    if(thePrototype != NULL)
    {
      mPrototypes[thePrototype->GetID()]=thePrototype;
    }
    else
    {
      ELOG() << "PrototypeManager::AddPrototype() Null pointer provided!" << std::endl;
    }
  }


  Prototype* PrototypeManager::GetPrototype(const typePrototypeID thePrototypeID)
  {
    Prototype* anResult = NULL;

    if(mPrototypes.find(thePrototypeID) != mPrototypes.end())
    {
      anResult = mPrototypes[thePrototypeID];
    }
    else
    {
      WLOG() << "PrototypeManager::GetPrototype(" << thePrototypeID << ") Not found!" << std::endl;
    }

    // Return anResult found (which might be NULL if none was found)
    return anResult;
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
