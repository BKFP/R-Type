##
## EPITECH PROJECT, 2023
## R-Type
## File description:
## CMakeLists client
##

# Project Files
file(GLOB CLIENT_FILES "src/*.cpp")
include_directories("./includes")

# Executable
add_executable(r-type_client ${CLIENT_FILES})

# Linking Dependencies
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(r-type_client PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
