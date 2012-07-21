# Entity library public header include file
set(Entity_HEADER ${INCROOT}/Entity.hpp)

# Entity library additional include files
set(INC
    ${INCROOT}/Entity/systems/ActionSystem.hpp
    ${INCROOT}/Entity/systems/AnimationSystem.hpp
    ${INCROOT}/Entity/systems/MovementSystem.hpp
    ${INCROOT}/Entity/systems/RenderSystem.hpp
    ${INCROOT}/Entity/classes/ActionGroup.hpp
    ${INCROOT}/Entity/classes/PrototypeManager.hpp
    ${INCROOT}/Entity/classes/Instance.hpp
    ${INCROOT}/Entity/classes/Prototype.hpp
    ${INCROOT}/Entity/interfaces/IAction.hpp
    ${INCROOT}/Entity/interfaces/IEntity.hpp
    ${INCROOT}/Entity/interfaces/ISystem.hpp
    ${INCROOT}/Entity/Entity_types.hpp
)

# Entity library source files
set(SRC
    ${SRCROOT}/Entity/systems/ActionSystem.cpp
    ${SRCROOT}/Entity/systems/AnimationSystem.cpp
    ${SRCROOT}/Entity/systems/MovementSystem.cpp
    ${SRCROOT}/Entity/systems/RenderSystem.cpp
    ${SRCROOT}/Entity/classes/ActionGroup.cpp
    ${SRCROOT}/Entity/classes/PrototypeManager.cpp
    ${SRCROOT}/Entity/classes/Instance.cpp
    ${SRCROOT}/Entity/classes/Prototype.cpp
    ${SRCROOT}/Entity/interfaces/IAction.cpp
    ${SRCROOT}/Entity/interfaces/IEntity.cpp
    ${SRCROOT}/Entity/interfaces/ISystem.cpp
    ${SRCROOT}/Entity/Entity_types.cpp
)

# add platform specific sources

# find external SFML libraries
find_package(SFML REQUIRED graphics window system)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})


if(BUILD_SHARED_LIBS)
  # define the gqe-Entity target
  gqe_add_library(gqe-entity
                  HEADER        ${Entity_HEADER}
                  HEADER_DIR    ${INCROOT}/Entity
                  INCLUDES      ${INC}
                  SOURCES       ${SRC}
                  DEPENDS       gqe-core
                  EXTERNAL_LIBS ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY} )
else()
  # define the gqe-Entity target (for a static build, we use depends to remove LNK4006 and LNK4221 errors with Visual Studio)
  gqe_add_library(gqe-entity
                  HEADER        ${Entity_HEADER}
                  HEADER_DIR    ${INCROOT}/Entity
                  INCLUDES      ${INC}
                  SOURCES       ${SRC}
                  DEPENDS       gqe-core
                  DEPENDS       ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY} )
endif()
