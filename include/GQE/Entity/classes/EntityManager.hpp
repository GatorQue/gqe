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
#include <GQE/Entity/classes/Entity.hpp>

namespace GQE
{
	class GQE_API EntityManager
	{
	public:

		EntityManager();

		///Manager Functions
		void AddPrototype(Prototype* thePrototype);

		typeInstanceID AddInstance(const typePrototypeID thePrototypeID);

		Instance* GetInstance(const typeInstanceID theInstanceID);

		Prototype* GetPrototype(const typePrototypeID thePrototypeID);

		void HandleEvents(sf::Event theEvent);

		void UpdateFixed();

		void UpdateVariable(float theElapstedTime);

		void Draw();

		void HandleCleanup();

	protected:

	private:
		std::vector<Instance*> mInstanceList;
		std::map<const typePrototypeID, Prototype*> mPrototypeList;
		typeInstanceID mNextID;
	};
}

#endif

