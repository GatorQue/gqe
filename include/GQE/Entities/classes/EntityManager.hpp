/**
* Provides a system for Managing entities,
*
* @file include/GQE/Entity/classes/EntityManager.hpp>
* @author Jacob Dix
* @date 20120425 Initial Release
*/
#ifndef ENTITY_MANAGER_HPP_INCLUDED
#define ENTITY_MANAGER_HPP_INCLUDED
#include "GQE/Entities/interfaces/Entity.hpp"
#include <map>
#include <vector>
namespace GQE
{
	class GQE_API EntityManager
	{
	public:
		//Manager Functions
		static void AddEntity(typeEntityID theEntityID, int theNumber=1);

		static AProperty* GetProperty(typeEntityID theEntityID, std::string theLable);

		static void SetProperty(typeEntityID theEntityID, AProperty* theProperty);

		static void AttachComponent(typeEntityID theEntityID, IComponent* theComponent);

		static void DetachComponent(typeEntityID theEntityID, typeComponentID theComponentID);

		static void HandleEvents(sf::Event theEvent);

		static void UpdateFixed();

		static void UpdateVariable(float theElapstedTime);

		static void Draw();

		static void HandleCleanup();


	private:
		static std::vector<Entity*> mEntityList;
	};
}

#endif

