/**
* Provides a system for entities,
*
* @file include/GQE/Entity/classes/Entity.hpp>
* @author Jacob Dix
* @date 20120418 - Initial Release
* @date 20120507 - Changed how properties are retrived/set.
*/
#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

#include <map>
#include <vector>
#include <SFML/Window/Event.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Entity/Entity_types.hpp>
#include <GQE/Entity/interfaces/IProperty.hpp>

namespace GQE
{
	class GQE_API Entity
	{
	public:
		Entity();

		~Entity();

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

		void AddProperty(IProperty* theProperty);

		void AttachComponent(IComponent* theComponent);

		void DetachComponent(const typeComponentID theComponentID);

		void HandleEvents(sf::Event theEvent);

		void UpdateFixed();

		void UpdateVariable(float theElapstedTime);

		void Draw();

		void HandleCleanup();
	protected:
		std::map<const typePropertyID, IProperty*>   mPropertyList;
		std::map<const typeComponentID, IComponent*> mComponentList;
	};
}

#endif
