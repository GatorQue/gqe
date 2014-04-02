#include <GQE/Entity/classes/SystemManager.hpp>
#include <GQE/Entity/interfaces/ISystem.hpp>
namespace GQE
{
  /// Single instance of the most recently created App class
  SystemManager* SystemManager::gSystemManager = NULL;

  SystemManager::SystemManager()
  {
    gSystemManager=this;
  }
  SystemManager::~SystemManager()
  {

  }
  void SystemManager::TriggerEvent(typeSystemID theSystemId, std::string theEventId,void* theContext)
  {
    if(mSystems.find(theSystemId)!=mSystems.end())
    {
      GQE::ISystem* anSystem=mSystems[theSystemId];
      if(theContext==NULL)
        anSystem->mEventManager.Get(theEventId)->DoEvent();
      else
        anSystem->mEventManager.Get(theEventId)->DoEvent(theContext);
    }
  }

  void SystemManager::AddSystem(GQE::ISystem* theSystem, typeSystemID theSystemId)
  {
    //santity check theSystem Variable
    if(theSystem!=NULL)
    {
      std::string anSystemId;
      if(theSystemId=="")
        anSystemId=theSystem->GetID();
      else
        anSystemId=theSystemId;
      mSystems[anSystemId]=theSystem;
    }
  }
  SystemManager* SystemManager::GetSystemManager(void)
  {
    return gSystemManager;
  }
}