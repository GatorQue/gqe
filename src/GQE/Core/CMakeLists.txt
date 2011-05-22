# core library public header include file
set(HEADER ${INCROOT}/Core.hpp)

# core library additional include files
set(INC
    ${INCROOT}/Core/assets/ConfigAsset.hpp
    ${INCROOT}/Core/assets/FontAsset.hpp
    ${INCROOT}/Core/assets/ImageAsset.hpp
    ${INCROOT}/Core/assets/MusicAsset.hpp
    ${INCROOT}/Core/assets/SoundAsset.hpp
    ${INCROOT}/Core/classes/App.hpp
    ${INCROOT}/Core/classes/AssetManager.hpp
    ${INCROOT}/Core/classes/ConfigReader.hpp
    ${INCROOT}/Core/classes/StatManager.hpp
    ${INCROOT}/Core/classes/StateManager.hpp
    ${INCROOT}/Core/states/MenuState.hpp
    ${INCROOT}/Core/states/SplashState.hpp
    ${INCROOT}/Core/interfaces/IState.hpp
    ${INCROOT}/Core/interfaces/TAsset.hpp
    ${INCROOT}/Core/Core_types.hpp
)

# core library source files
set(SRC
    ${SRCROOT}/Core/assets/ConfigAsset.cpp
    ${SRCROOT}/Core/assets/FontAsset.cpp
    ${SRCROOT}/Core/assets/ImageAsset.cpp
    ${SRCROOT}/Core/assets/MusicAsset.cpp
    ${SRCROOT}/Core/assets/SoundAsset.cpp
    ${SRCROOT}/Core/classes/App.cpp
    ${SRCROOT}/Core/classes/AssetManager.cpp
    ${SRCROOT}/Core/classes/ConfigReader.cpp
    ${SRCROOT}/Core/classes/StatManager.cpp
    ${SRCROOT}/Core/classes/StateManager.cpp
    ${SRCROOT}/Core/states/MenuState.cpp
    ${SRCROOT}/Core/states/SplashState.cpp
)

# add platform specific sources


# find external SFML libraries
find_package(SFML REQUIRED audio graphics window system)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})

# define the gqe-core target
gqe_add_library(gqe-core
                HEADER        ${HEADER}
                HEADER_DIR    ${INCROOT}/Core
                INCLUDES      ${INC}
                SOURCES       ${SRC}
                EXTERNAL_LIBS ${SFML_AUDIO_LIBRARY} ${SFML_GRAPHICS_LIBRARY} ${SFML_SYSTEM_LIBRARY} ${SFML_WINDOW_LIBRARY})
                
#add_library(gqe-core SHARED ${HEADER} ${INC} ${SRC})

#target_link_libraries(gqe-core ${SFML_AUDIO_LIBRARY} ${SFML_GRAPHICS_LIBRARY} ${SFML_SYSTEM_LIBRARY} ${SFML_WINDOW_LIBRARY})
 
# set target public header file for this library
#set_target_properties(gqe-core PROPERTIES PUBLIC_HEADER "${HEADER}")

# install this library
#install(TARGETS gqe-core
#  # IMPORTANT: Add the gqe-core library to the "export-set"
#  EXPORT GQE_LibraryDepends
#  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
#  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
#  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/GQE" COMPONENT dev)

# install Core library include files
#install(DIRECTORY ${INCROOT}/Core
#        DESTINATION ${INSTALL_INCLUDE_DIR}/GQE
#        COMPONENT dev
#        PATTERN ".svn" EXCLUDE)
