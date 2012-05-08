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
#include <GQE/Entity/interfaces/IComponent.hpp>

namespace GQE
{
	class GQE_API Entity
	{
	public:
		Entity();

		~Entity();
		template<class TYPE>
		TYPE GetProperty(std::string theLable)
		{
			if(theLable!="")
			{
				if(mPropertyList.find(theLable)!=mPropertyList.end())
				{
					if(mPropertyList.at(theLable)->getType()->Name()==typeid(TYPE).name())
						return static_cast<TProperty<TYPE>*>(mPropertyList[theLable])->getValue();
				}
			}
			TYPE anReturn;
			return anReturn;
		}
		template<class TYPE>
		void SetProperty(std::string theLable, TYPE theValue)
		{
			if(theLable!="")
			{
				if(mPropertyList.find(theLable)!=mPropertyList.end())
				{
					if(mPropertyList.at(theLable)->getType()->Name()==typeid(TYPE).name())
						static_cast<TProperty<TYPE>*>(mPropertyList[theLable])->setValue(theValue);
				}
			}
		}
		template<class TYPE>
		void AddProperty(std::string theLable, TYPE theValue)
		{
			if(theLable=="")
			{
				//Log Error
				return;
			}
			if(mPropertyList.find(theLable)!=mPropertyList.end())
			{
				//Log Error
				return;
			}
			TProperty<TYPE>* anProperty=new TProperty<TYPE>(theLable);
			anProperty->setValue(theValue);
			mPropertyList[anProperty->getLable()]=anProperty;
		}
		void AddProperty(AProperty* theProperty)
		{
			if(theProperty->getLable()=="")
			{
				//Log Error
				return;
			}
			if(mPropertyList.find(theProperty->getLable())!=mPropertyList.end())
			{
				//Log Error
				return;
			}
			mPropertyList[theProperty->getLable()]=theProperty;
		}
		void AttachComponent(IComponent* theComponent);

		void DetachComponent(typeComponentID theComponentID);

		void HandleEvents(sf::Event theEvent);

		void UpdateFixed();

		void UpdateVariable(float theElapstedTime);

		void Draw();

		void HandleCleanup();
	protected:
		std::map<std::string, AProperty*> mPropertyList;
		std::map<std::string, IComponent*> mComponentList;
	};
}

#endif
