
# Examples will be importing from GQE so we need to remove this definition
remove_definitions(-DGQE_EXPORTS)

# add the examples subdirectories
add_subdirectory(demo)
