/**
 * Provides the EntityEvent class for handling basic event functions for entitys.
 * Can be inhaireted to add custom game specific events.
 *
 * @file src/GQE/Entity/classes/EntityEvents.cpp
 * @author Jacob Dix
 * @date 20140410 - Initial Release
*/
#include <GQE/Entity/classes/EntityEvents.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>
namespace GQE
{
  EntityEvents::EntityEvents(IApp& theApp) :
  mApp(theApp)
  {

  }
  void EntityEvents::RegisterEvents()
  {
    mApp.mEventManager.Add<EntityEvents, PropertyManager>("DestroyEntity", *this, &EntityEvents::EventDestroyEntity);
  }
  void EntityEvents::EventDestroyEntity(PropertyManager* theContext)
  {
    IEntity* anEntity=theContext->Get<IEntity*>("Entity");
    if(anEntity!=NULL)
      anEntity->Destroy();
  }
}
