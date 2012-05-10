#include "GQE/Entity/classes/Prototype.hpp"
#include "GQE/Entity/classes/Instance.hpp"
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

}
}