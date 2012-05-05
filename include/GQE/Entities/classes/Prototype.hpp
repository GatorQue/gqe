#ifndef PROTOTYPE_HPP_INCLUDED
#define PROTOTYPE_HPP_INCLUDED
#include "GQE/Entities/classes/Entity.hpp"
namespace GQE
{
	class Prototype : public Entity
	{
	public:
		Prototype(typePrototypeID thePrototypeID);
		~Prototype();
		typePrototypeID GetID();
		Instance* MakeInstance(typeInstanceID theID);
	private:
		typePrototypeID mID;
	};
}

#endif