#include <GQE/Entity/interfaces/IProperty.hpp>

namespace GQE
{
    IProperty::IProperty(std::string theType, const typePropertyID thePropertyID) :
		  mPropertyID(thePropertyID),
			mType(theType)
		{
		}

    IProperty::~IProperty()
    {
    }

		IProperty::Type_t* IProperty::getType(void)
		{
			return &mType;
		}

		const typePropertyID IProperty::GetID(void) const
		{
			return mPropertyID;
		}

		void IProperty::setType(std::string theType)
		{
			mType = Type_t(theType);
		}
}