#include "GQE/Entity/classes/Prototype.hpp"
#include "GQE/Entity/classes/Instance.hpp"
namespace GQE
{
  Instance::Instance(const typeInstanceID theInstanceID, Prototype* thePrototype):
    Entity(),
	  mID(theInstanceID),
	  mPrototype(thePrototype)
  {

  }

  Instance::~Instance()
  {

  }

  const typeInstanceID Instance::GetID() const
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