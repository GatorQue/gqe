/**
* Provides a base abstract system for storing entity wide variables.
*
* @file include/GQE/Entity/interfaces/IProperty.hpp
* @author Jacob Dix
* @date 20120416 - Initial Release
* @date 20120417 - Release 1
*/
#ifndef IPROPERTY_HPP_INCLUDED
#define IPROPERTY_HPP_INCLUDED

#include <GQE/Entity/Entity_types.hpp>

namespace GQE
{
	class GQE_API IProperty
	{
	public:

		class Type_t
		{
		private:
			std::string mName;
		public:
			explicit Type_t(std::string theName) : mName(theName) {}
			std::string Name() const
			{
				return mName;
			};
		};

		IProperty(std::string theType, const typePropertyID thePropertyID);
    virtual ~IProperty();

		Type_t* getType(void);

		const typePropertyID GetID(void) const;

    virtual void Update() = 0;
		
    virtual IProperty* MakeChild() = 0;
	protected:
		void setType(std::string theType);

  private:
		const typePropertyID mPropertyID;
		Type_t mType;
	};
} // namespace GQE
#endif //IPROPERTY_HPP_INCLUDED
