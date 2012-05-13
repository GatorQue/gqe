/**
 * Provides the GQE Core internal global or static variable declarations.
 *
 * @file src/GQE/Core/Core_types.cpp
 * @author Ryan Lindeman
 * @date 20110608 - Initial Release
 * @date 20120426 - Remove gLogger variable (moved to ILogger::gInstance)
 * @date 20120512 - Remove gApp variable (moved to IApp::gApp)
 * @date 20120512 - Force compiler to create TAsset<xyz> base classes
 */
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <GQE/Core/classes/ConfigReader.hpp>
#include <GQE/Core/interfaces/TAsset.hpp>
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/loggers/onullstream>

namespace GQE
{
  /// Force instance of ConfigReader TAsset template class
  template TAsset<ConfigReader>;

  /// Force instance of sf::Font TAsset template class
  template TAsset<sf::Font>;

#if (SFML_VERSION_MAJOR < 2)
  /// Force instance of sf::Image TAsset template class
  template TAsset<sf::Image>;
#else
  /// Force instance of sf::Texture TAsset template class
  template TAsset<sf::Texture>;
#endif

  /// Force instance of sf::Music TAsset template class
  template TAsset<sf::Music>;

  /// Force instance of sf::SoundBuffer TAsset template class
  template TAsset<sf::SoundBuffer>;

  // Internal Core global variables
  GQE_API std::onullstream gNullStream;
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
