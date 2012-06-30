/**
 * Provides the IAction interface class used by the ActionSystem.
 *
 * @file include/GQE/Entity/interfaces/IAction.hpp
 * @author Jacob Dix
 * @date 20120627 - Initial Release
 */
#ifndef IACTION_HPP_INCLUDED
#define IACTION_HPP_INCLUDED

#include <map>
#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
  /// The IAction base class to be managed by the ActionSystem class
  class GQE_API IAction
  {
    public:
      /**
       * IAction default constructor
       * @param[in] theActionID used to identify this action
       */
      IAction(const typeActionID theActionID);

      /**
       * IAction destructor
       */
      virtual ~IAction();

      /**
       * GetID will return the Action ID used by this IAction class.
       * @return the action ID for this IAction class
       */
      const typeActionID GetID(void) const;

      /**
       * DoAction is responsible for performing the work of this IAction class
       * using theEntity provided as the originator of the IAction.
       */
      virtual void DoAction(IEntity* theEntity) = 0;

    protected:
      // Variables
      ///////////////////////////////////////////////////////////////////////////

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// The action ID assigned to this IAction class
      const typeActionID mActionID;
  };
} // namespace GQE

#endif // IACTION_HPP_INCLUDED

/**
 * @class GQE::IAction
 * @ingroup Entity
 * The IAction class represents the interface used by all Actions that can be
 * performed by a specific IEntity class. Most universal actions are developed
 * as ISystem derived classes, but the IAction class is for those that are
 * very IEntity specific and only affect a few IEntity classes. These IAction
 * classes are all managed by the ActionSystem class.
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
