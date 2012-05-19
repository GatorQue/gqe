#include "GQE/Entity/interfaces/IEntity.hpp"
#include <GQE/Entity/interfaces/IComponent.hpp>

namespace GQE
{

	IEntity::IEntity()
	{
	}

	IEntity::~IEntity()
	{
		std::map<const typePropertyID, IProperty*>::iterator anProptertyIter;
		std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
		for(anProptertyIter=mPropertyList.begin(); anProptertyIter!=mPropertyList.end(); ++anProptertyIter)
		{
  		IProperty* anProperty = (anProptertyIter->second);
			delete anProperty;
			anProperty = NULL;
		}
		for(anComponentIter=mComponentList.begin(); anComponentIter!=mComponentList.end(); ++anComponentIter)
		{
  		IComponent* anComponent = (anComponentIter->second);
			anComponent->DeInit();
			anComponent->HandleCleanup();
			delete anComponent;
			anComponent=NULL;
		}
	}

  void IEntity::AddProperty(IProperty* theProperty)
	{
		if(mPropertyList.find(theProperty->GetID())!=mPropertyList.end())
		{
      ELOG() << "Entity:AddProperty() property(" << theProperty->GetID() << ") not found!" << std::endl;
			return;
		}
		mPropertyList[theProperty->GetID()]=theProperty;
	}

  void IEntity::AttachComponent(IComponent* theComponent)
	{
    if(NULL == theComponent)
    {
      ELOG() << "Entity:AttachComponent() null component provided!" << std::endl;
			return;
    }
		if(mComponentList.find(theComponent->GetID())!=mComponentList.end())
		{
      ELOG() << "Entity:AttachComponent() component(" << theComponent->GetID() << ") not found!" << std::endl;
			return;
		}
		mComponentList[theComponent->GetID()]=theComponent;
		theComponent->DoInit(this);
	}

	void IEntity::DetachComponent(const typeComponentID theComponentID)
	{
		if(mComponentList.find(theComponentID)==mComponentList.end())
		{
      ELOG() << "Entity:DetachComponent() component(" << theComponentID << ") not found!" << std::endl;
			return;
    }
		IComponent* anComponent = mComponentList[theComponentID];
		anComponent->DeInit();
	}

	void IEntity::HandleEvents(sf::Event theEvent)
	{
		std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
		for(anComponentIter=mComponentList.begin(); anComponentIter!=mComponentList.end(); ++anComponentIter)
		{
  		IComponent* anComponent = anComponentIter->second;
			anComponent->HandleEvents(theEvent);
		}
	}
	
  void IEntity::UpdateFixed()
	{
		std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
		for(anComponentIter=mComponentList.begin(); anComponentIter!=mComponentList.end(); ++anComponentIter)
		{
  		IComponent* anComponent = anComponentIter->second;
			anComponent->UpdateFixed();
		}
	}

	void IEntity::UpdateVariable(float theElapstedTime)
	{
		std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
		for(anComponentIter=mComponentList.begin(); anComponentIter!=mComponentList.end(); ++anComponentIter)
		{
  		IComponent* anComponent = anComponentIter->second;
			anComponent->UpdateVariable(theElapstedTime);
		}
	}
	
  void IEntity::Draw()
	{
		std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
		for(anComponentIter=mComponentList.begin(); anComponentIter!=mComponentList.end(); ++anComponentIter)
		{
  		IComponent* anComponent = anComponentIter->second;
			anComponent->Draw();
		}
	}

	void IEntity::HandleCleanup()
	{
		std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
		for(anComponentIter=mComponentList.begin(); anComponentIter!=mComponentList.end(); ++anComponentIter)
		{
  		IComponent* anComponent = anComponentIter->second;
			anComponent->HandleCleanup();
			if(anComponent->CanRemove())
			{
				mComponentList.erase(anComponentIter);
				delete anComponent;
				anComponent = NULL;
			}
		}
	}
}
