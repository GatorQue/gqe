
# Examples will be importing from GQE so we need to remove this definition
remove_definitions(-DGQE_EXPORTS)

# add each example
add_subdirectory(spacedots)
add_subdirectory(tictactoe)
