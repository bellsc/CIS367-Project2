cmake_minimum_required(VERSION 2.8.4)
project(Proj2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (NOT WIN32)
set(CMAKE_MODULE_PATH /usr/local/lib/cmake /usr/local/lib/x86_64-linux-gnu/cmake)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw)
endif (NOT WIN32)

find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)
if (WIN32)
  include_directories("C:/GLFW/include" "C:/glm" "C:/GLEW/include")
  link_directories("C:/GLFW/lib-mingw" "C:/GLEW/lib/Release/x64")
else (WIN32)
  find_package (glfw3 REQUIRED)
  find_package (GLM REQUIRED)
  find_package (GLEW REQUIRED STATIC)
endif (WIN32)
if (APPLE)
  include_directories(/usr/local/include)
  find_library(COCOA_LIBRARY Cocoa REQUIRED)
  find_library(IOKIT_LIBRARY IOKit REQUIRED)
  find_library(COREVID_LIBRARY CoreVideo REQUIRED)
endif (APPLE)

set(SOURCE_FILES main.cpp WheelTemp.cpp Shapes/Ring.cpp Shapes/TruncCone.cpp Shapes/UnitCube.cpp)
add_executable(Proj2 ${SOURCE_FILES})


  target_link_libraries (Proj2 ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} ${COCOA_LIBRARY}
     ${COREVID_LIBRARY} ${IOKIT_LIBRARY} ${GLEW_LIBRARY})
if (WIN32)
  target_link_libraries (Proj2 ${OPENGL_LIBRARIES} glfw3 glew32s glu32 opengl32)
endif (WIN32)

if (UNIX)
  target_link_libraries (Proj2 ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY} ${GLEW_LIBRARY})
endif (UNIX)




