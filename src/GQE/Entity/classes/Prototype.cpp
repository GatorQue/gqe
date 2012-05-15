#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/classes/Instance.hpp>
#include <GQE/Entity/interfaces/IComponent.hpp>

namespace GQE
{
  Prototype::Prototype(const typePrototypeID thePrototypeID):
    Entity(),
	  mPrototypeID(thePrototypeID)
  {

  }

  Prototype::~Prototype()
  {

  }

  const typePrototypeID Prototype::GetID(void) const
  {
	  return mPrototypeID;
  }

  Instance* Prototype::MakeInstance(typeInstanceID theID)
  {
	  Instance* anInstance=new Instance(theID,this);
	  std::map<const typePropertyID, IProperty*>::iterator anProptertyIter;
	  std::map<const typeComponentID, IComponent*>::iterator anComponentIter;
	  for(anProptertyIter=mPropertyList.begin(); anProptertyIter!=mPropertyList.end(); ++anProptertyIter)
	  {
		  IProperty* anProperty = (anProptertyIter->second);
		  anInstance->AddProperty(anProperty->MakeChild());
	  }
	  for(anComponentIter=mComponentList.begin(); anComponentIter!=mComponentList.end(); ++anComponentIter)
	  {
		  IComponent* anComponent = (anComponentIter->second);
		  anInstance->AttachComponent(anComponent->MakeClone());
	  }
	  return anInstance;
  }
}