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
#include "Entity_types.hpp"
#include "interfaces/AProperty.hpp"
#include <typeinfo>
template<class TYPE=unsigned int>
class TProperty : public AProperty
{
public:
    TProperty():
        AProperty(typeid(TProperty<TYPE>).name())
    {

    }
    TProperty(std::string theLable):
        AProperty(typeid(TYPE).name(),theLable)
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
private:
    TYPE mValue;
};
#endif
