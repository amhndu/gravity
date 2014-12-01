cmake_minimum_required(VERSION 2.6)
project(gravity)

# Specify C++11 flag for g++
if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
endif()

# Add directory containing FindSFML.cmake to module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}")

# Add sources
set(SOURCES
    "${PROJECT_SOURCE_DIR}/main.cpp"
    "${PROJECT_SOURCE_DIR}/Game.cpp"
    "${PROJECT_SOURCE_DIR}/Satellite.cpp"
    "${PROJECT_SOURCE_DIR}/obbcollision.cpp" )
# Find SFML
find_package(SFML 2 COMPONENTS graphics window system main)

if(SFML_FOUND)
        include_directories(${SFML_INCLUDE_DIR})
else()
        set(SFML_ROOT "" CACHE PATH "SFML top-level directory")
        message("\n-> SFML directory not found. Set SFML_ROOT to SFML's top-level path (containing \"include\" and \"lib\" directories).")
        message("-> Make sure the SFML libraries with the same configuration (Release/Debug, Static/Dynamic) exist.\n")
endif()

add_executable(gravity ${SOURCES})

target_link_libraries(gravity ${SFML_LIBRARIES})
