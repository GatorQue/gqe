#ifndef PROTOTYPE_HPP_INCLUDED
#define PROTOTYPE_HPP_INCLUDED
#include "GQE/Entity/classes/Entity.hpp"
namespace GQE
{
	class GQE_API Prototype : public Entity
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