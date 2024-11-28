cmake_minimum_required(VERSION 3.13) 
project(keyboard_ctr)
# set(CMAKE_CXX_STANDARD 11)            # 设定C++标准，如果是C，则是CMAKE_C_STANDARD
## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
set(CMAKE_BUILD_TYPE "Debug")	
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
)

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBEVDEV REQUIRED libevdev)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES keyboard_ctr
#  CATKIN_DEPENDS libevdev roscpp std_msgs
#  DEPENDS system_lib
)


## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(/usr/include/libevdev-1.0/)
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)
file(GLOB SRC_FILES "src/*cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBEVDEV REQUIRED libevdev)

target_include_directories(keyboard_ctr PRIVATE
  ${catkin_INCLUDE_DIRS}
  ${LIBEVDEV_INCLUDE_DIRS} 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/keyboard_ctr>)

target_link_libraries(keyboard_ctr PRIVATE
  ${LIBEVDEV_LIBRARIES}
  ${catkin_LIBRARIES})


install(TARGETS keyboard_ctr
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)


