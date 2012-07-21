# core library public header include file
set(CORE_HEADER ${INCROOT}/Core.hpp)

# core library additional include files
set(INC
    ${INCROOT}/Core/Core_types.hpp
    ${INCROOT}/Core/assets/ConfigAsset.hpp
    ${INCROOT}/Core/assets/ConfigHandler.hpp
    ${INCROOT}/Core/assets/FontAsset.hpp
    ${INCROOT}/Core/assets/FontHandler.hpp
    ${INCROOT}/Core/assets/ImageAsset.hpp
    ${INCROOT}/Core/assets/ImageHandler.hpp
    ${INCROOT}/Core/assets/MusicAsset.hpp
    ${INCROOT}/Core/assets/MusicHandler.hpp
    ${INCROOT}/Core/assets/SoundAsset.hpp
    ${INCROOT}/Core/assets/SoundHandler.hpp
    ${INCROOT}/Core/classes/AssetManager.hpp
    ${INCROOT}/Core/classes/ConfigReader.hpp
    ${INCROOT}/Core/classes/EventManager.hpp
    ${INCROOT}/Core/classes/PropertyManager.hpp
    ${INCROOT}/Core/classes/StatManager.hpp
    ${INCROOT}/Core/classes/StateManager.hpp
    ${INCROOT}/Core/interfaces/IApp.hpp
    ${INCROOT}/Core/interfaces/IAssetHandler.hpp
    ${INCROOT}/Core/interfaces/IEvent.hpp
    ${INCROOT}/Core/interfaces/ILogger.hpp
    ${INCROOT}/Core/interfaces/IProperty.hpp
    ${INCROOT}/Core/interfaces/IState.hpp
    ${INCROOT}/Core/interfaces/TAsset.hpp
    ${INCROOT}/Core/interfaces/TAssetHandler.hpp
    ${INCROOT}/Core/interfaces/TEvent.hpp
    ${INCROOT}/Core/interfaces/TProperty.hpp
    ${INCROOT}/Core/loggers/Log_macros.hpp
    ${INCROOT}/Core/loggers/FatalLogger.hpp
    ${INCROOT}/Core/loggers/FileLogger.hpp
    ${INCROOT}/Core/loggers/ScopeLogger.hpp
    ${INCROOT}/Core/loggers/StringLogger.hpp
    ${INCROOT}/Core/loggers/onullstream
    ${INCROOT}/Core/states/SplashState.hpp
    ${INCROOT}/Core/utils/StringUtil.hpp
)

# core library source files
set(SRC
    ${SRCROOT}/Core/Core_types.cpp
    ${SRCROOT}/Core/assets/ConfigAsset.cpp
    ${SRCROOT}/Core/assets/ConfigHandler.cpp
    ${SRCROOT}/Core/assets/FontAsset.cpp
    ${SRCROOT}/Core/assets/FontHandler.cpp
    ${SRCROOT}/Core/assets/ImageAsset.cpp
    ${SRCROOT}/Core/assets/ImageHandler.cpp
    ${SRCROOT}/Core/assets/MusicAsset.cpp
    ${SRCROOT}/Core/assets/MusicHandler.cpp
    ${SRCROOT}/Core/assets/SoundAsset.cpp
    ${SRCROOT}/Core/assets/SoundHandler.cpp
    ${SRCROOT}/Core/classes/AssetManager.cpp
    ${SRCROOT}/Core/classes/ConfigReader.cpp
    ${SRCROOT}/Core/classes/EventManager.cpp
    ${SRCROOT}/Core/classes/PropertyManager.cpp
    ${SRCROOT}/Core/classes/StatManager.cpp
    ${SRCROOT}/Core/classes/StateManager.cpp
    ${SRCROOT}/Core/interfaces/IApp.cpp
    ${SRCROOT}/Core/interfaces/IAssetHandler.cpp
    ${SRCROOT}/Core/interfaces/IEvent.cpp
    ${SRCROOT}/Core/interfaces/ILogger.cpp
    ${SRCROOT}/Core/interfaces/IProperty.cpp
    ${SRCROOT}/Core/interfaces/IState.cpp
    ${SRCROOT}/Core/loggers/FatalLogger.cpp
    ${SRCROOT}/Core/loggers/FileLogger.cpp
    ${SRCROOT}/Core/loggers/ScopeLogger.cpp
    ${SRCROOT}/Core/loggers/StringLogger.cpp
    ${SRCROOT}/Core/states/SplashState.cpp
    ${SRCROOT}/Core/utils/StringUtil.cpp
)

# add platform specific sources


# find external SFML libraries
find_package(SFML REQUIRED audio graphics window system)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})

if(BUILD_SHARED_LIBS)
  # define the gqe-core target
  gqe_add_library(gqe-core
                  HEADER        ${CORE_HEADER}
                  HEADER_DIR    ${INCROOT}/Core
                  INCLUDES      ${INC}
                  SOURCES       ${SRC}
                  EXTERNAL_LIBS ${SFML_AUDIO_LIBRARY} ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY})
else()
  # define the gqe-core target (for a static build, we use depends to remove LNK4006 and LNK4221 errors with Visual Studio)
  gqe_add_library(gqe-core
                  HEADER        ${CORE_HEADER}
                  HEADER_DIR    ${INCROOT}/Core
                  INCLUDES      ${INC}
                  SOURCES       ${SRC}
                  DEPENDS       ${SFML_AUDIO_LIBRARY} ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY})
endif()
