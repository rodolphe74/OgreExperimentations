cmake_minimum_required(VERSION 3.0)
project(OgreB)

if(UNIX)
set(CMAKE_CXX_FLAGS -O ${cxxflags})
endif(UNIX)

find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

find_package(Bullet REQUIRED)
include_directories(${BULLET_INCLUDE_DIR})

add_executable(rigidSimul RigidSimul.cpp)

if(UNIX)
target_link_libraries(rigidSimul OgreBites ${BULLET_LIBRARIES} -lGL -lGLU)
endif(UNIX)

if(MSVC)
target_link_libraries(rigidSimul OgreBites ${BULLET_LIBRARIES})
endif(MSVC)
