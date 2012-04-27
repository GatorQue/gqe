/**
 * Provides the GQE Entities typedef's and forward class declarations.
 *
 * @file include/GQE/Entities/Entities_types.hpp
 * @author Jacob Dix
 * @date 20120426 - Initial Release
 */
#ifndef ENTITY_TYPES_HPP_INCLUDED
#define ENTITY_TYPES_HPP_INCLUDED

#include <string>
#include <GQE/Config.hpp>
//#include <GQE/Core.hpp>

namespace GQE
{
  // Forward declare GQE Entities interfaces provided
  class IComponent;

  // Forward declare GQE Entities classes provided
  class Entity;

  /// Declare Component ID typedef which is used for identifying Component objects
  typedef std::string typeComponentID;

  /// Declare Entity ID typedef which is used for identifying Entity objects
  typedef std::string typeEntityID;

  //Helper Funtions
  void setInt(Entity* theEntity, std::string theLable, int theValue);
  int getInt(Entity* theEntity, std::string theLable, int theDefault);

  void setString(Entity* theEntity, std::string theLable, std::string theValue);
  std::string getString(Entity* theEntity, std::string theLable, std::string theDefault);
}
#endif
