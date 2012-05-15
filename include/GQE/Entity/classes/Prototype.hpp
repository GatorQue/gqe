#ifndef PROTOTYPE_HPP_INCLUDED
#define PROTOTYPE_HPP_INCLUDED

#include <GQE/Entity/classes/Entity.hpp>

namespace GQE
{
	class GQE_API Prototype : public Entity
	{
	  public:
		  Prototype(const typePrototypeID thePrototypeID);
		  ~Prototype();
		  const typePrototypeID GetID(void) const;
		  Instance* MakeInstance(typeInstanceID theID);
	  private:
		  const typePrototypeID mPrototypeID;
	};
}

#endif // PROTOTYPE_HPP_INCLUDED