#include <GQE/Entity/classes/EntityManager.hpp>
#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/classes/Instance.hpp>
namespace GQE
{
	EntityManager::EntityManager() :
    mNextID(0)
  {

  }

  void EntityManager::AddPrototype(Prototype* thePrototype)
  {
	  if(thePrototype==NULL)
		  return;
	  mPrototypeList[thePrototype->GetID()]=thePrototype;
  }

  typeInstanceID EntityManager::AddInstance(const typePrototypeID thePrototypeID)
  {
    typeInstanceID anResult = 0;
    if(mPrototypeList.find(thePrototypeID) != mPrototypeList.end())
    {
	    Instance* anInstance =
        mPrototypeList.at(thePrototypeID)->MakeInstance(++mNextID);
	    mInstanceList.push_back(anInstance);
      anResult = anInstance->GetID();
    }

    // Return anResult found/created or 0 if none was created
	  return anResult;
  }

  Instance* EntityManager::GetInstance(const typeInstanceID theEntityID)
  {
    Instance* anResult = NULL;

	  std::vector<Instance*>::iterator anInstanceIter;
	  for(anInstanceIter=mInstanceList.begin(); anInstanceIter!=mInstanceList.end(); ++anInstanceIter)
	  {
		  Instance* anInstance = *anInstanceIter;
		  if(anInstance->GetID() == theEntityID)
		  {
        anResult = anInstance;
        break;
		  }
	  }

    // Return anResult found (which might be NULL if none was found)
	  return anResult;
  }

  Prototype* EntityManager::GetPrototype(const typePrototypeID thePrototypeID)
  {
    Prototype* anResult = NULL;

	  if(mPrototypeList.find(thePrototypeID)!=mPrototypeList.end())
    {
  	  anResult = mPrototypeList[thePrototypeID];
    }

    // Return anResult found (which might be NULL if none was found)
	  return anResult;
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
