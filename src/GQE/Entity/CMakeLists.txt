# Entity library public header include file
set(Entity_HEADER ${INCROOT}/Entity.hpp)

# Entity library additional include files
set(INC
    ${INCROOT}/Entity/classes/Entity.hpp
    ${INCROOT}/Entity/classes/Prototype.hpp
    ${INCROOT}/Entity/classes/Instance.hpp
    ${INCROOT}/Entity/classes/EntityManager.hpp
    ${INCROOT}/Entity/interfaces/IComponent.hpp
    ${INCROOT}/Entity/interfaces/IProperty.hpp
    ${INCROOT}/Entity/interfaces/TProperty.hpp
    ${INCROOT}/Entity/Components/RenderComponent.hpp
    ${INCROOT}/Entity/Components/DynamicComponent.hpp
    ${INCROOT}/Entity/Components/CoordComponent.hpp
    ${INCROOT}/Entity/Components/SolidComponent.hpp
    ${INCROOT}/Entity/Entity_types.hpp
)

# Entity library source files
set(SRC
    ${SRCROOT}/Entity/classes/Entity.cpp
    ${SRCROOT}/Entity/classes/Prototype.cpp
    ${SRCROOT}/Entity/classes/Instance.cpp
    ${SRCROOT}/Entity/classes/EntityManager.cpp
    ${SRCROOT}/Entity/interfaces/IProperty.cpp
    ${SRCROOT}/Entity/interfaces/IComponent.cpp
    ${SRCROOT}/Entity/Components/RenderComponent.cpp
    ${SRCROOT}/Entity/Components/DynamicComponent.cpp
    ${SRCROOT}/Entity/Components/CoordComponent.cpp
    ${SRCROOT}/Entity/Components/SolidComponent.cpp
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
