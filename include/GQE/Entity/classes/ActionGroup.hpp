/**
 * Provides a class that manages all active and inactive IAction classes for an
 * IEntity. This is typically stored as a property using the PropertyManager
 * class.
 *
 * @file include/GQE/Entity/classes/ActionGroup.hpp
 * @author Jacob Dix
 * @date 20120627 - Initial Release
 */

#ifndef ACTION_GROUP_HPP_INCLUDED
#define ACTION_GROUP_HPP_INCLUDED

#include <map>
#include <typeinfo>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/interfaces/IProperty.hpp>
#include <GQE/Core/interfaces/TProperty.hpp>
#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
  /// Provides the PropertyManager class for managing IProperty classes
  class GQE_API ActionGroup
  {
    public:
      /**
       * ActionGroup default constructor
       */
      ActionGroup();

      /**
       * ActionGroup default constructor
       * @param[in] theActionSystem pointer to use for adding actions
       */
      ActionGroup(ActionSystem* theActionSystem);

      /**
       * ActionGroup deconstructor
       */
      virtual ~ActionGroup();

      /**
       * IsAvailable returns true if theActionID specified exists in either the
       * active or disabled ActionGroup lists.
       * @param[in] theActionID to lookup in the ActionGroup lists
       * @return true if theActionID is available, false otherwise
       */
      bool IsAvailable(const typeActionID theActionID) const;

      /**
       * IsActive returns true if theActionID provided is currently active in
       * this ActionGroup.
       * @param[in] theActionID to lookup in this ActionGroup
       * @return true if theActionID exists and is active, false otherwise
       */
      bool IsActive(const typeActionID theActionID) const;

      /**
       * Activate will active theActionID specified if it is available in the
       * disabled ActionGroup list.
       * @param[in] theActionID to activate
       */
      void Activate(const typeActionID theActionID);

      /**
       * Disable will disable theActionID specified if it is found in the
       * active ActionGroup list.
       * @param[in] theActionID to disable
       */
      void Disable(const typeActionID theActionID);

      /**
       * Add will attempt to retrieve theActionID from the ActionSystem and
       * add the action as a disabled action in this ActionGroup class.
       * @param[in] theActionID to find in the ActionSystem and add
       */
      void Add(const typeActionID theActionID);

      /**
       * Drop will attempt to drop theActionID from both the active or
       * disabled ActionGroup list such that it can never be activated
       * again for this ActionGroup.
       * @param[in] theActionID to find and drop in the ActionGroup lists
       */
      void Drop(const typeActionID theActionID);

      /**
       * DoActions is responsible for calling the DoAction method and providing
       * the IEntity pointer for each Active action in this ActionGroup.
       * @param[in] theEntity pointer to provide to each DoAction method
       */
      void DoActions(IEntity* theEntity);

      /**
       * SetActionSystem will save theActionSystem pointer provided as the
       * source for adding new actions to this ActionGroup.
       */
      void SetActionSystem(ActionSystem* theActionSystem);

    protected:

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// A reference address to the ActionSystem class
      ActionSystem* mActionSystem;
      /// A map of all active IActions available for this ActionGroup class
      std::map<const typeActionID, IAction*> mActive;
      /// A map of all disabled IActions available for this ActionGroup class
      std::map<const typeActionID, IAction*> mDisabled;

  }; // ActionGroup class
} // namespace GQE

#endif

/**
 * @class GQE::ActionGroup
 * @ingroup Entity
 * The ActionGroup class is responsible for managing and executing each active
 * IAction registered for this ActionGroup.
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
