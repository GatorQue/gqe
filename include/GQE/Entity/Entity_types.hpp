/**
 * Provides the GQE Entity typedef's and forward class declarations.
 *
 * @file include/GQE/Entity/Entity_types.hpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120616 - Fixed typedef comments
 * @date 20120618 - Changed name from typeInstanceID to typeEntityID
 * @date 20120623 - Add forward declaration of ISystem derived classes
 * @date 20120627 - Add forward declaration of ActionSystem classes
 * @date 20120720 - Moved PropertyManager to Core library
 */
#ifndef ENTITY_TYPES_HPP_INCLUDED
#define ENTITY_TYPES_HPP_INCLUDED

#include <string>
#include <GQE/Config.hpp>
#include <GQE/Core.hpp>

namespace GQE
{
  // Forward declare GQE Entity interfaces provided
  class IAction;
  class IEntity;
  class ISystem;

  // Forward declare GQE Entity classes provided
  class Prototype;
  class Instance;
  class PrototypeManager;

  // Forward declare GQE Entity systems provided
  class ActionSystem;
  class AnimationSystem;
  class MovementSystem;
  class RenderSystem;

  /// Declare Action ID typedef which is used for identifying IAction objects
  typedef std::string typeActionID;

  /// Declare Entity ID typedef which is used for identifying IEntity objects
  typedef Uint32 typeEntityID;

  /// Declare Prototype ID typedef which is used for identifying Prototype objects
  typedef std::string typePrototypeID;

  /// Declare System ID typedef which is used for identifying ISystem objects
  typedef std::string typeSystemID;
}
#endif // ENTITY_TYPES_HPP_INCLUDED

/**
 * @ingroup Entity
 * A forward declaration of all classes, components, and interfaces provided by
 * the GQE Entity module/library.
 *
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

