#ifndef INSTANCE_HPP_INCLUDED
#define INSTANCE_HPP_INCLUDED
#include "GQE/Entities/classes/Entity.hpp"
namespace GQE
{
	class GQE_API Instance : public Entity
	{
	public:
		Instance(typeInstanceID theInstanceID, Prototype* thePrototype);
		~Instance();
		typeInstanceID GetID();
		Prototype* GetPrototype();
		void UpdateInfo();
	private:
		typeInstanceID mID;
		Prototype* mPrototype;
	};
}

#endif