/**
* Provides a base abstract system for storing entity wide variables.
*
* @file include/GQE/Entity/interfaces/AProperty.hpp
* @author Jacob Dix
* @date 20120416 - Initial Release
* @date 20120417 - Release 1
*/
#ifndef APROPERTY_HPP_INCLUDED
#define APROPERTY_HPP_INCLUDED
#include "GQE/Entities/Entities_types.hpp"
namespace GQE
{
	class GQE_API AProperty
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
		AProperty(std::string theType, std::string theLable=""):
			mLable(theLable),
			mType(theType)
		{

		}
		Type_t* getType()
		{
			return &mType;
		}
		std::string getLable()
		{
			return mLable;
		};
		void setLable(std::string theLable)
		{
			mLable=theLable;
		}
	protected:
		void setType(std::string theType)
		{
			mType=Type_t(theType);
		}
	private:
		std::string mLable;
		Type_t mType;
	};
}
#endif
