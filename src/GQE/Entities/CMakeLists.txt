# entities library public header include file
set(ENTITIES_HEADER ${INCROOT}/Entities.hpp)

# entities library additional include files
set(INC
    ${INCROOT}/Entities/classes/CollisionPolygon.hpp
    ${INCROOT}/Entities/classes/ObjectManager.hpp
    ${INCROOT}/Entities/interfaces/IObject.hpp
    ${INCROOT}/Entities/Entities_types.hpp
)

# entities library source files
set(SRC
    ${SRCROOT}/Entities/classes/CollisionPolygon.cpp
    ${SRCROOT}/Entities/classes/ObjectManager.cpp
    ${SRCROOT}/Entities/interfaces/IObject.cpp
)

# add platform specific sources

# find external SFML libraries
find_package(SFML REQUIRED graphics window system)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})

if(BUILD_SHARED_LIBS)
  # define the gqe-entities target
  gqe_add_library(gqe-entities
                  HEADER        ${ENTITIES_HEADER}
                  HEADER_DIR    ${INCROOT}/Entities
                  INCLUDES      ${INC}
                  SOURCES       ${SRC}
                  DEPENDS       gqe-core
                  EXTERNAL_LIBS ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY} )
else()
  # define the gqe-entities target (for a static build, we use depends to remove LNK4006 and LNK4221 errors with Visual Studio)
  gqe_add_library(gqe-entities
                  HEADER        ${ENTITIES_HEADER}
                  HEADER_DIR    ${INCROOT}/Entities
                  INCLUDES      ${INC}
                  SOURCES       ${SRC}
                  DEPENDS       gqe-core
                  DEPENDS       ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY} )
endif()
