cmake_minimum_required(VERSION 2.8.3)
project(ardrone_joystick_control)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  sensor_msgs
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES ardrone_joystick_control
#  CATKIN_DEPENDS geometry_msgs roscpp sensos_msgs std_msgs
#  DEPENDS system_lib
)


include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(control_joy_to_parrot src/control_joy_to_parrot.cpp)
target_link_libraries(control_joy_to_parrot ${catkin_LIBRARIES})
add_dependencies(control_joy_to_parrot ardrone_joystick_conrol)
