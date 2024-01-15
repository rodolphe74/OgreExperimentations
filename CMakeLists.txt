cmake_minimum_required(VERSION 3.0)
project(OgreB)

set(CMAKE_CXX_FLAGS -g ${cxxflags})

find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

find_package(Bullet REQUIRED)
include_directories(${BULLET_INCLUDE_DIR})

add_executable(rigidSimul rigidSimul.cpp)
target_link_libraries(rigidSimul OgreBites ${BULLET_LIBRARIES} -lGL -lGLU)

