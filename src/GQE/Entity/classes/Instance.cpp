/**
 * Provides an instantiation of an IEntity class that was based on a Prototype
 * IEntity class.
 * @file src/GQE/Entity/classes/Instance.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120618 - Move ID related stuff to IEntity base class
 * @date 20120630 - Add Destroy method implementation to call DestroyInstance
 */
#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/classes/Instance.hpp>
#include <GQE/Entity/interfaces/ISystem.hpp>

namespace GQE
{
  Instance::Instance(Prototype& thePrototype) :
    mPrototype(thePrototype)
  {
  }

  Instance::~Instance()
  {
    // Call our Prototype to remove ourselves
    mPrototype.DestroyInstance(GetID());
  }

  void Instance::Destroy(void)
  {
    // First drop all our registered ISystem classes
    DropAllSystems();

    // Next, call our Prototype to remove ourselves
    mPrototype.DestroyInstance(GetID());
  }

  Prototype& Instance::GetPrototype()
  {
    return mPrototype;
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
