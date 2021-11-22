cmake_minimum_required(VERSION 3.0.2)
project(dogrun)

find_package(catkin REQUIRED COMPONENTS
  OpenCV
  cv_bridge
  image_transport
  roscpp
  sensor_msgs
  geometry_msgs
  tf
  message_generation
)

add_message_files(
  FILES
  flag.msg
  Dog.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS roscpp std_msgs message_runtime
)

include_directories(${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_executable(circletest src/circle.cpp)
target_link_libraries(circletest ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(twist_serial src/twist_serial.cpp)
target_link_libraries(twist_serial ${catkin_LIBRARIES})

target_link_libraries(twist_serial /home/ishi14/catkin_ws/src/dogrun/SerialBridge/bin/libSerialBridge.a)
target_include_directories(twist_serial PRIVATE /home/ishi14/catkin_ws/src/dogrun/SerialBridge/src)

add_executable(bowwow src/bowwow.cpp)
target_link_libraries(bowwow ${catkin_LIBRARIES})

add_executable(joy_flag src/joyFlagger.cpp)
target_link_libraries(joy_flag ${catkin_LIBRARIES})