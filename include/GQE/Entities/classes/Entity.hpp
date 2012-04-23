/**
* Provides a system for entities,
*
* @file include/GQE/Entity/classes/Entity.hpp>
* @author Jacob Dix
* @date 20120418 - Initial Release
*/
#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED
#include "interfaces/IComponent.hpp"
#include <map>
#include <vector>
class Entity
{
public:
    Entity(typeEntityID theEntityID);

    ~Entity();

    std::string GetID();

    AProperty* GetProperty(std::string theKey);

    void SetProperty(AProperty* theProperty);

    void AttachComponent(IComponent* theComponent);

    void DetachComponent(typeComponentID theComponentID);

    //void HandleEvents(sf::Event theEvent);

    void UpdateFixed();

    void UpdateVariable(float theElapstedTime);

    void Draw();

    void HandleCleanup();

	//Static Manager Functions
	static void AddEntity(typeEntityID theEntityID);
	//static void AllHandleEvents(sf::Event theEvent);
	static void AllUpdateFixed();
	static void AllUpdateVariable(float theElapsedTime);
	static void AllDraw();
private:
    std::map<std::string, AProperty*> mPropertyList;
    std::map<std::string, IComponent*> mComponentList;
    typeEntityID mEntityID;
	static std::vector<Entity*> mEntityList;
};

#endif
