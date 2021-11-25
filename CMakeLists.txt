# CMakeList.txt : CMake project for CommandEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("CommandEngine")

# Add source to this project's executable.
add_executable (CommandEngine "CommandEngine.cpp" "CommandEngine.hpp" "Engine.hpp" "Wrapper.hpp" )

# TODO: Add tests and install targets if needed.
