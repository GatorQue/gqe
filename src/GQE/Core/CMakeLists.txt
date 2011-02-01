set(INCROOT ${CMAKE_SOURCE_DIR}/include/GQE/Core)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/GQE/Core)

# all source files
set(SRC
    ${SRCROOT}/assets/FontAsset.cpp
    ${INCROOT}/assets/FontAsset.hpp
    ${SRCROOT}/assets/ImageAsset.cpp
    ${INCROOT}/assets/ImageAsset.hpp
    ${SRCROOT}/assets/MusicAsset.cpp
    ${INCROOT}/assets/MusicAsset.hpp
    ${SRCROOT}/assets/SoundAsset.cpp
    ${INCROOT}/assets/SoundAsset.hpp
    ${SRCROOT}/classes/App.cpp
    ${INCROOT}/classes/App.hpp
    ${SRCROOT}/classes/AssetManager.cpp
    ${INCROOT}/classes/AssetManager.hpp
    ${SRCROOT}/classes/ConfigReader.cpp
    ${INCROOT}/classes/ConfigReader.hpp
    ${SRCROOT}/classes/StatManager.cpp
    ${INCROOT}/classes/StatManager.hpp
    ${SRCROOT}/classes/StateManager.cpp
    ${INCROOT}/classes/StateManager.hpp
    ${SRCROOT}/states/MenuState.cpp
    ${INCROOT}/states/MenuState.hpp
    ${SRCROOT}/states/SplashState.cpp
    ${INCROOT}/states/SplashState.hpp
)

# add platform specific sources


# find external SFML libraries
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML REQUIRED audio graphics window system)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})

# build the list of external libraries to link
if(BUILD_SHARED_LIBS)
  set(CORE_EXT_LIBS
    ${SFML_AUDIO_LIBRARY}
    ${SFML_GRAPHICS_LIBRARY}
    ${SFML_SYSTEM_LIBRARY}
    ${SFML_WINDOW_LIBRARY}
  )
endif()

# define the gqe-core target
gqe_add_library(gqe-core
                SOURCES ${SRC}
                EXTERNAL_LIBS ${CORE_EXT_LIBS})
