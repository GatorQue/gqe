
set(SRCROOT ${CMAKE_SOURCE_DIR}/examples/demo)

# all source files
set(SRC ${SRCROOT}/main.cpp)

# find external SFML libraries
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML REQUIRED main)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})

# define the demo target
gqe_add_example(demo GUI_APP
                 SOURCES ${SRC}
                 DEPENDS gqe-core)
