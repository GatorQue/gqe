#ifndef ENTITY_TYPES_HPP_INCLUDED
#define ENTITY_TYPES_HPP_INCLUDED
#include <string>
#include "GQE/Core.hpp"
namespace GQE
{
	//forward declarations
	class IComponent;
	class Entity;
	//Structures
	typedef std::string typeComponentID;
	typedef std::string typeEntityID;
	//Helper Funtions
	void setInt(Entity* theEntity, std::string theLable, int theValue);
	int getInt(Entity* theEntity, std::string theLable, int theDefault);

	void setString(Entity* theEntity, std::string theLable, std::string theValue);
	std::string getString(Entity* theEntity, std::string theLable, std::string theDefault);
}
#endif
