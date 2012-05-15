#ifndef INSTANCE_HPP_INCLUDED
#define INSTANCE_HPP_INCLUDED
#include "GQE/Entity/classes/Entity.hpp"
namespace GQE
{
	class GQE_API Instance : public Entity
	{
	public:
		Instance(const typeInstanceID theInstanceID, Prototype* thePrototype);
		virtual ~Instance();
		const typeInstanceID GetID() const;
		Prototype* GetPrototype();
		void UpdateInfo();
	private:
		const typeInstanceID mID;
		Prototype* mPrototype;
	};
}

#endif