#include "GQE/Entities/classes/Prototype.hpp"
#include "GQE/Entities/classes/Instance.hpp"
namespace GQE
{
	Instance::Instance(typeInstanceID theInstanceID, Prototype* thePrototype):
	Entity(),
	mID(theInstanceID),
	mPrototype(thePrototype)
	{

	}
	Instance::~Instance()
	{

	}
	typeInstanceID Instance::GetID()
	{
		return mID;
	}
	Prototype* Instance::GetPrototype()
	{
		return mPrototype;
	}
	void Instance::UpdateInfo()
	{
		std::map<std::string, AProperty*>::iterator it;
		for(it=mPropertyList.begin();it!=mPropertyList.end();++it)
		{
			(*it->second).Update();
		}
	}
}