cmake_minimum_required(VERSION 3.5)
project(OgreB)

if(UNIX)
set(CMAKE_CXX_FLAGS -g ${cxxflags})
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


add_executable(rook Rook.cpp)

if(UNIX)
target_include_directories(rook PRIVATE OBJ-Loader/Source)
target_link_libraries(rook OgreBites ${BULLET_LIBRARIES} -lGL -lGLU)
endif(UNIX)

if(MSVC)
target_include_directories(rook PRIVATE OBJ-Loader/Source)
target_link_libraries(rook OgreBites ${BULLET_LIBRARIES})
endif(MSVC)