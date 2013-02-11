cmake_minimum_required(VERSION 2.8)

project(ARGSS CXX C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Modules")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(ARGSS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(MAIN_FILE "${ARGSS_SRC_DIR}/main.cpp")

set(ARGSS_SUB_DIRS binding)

if(APPLE)
  add_definitions(-D MACOSX=1)
  list(APPEND ARGSS_SUB_DIRS osx)
endif()

if(WIN32)
  add_definitions(-D WIN32=1)
  list(APPEND ARGSS_SUB_DIRS win32)
endif()

if(UNIX)
  list(APPEND ARGSS_SUB_DIRS tools/posix)
endif()

file(GLOB ARGSS_SRCS "${ARGSS_SRC_DIR}/*.cpp")
list(REMOVE_ITEM ARGSS_SRCS "${MAIN_FILE}")

foreach(i ${ARGSS_SUB_DIRS})
  file(GLOB srcs "${ARGSS_SRC_DIR}/${i}/*.cpp")
  list(APPEND ARGSS_SRCS ${srcs})
endforeach()

file(GLOB SOIL_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SOIL/src/*.c")
list(APPEND ARGSS_SRCS ${SOIL_SRCS})

foreach(i OpenGL SDL Freetype SDL_mixer Boost)
  find_package(${i} REQUIRED)
endforeach()

message(STATUS "${CMAKE_INSTALL_PREFIX}")
find_package(Ruby 1.8 REQUIRED)
message(STATUS "ruby version: ${RUBY_VERSION}")

list(APPEND ARGSS_LIBRARIES
  ${RUBY_LIBRARY} ${OPENGL_LIBRARIES}
  ${SDL_LIBRARY} ${FREETYPE_LIBRARIES}
  ${SDL_MIXER_LIBRARIES} ${BOOST_LIBRARIES})

include_directories("${ARGSS_SRC_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/dependencies"
  "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/simpleini"
  "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SOIL/src"
  ${RUBY_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR}
  ${SDL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS}
  ${SDL_MIXER_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS})

add_library(${PROJECT_NAME}_static STATIC ${ARGSS_SRCS})

add_executable(${PROJECT_NAME} MACOSX_BUNDLE "${MAIN_FILE}")
target_link_libraries(${PROJECT_NAME}
  ${PROJECT_NAME}_static ${ARGSS_LIBRARIES})

add_executable(${PROJECT_NAME}_com "${MAIN_FILE}")
target_link_libraries(${PROJECT_NAME}_com
  ${PROJECT_NAME}_static ${ARGSS_LIBRARIES})
