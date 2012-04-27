/**
 * Provides a system for entities,
 *
 * @file include/GQE/Entity/classes/Entity.hpp>
 * @author Jacob Dix
 * @date 20120418 - Initial Release
 */
#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

#include <map>
#include <vector>
#include <GQE/Entities/interfaces/IComponent.hpp>

namespace GQE
{
  class GQE_API Entity
  {
    public:
      Entity(typeEntityID theEntityID);

      ~Entity();

      typeEntityID GetID();
      AProperty* GetProperty(std::string theLable);

      void SetProperty(AProperty* theProperty);

      void AttachComponent(IComponent* theComponent);

      void DetachComponent(typeComponentID theComponentID);

      void HandleEvents(sf::Event theEvent);

      void UpdateFixed();

      void UpdateVariable(float theElapstedTime);

      void Draw();

      void HandleCleanup();
    private:
      std::map<std::string, AProperty*> mPropertyList;
      std::map<std::string, IComponent*> mComponentList;
      typeEntityID mEntityID;
  };
}

#endif
