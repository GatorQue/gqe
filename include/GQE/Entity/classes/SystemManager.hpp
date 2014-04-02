/**
 * Provides a class that handles messages sent between systems.
 *
 * @file include/GQE/Entity/classes/SystemManager.hpp
 * @author Jacob Dix
 * @date 20140111 - Initial Release
 * @date 20140401 - Renamed from SystemManager to World. 
 */

#ifndef SYSTEM_DIRECTORY_HPP_INCLUDED
#define SYSTEM_DIRECTORY_HPP_INCLUDED

#include <map>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
  /// Provides the PropertyManager class for managing IProperty classes
  class GQE_API SystemManager
  {
    public:
      /**
       * SystemManager default constructor
       */
      SystemManager();

      /**
       * SystemManager deconstructor
       */
      virtual ~SystemManager();

      void TriggerEvent(typeSystemID theSystemId, std::string theEventId,void* theContext=NULL);
      void AddSystem(GQE::ISystem* theSystem, typeSystemID theSystemId = "");

      /**
       * GetSystemManager will return the most recent SystemManager based class that was created so
       * it can be used to access the SystemManager. NULL will be returned if none is available
       */
      static SystemManager* GetSystemManager(void);
    private:
      static SystemManager* gSystemManager;
      std::map<std::string,GQE::ISystem*> mSystems;
      /**
       * SystemManager copy constructor is private because we do not allow copies of
       * our Singleton class
       */
      SystemManager(const SystemManager&);               // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our Singleton class
       */
      SystemManager& operator=(const SystemManager&);    // Intentionally undefined
  }; // SystemManager class
} // namespace GQE

#endif

/**
 * @class GQE::SystemManager
 * @ingroup Entity

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
