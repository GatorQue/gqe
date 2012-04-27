/**
 * Provides a system for Managing entities,
 *
 * @file include/GQE/Entity/classes/EntityManager.hpp>
 * @author Jacob Dix
 * @date 20120425 Initial Release
 */
#ifndef ENTITY_MANAGER_HPP_INCLUDED
#define ENTITY_MANAGER_HPP_INCLUDED

#include <map>
#include <vector>
#include <GQE/Entities/classes/Entity.hpp>

namespace GQE
{
  class GQE_API EntityManager
  {
    public:
      //Manager Functions
      void AddEntity(typeEntityID theEntityID, int theNumber=1);

      AProperty* GetProperty(typeEntityID theEntityID, std::string theLable);

      void SetProperty(typeEntityID theEntityID, AProperty* theProperty);

      void AttachComponent(typeEntityID theEntityID, IComponent* theComponent);

      void DetachComponent(typeEntityID theEntityID, typeComponentID theComponentID);

      void HandleEvents(sf::Event theEvent);

      void UpdateFixed();

      void UpdateVariable(float theElapstedTime);

      void Draw();

      void HandleCleanup();

    protected:
      Entity* GetEntity(typeEntityID);
    private:
      std::vector<Entity*> mEntityList;

  };
}

#endif

