/**
 * Provides the GQE namespace includes for the GQE Entity includes and
 * variables.
 *
 * @file include/GQE/Entity.hpp
 * @author Jacob Dix
 * @date 20100423 - Initial Release
 * @date 20120616 - Added PropertyManager class
 * @date 20120618 - Added missing Entity classes
 * @date 20120623 - Added AnimationSystem class
 * @date 20120720 - Moved PropertyManager to Core library
 */
#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Headers
/////////////////////////////////////////////////////////////////////////////
#include <GQE/Config.hpp>

// GQE Entity includes
#include <GQE/Entity/classes/PrototypeManager.hpp>
#include <GQE/Entity/classes/Instance.hpp>
#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>
#include <GQE/Entity/interfaces/ISystem.hpp>
#include <GQE/Entity/systems/AnimationSystem.hpp>
#include <GQE/Entity/systems/MovementSystem.hpp>
#include <GQE/Entity/systems/RenderSystem.hpp>

#endif // ENTITY_HPP_INCLUDED

/**
 * @defgroup Entity GQE Entity module
 * The GQE Entity module/library provides Entity related classes that can be
 * used to manage game entities used for many games and game genres.
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

