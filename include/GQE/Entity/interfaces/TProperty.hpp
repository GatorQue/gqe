/**
* Provides a system for storing entity wide variables.
*
* @file include/GQE/Entity/interfaces/TProperty.hpp
* @author Jacob Dix
* @date 20120416 - Initial Release
* @date 20120417 - Release 1
*/
#ifndef TPROPERTY_HPP_INCLUDED
#define TPROPERTY_HPP_INCLUDED

#include <GQE/Entity/Entity_types.hpp>
#include <GQE/Entity/interfaces/IProperty.hpp>
#include <typeinfo>

namespace GQE
{
	template<class TYPE=unsigned int>
	class GQE_API TProperty : public IProperty
	{
	public:
		TProperty():
				IProperty(typeid(TProperty<TYPE>).name())
				{

				}
				TProperty(std::string theLable):
				IProperty(typeid(TYPE).name(),theLable)
				{

				}
				TYPE getValue()
				{
					return mValue;
				}
				void setValue(TYPE theValue)
				{
					mValue=theValue;
				}
				void Update()
				{

				}
				IProperty* MakeChild()
				{
					TProperty<TYPE>* anProperty=new TProperty<TYPE>(GetID());
					anProperty->setValue(mValue);
					return anProperty;
				}
	private:
		TYPE mValue;
	};
}
#endif
