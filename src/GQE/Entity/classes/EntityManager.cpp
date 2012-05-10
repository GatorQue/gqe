#include <GQE/Entity/classes/EntityManager.hpp>
#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/classes/Instance.hpp>
namespace GQE
{
	EntityManager::EntityManager():
mNextID(0)
{

}
void EntityManager::AddPrototype(Prototype* thePrototype)
{
	if(thePrototype==NULL)
		return;
	mPrototypeList[thePrototype->GetID()]=thePrototype;
}
typeInstanceID EntityManager::AddInstance(typePrototypeID thePrototypeID)
{
	if(thePrototypeID=="")
		return 0;
	mNextID++;
	Instance* anInstance=mPrototypeList.at(thePrototypeID)->MakeInstance(mNextID);
	mInstanceList.push_back(anInstance);
	return anInstance->GetID();
}

Instance* EntityManager::GetInstance(typeInstanceID theEntityID)
{
	if(theEntityID==0)
		return NULL;
	Instance* anInstance;
	std::vector<Instance*>::iterator anInstanceIter;
	for(anInstanceIter=mInstanceList.begin(); anInstanceIter!=mInstanceList.end(); ++anInstanceIter)
	{
		anInstance=*anInstanceIter;
		if(anInstance->GetID()==theEntityID)
		{
			return anInstance;
		}
	}
	return NULL;
}
Prototype* EntityManager::GetPrototype(typePrototypeID thePrototypeID)
{
	if(thePrototypeID=="")
		return NULL;
	if(mPrototypeList.find(thePrototypeID)==mPrototypeList.end())
		return NULL;
	Prototype* anPrototype=mPrototypeList[thePrototypeID];
	return anPrototype;
}
void EntityManager::HandleEvents(sf::Event theEvent)
{
	std::vector<Instance*>::iterator it;
	for(it=mInstanceList.begin(); it!=mInstanceList.end(); ++it)
	{
		(*it)->UpdateInfo();
		(*it)->HandleEvents(theEvent);
	}
}
void EntityManager::UpdateFixed()
{
	std::vector<Instance*>::iterator it;
	for(it=mInstanceList.begin(); it!=mInstanceList.end(); ++it)
	{
		(*it)->UpdateFixed();
	}
}
void EntityManager::UpdateVariable(float theElapsdTime)
{
	std::vector<Instance*>::iterator it;
	for(it=mInstanceList.begin(); it!=mInstanceList.end(); ++it)
	{
		(*it)->UpdateVariable(theElapsdTime);
	}
}
void EntityManager::Draw()
{
	std::vector<Instance*>::iterator it;
	for(it=mInstanceList.begin(); it!=mInstanceList.end(); ++it)
	{
		(*it)->Draw();
	}
}
}
