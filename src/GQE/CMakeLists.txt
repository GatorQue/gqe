# include the GQE specific macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# add the GQE sources path
include_directories(${CPROJECT_SOURCE_DIR}/src)

# set the output directory for GQE libraries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# set the source directory for include and source files
set(INCROOT ${PROJECT_SOURCE_DIR}/include/GQE)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/GQE)

# GQE needs to export symbols when build so define the export flag
add_definitions(-DGQE_EXPORTS)

# add the modules subdirectories
add_subdirectory(Core)
add_subdirectory(Entity)

# install Config header include file
install(FILES ${INCROOT}/Config.hpp
        DESTINATION include/GQE
        COMPONENT dev)
