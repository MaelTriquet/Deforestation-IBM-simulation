cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MySFMLProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the SFML package
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# Automatically include all .cpp files in the src directory
file(GLOB SOURCES "src/*.cpp")

# Add the executable
add_executable(MySFMLProject ${SOURCES})

# Include directories
target_include_directories(MySFMLProject PRIVATE src)

# Link the SFML libraries
target_link_libraries(MySFMLProject sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

# If you are using additional libraries, link them here
# target_link_libraries(MySFMLProject additional_library)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()