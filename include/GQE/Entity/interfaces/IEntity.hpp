/**
 * Provides the IEntity interface class for all game entities in a game.
 *
 * @file include/GQE/Entity/interfaces/IEntity.hpp>
 * @author Jacob Dix
 * @date 20120418 - Initial Release
 * @date 20120507 - Changed how properties are retrived/set.
 * @date 20120519 - Renaimed Entity class to IEntity and moved to interfaces folder.
 */
#ifndef IENTITY_HPP_INCLUDED
#define IENTITY_HPP_INCLUDED

#include <map>
#include <vector>
#include <typeinfo>
#include <SFML/Window/Event.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Entity/interfaces/IProperty.hpp>
#include <GQE/Entity/interfaces/TProperty.hpp>
#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
  /// The IEntity base class to be managed by the EntityManager class
  class GQE_API IEntity
  {
    public:
      /**
       * IEntity default constructor
       */
      IEntity();

      /**
       * IEntity destructor
       */
      virtual ~IEntity();

      /**
       * GetProperty returns the property as type with the ID of thePropertyID.
       * @param[in] thePropertyID is the ID of the property to return.
       * @return the value stored in the found propery in the form of TYPE. If no
       * Property was found it returns the default value the type constructor.
       */
      template<class TYPE>
        TYPE GetProperty(const typePropertyID thePropertyID)
        {
          if(mPropertyList.find(thePropertyID)!=mPropertyList.end())
          {
            if(mPropertyList.at(thePropertyID)->GetType()->Name()==typeid(TYPE).name())
              return static_cast<TProperty<TYPE>*>(mPropertyList[thePropertyID])->GetValue();
          }
          else
          {
            WLOG() << "Entity:GetProperty() returning blank property(" << thePropertyID << ") type" << std::endl;
          }
          TYPE anReturn=TYPE();
          return anReturn;
        }

      /**
       * SetProperty sets the property with the ID of thePropertyID to theValue.
       * @param[in] thePropertyID is the ID of the property to set.
       * @param[in] theValue is the value to set.
       */
      template<class TYPE>
        void SetProperty(const typePropertyID thePropertyID, TYPE theValue)
        {
          if(mPropertyList.find(thePropertyID)!=mPropertyList.end())
          {
            if(mPropertyList.at(thePropertyID)->GetType()->Name()==typeid(TYPE).name())
            {
              static_cast<TProperty<TYPE>*>(mPropertyList[thePropertyID])->SetValue(theValue);
            }
          }
          else
          {
            ELOG() << "Entity:SetProperty() unable to find property(" << thePropertyID << ")" << std::endl;
          }
        }

      /**
       * AddProperty Creates a Property and addes it to this entity.
       * @param[in] thePropertyID is the ID of the property to create.
       * @param[in] theValue is the inital value to set.
       */
      template<class TYPE>
        void AddProperty(const typePropertyID thePropertyID, TYPE theValue)
        {
          if(mPropertyList.find(thePropertyID)!=mPropertyList.end())
          {
            ELOG() << "Entity:AddProperty() label(" << thePropertyID << ") not found!" << std::endl;
            return;
          }
          TProperty<TYPE>* anProperty=new TProperty<TYPE>(thePropertyID);
          anProperty->SetValue(theValue);
          mPropertyList[anProperty->GetID()]=anProperty;
        }

      /**
       * AddProperty gets a premade Property and addes it to this entity.
       * @param[in] theProperty is a pointer to a pre exisiting property.
       */
      void AddProperty(IProperty* theProperty);

      /**
       * AttachComponent Attaches a component to this entity.
       * @param[in] theComponent is the pre made component to set.
       */
      void AttachComponent(IComponent* theComponent);

      /**
       * DetachComponent removes an exsisting component from this entity.
       * @param[in] theComponentID is the ID of the Componenet.
       */
      void DetachComponent(const typeComponentID theComponentID);

      /**
       * HandleEvents tells all this entity's components to call their
       * HandleEvent function.
       * @param[in] theEvent is the SFML Event.
       */
      void HandleEvents(sf::Event theEvent);

      /**
       * UpdateFixed tells all this entity's components to call their UpdateFixed
       * function.
       */
      void UpdateFixed(void);

      /**
       * UpdateVariable tells all this entity's components to call their
       * UpdateVariable function.
       * @param[in] theElapsedTime is the time since last UpdateVariable Call.
       */
      void UpdateVariable(float theElapsedTime);

      /**
       * Draw Tells all this entity's components to call their Draw Function
       */
      void Draw();

      /**
       * Destroys this entity and every thing attached to it.
       */
      void HandleCleanup();
    protected:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// A map of all Properties available for this IEntity derived class
      std::map<const typePropertyID, IProperty*>   mPropertyList;

      /// A map of all Components available for this IEntity derived class
      std::map<const typeComponentID, IComponent*> mComponentList;
  };
}

#endif // IENTITY_HPP_INCLUDED

/**
 * @class GQE::IEntity
 * @ingroup Entity
 * The IEntity class represents the interface used by all Entities managed by
 * the EntityManager class. This class is at the heart of the GQE Entity
 * library.
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

