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
    ///Manager Functions
    Entity* AddPrototype(typePrototypeID theEntityID);
    Entity* AddEntity(Entity* thePrototype);

    Entity* GetEntity(typeEntityID theEntityID);
    Entity* GetPrototype(typePrototypeID theEntityID);

    void HandleEvents(sf::Event theEvent);

    void UpdateFixed();

    void UpdateVariable(float theElapstedTime);

    void Draw();

    void HandleCleanup();

protected:

private:
    std::vector<Entity*> mEntityList;
    std::vector<Entity*> mPrototypeList;
    Uint32 mEntityCount;
};
}

#endif

