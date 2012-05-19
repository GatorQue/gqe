/**
* Provides a system for entities,
*
* @file include/GQE/Entity/interfaces/IEntity.hpp>
* @author Jacob Dix
* @date 20120418 - Initial Release
* @date 20120507 - Changed how properties are retrived/set.
* @date 20120519 - Renaimed Entity class to IEntity and moved to interfaces folder.
*/
#pragma once
#ifndef IENTITY_HPP_INCLUDED
#define IENTITY_HPP_INCLUDED

#include <map>
#include <vector>
#include <typeinfo>
#include <SFML/Window/Event.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Entity/Entity_types.hpp>
#include <GQE/Entity/interfaces/IProperty.hpp>
#include <GQE/Entity/interfaces/TProperty.hpp>

namespace GQE
{
	class GQE_API IEntity
	{
	public:
		/*
		* IEntity Constructor.
		*/
		IEntity();
		/*
		* IEntity Destructor.
		*/
		~IEntity();
		/*
		* GetProperty returns the property as type with the ID of thePropertyID.
		* @param[in] thePropertyID is the ID of the property to return.
		* @return the value stored in the found propery in the form of TYPE. If no Property was found it returns the default value
		* the type constructor.
		*/
		template<class TYPE>
		TYPE GetProperty(const typePropertyID thePropertyID)
		{
      if(mPropertyList.find(thePropertyID)!=mPropertyList.end())
			{
				if(mPropertyList.at(thePropertyID)->getType()->Name()==typeid(TYPE).name())
					return static_cast<TProperty<TYPE>*>(mPropertyList[thePropertyID])->getValue();
			}
      else
      {
        WLOG() << "Entity:GetProperty() returning blank property(" << thePropertyID << ") type" << std::endl;
      }
			TYPE anReturn=TYPE();
			return anReturn;
		}
		/*
		* SetProperty sets the property with the ID of thePropertyID to theValue.
		* @param[in] thePropertyID is the ID of the property to set.
		* @param[in] theValue is the value to set.
		*/
		template<class TYPE>
		void SetProperty(const typePropertyID thePropertyID, TYPE theValue)
		{
			if(mPropertyList.find(thePropertyID)!=mPropertyList.end())
			{
				if(mPropertyList.at(thePropertyID)->getType()->Name()==typeid(TYPE).name())
        {
					static_cast<TProperty<TYPE>*>(mPropertyList[thePropertyID])->setValue(theValue);
        }
			}
      else
      {
        ELOG() << "Entity:SetProperty() unable to find property(" << thePropertyID << ")" << std::endl;
      }
		}
		/*
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
			anProperty->setValue(theValue);
			mPropertyList[anProperty->GetID()]=anProperty;
		}
		/*
		* AddProperty gets a premade Property and addes it to this entity.
		* @param[in] theProperty is a pointer to a pre exisiting property.
		*/
		void AddProperty(IProperty* theProperty);
		/*
		* AttachComponent Attaches a component to this entity.
		* @param[in] theComponent is the pre made component to set.
		*/
		void AttachComponent(IComponent* theComponent);
		/*
		* DetachComponent removes an exsisting component from this entity.
		* @param[in] theComponentID is the ID of the Componenet.
		*/
		void DetachComponent(const typeComponentID theComponentID);
		/*
		* HandleEvents Tells all this entity's components to call their HandleEvent Function
		* @param[in] theEvent is the SFML Event.
		*/
		void HandleEvents(sf::Event theEvent);
		/*
		* UpdateFixed Tells all this entity's components to call their UpdateFixed Function
		*/
		void UpdateFixed();
		/*
		* UpdateVariable Tells all this entity's components to call their UpdateVariable Function
		* @param[in] theElapstedTime is the time since last UpdateVariable Call.
		*/
		void UpdateVariable(float theElapstedTime);
		/*
		* Draw Tells all this entity's components to call their Draw Function
		*/
		void Draw();
		/*
		* Destorys this entity and every thing attached to it.
		*/
		void HandleCleanup();
	protected:
		//Variables
		/////////////////
		std::map<const typePropertyID, IProperty*>   mPropertyList;
		std::map<const typeComponentID, IComponent*> mComponentList;
	};
}

#endif
