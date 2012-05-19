#ifndef PROTOTYPE_HPP_INCLUDED
#define PROTOTYPE_HPP_INCLUDED

#include "GQE/Entity/interfaces/IEntity.hpp"

namespace GQE
{
	class GQE_API Prototype : public IEntity
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