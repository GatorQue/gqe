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

#include <GQE/Entities/Entities_types.hpp>
#include <GQE/Entities/interfaces/AProperty.hpp>
#include <typeinfo>

namespace GQE
{
  template<class TYPE=unsigned int>
    class GQE_API TProperty : public AProperty
  {
    public:
	TProperty(AProperty* theParent):
        AProperty(typeid(TProperty<TYPE>).name(),theParent)
    {

    }
      TProperty(AProperty* theParent,std::string theLable):
        AProperty(typeid(TYPE).name(),theParent,theLable)
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
		  if(mParent==NULL)
			  return;
		  if(mParent->getType()->Name()!=typeid(TYPE).name())
			  return;
		  TProperty<TYPE>* theParent=static_cast<TProperty<TYPE>*>(mParent);
		  setValue(theParent->getValue());
	  }
	  AProperty* MakeChild()
	  {
		  TProperty<TYPE> anProperty(this,getLable());
		  anProperty.setValue(mValue);
		  return &anProperty;
	  }
    private:
      TYPE mValue;
  };
}
#endif
