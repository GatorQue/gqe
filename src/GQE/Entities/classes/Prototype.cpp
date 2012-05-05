#include "GQE/Entities/classes/Prototype.hpp"
#include "GQE/Entities/classes/Instance.hpp"
namespace GQE
{
	Prototype::Prototype(typePrototypeID thePrototypeID):
Entity(),
	mID(thePrototypeID)
{

}
Prototype::~Prototype()
{

}
typePrototypeID Prototype::GetID()
{
	return mID;
}
Instance* Prototype::MakeInstance(typeInstanceID theID)
{
	Instance* anInstance=new Instance(theID,this);
	std::map<std::string,AProperty*>::iterator anProptertyIter;
	std::map<std::string,IComponent*>::iterator anComponentIter;
	AProperty* anProperty;
	IComponent* anComponent;
	for(anProptertyIter=mPropertyList.begin(); anProptertyIter!=mPropertyList.end(); ++anProptertyIter)
	{
		anProperty=(anProptertyIter->second);
		anInstance->SetProperty(anProperty->MakeChild());
	}
	for(anComponentIter=mComponentList.begin(); anComponentIter!=mComponentList.end(); ++anComponentIter)
	{
		anComponent=(anComponentIter->second);
		anInstance->AttachComponent(anComponent->MakeClone());
	}
	return anInstance;
}
}