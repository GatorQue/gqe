/**
 * Provides the GQE Core internal global or static variable declarations.
 *
 * @file src/GQE/Core/Core_types.cpp
 * @author Ryan Lindeman
 * @date 20110608 - Initial Release
 */
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/loggers/onullstream>

namespace GQE
{
  // Internal Core global variables
  GQE_API App*             gApp = NULL;
  GQE_API ILogger*         gLogger = NULL;
  GQE_API std::onullstream gNullStream;
} // namespace GQE
