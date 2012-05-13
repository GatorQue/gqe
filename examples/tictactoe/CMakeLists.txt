project(TicTacToe)

set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

# all source files
set(SRC
 ${SRCROOT}/main.cpp
 ${SRCROOT}/TicTacToeApp.hpp
 ${SRCROOT}/TicTacToeApp.cpp
 ${SRCROOT}/GameState.hpp
 ${SRCROOT}/GameState.cpp)

# find external SFML libraries
if(WINDOWS)
  find_package(SFML REQUIRED main)
endif(WINDOWS)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})

# define the demo target
gqe_add_example(tictactoe GUI_APP
                SOURCES ${SRC}
                DEPENDS gqe-core)
