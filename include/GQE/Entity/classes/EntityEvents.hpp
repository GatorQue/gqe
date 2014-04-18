/**
 * Provides the EntityEvent class for handling basic event functions for entitys.
 * Can be inhaireted to add custom game specific events.
 *
 * @file include/GQE/Entity/classes/EntityEvents.hpp
 * @author Jacob Dix
 * @date 20140410 - Initial Release
*/
#ifndef ENTITY_EVENTS_HPP_INCLUDED
#define ENTITY_EVENTS_HPP_INCLUDED
#include <GQE/Entity/Entity_types.hpp>
namespace GQE
{
  class GQE_API EntityEvents
  {
  public:
    EntityEvents(IApp& theApp);
    virtual void RegisterEvents();
    void EventDestroyEntity(PropertyManager* theContext);
  protected:
    IApp& mApp;
  };
}
#endif
