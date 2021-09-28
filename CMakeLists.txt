cmake_minimum_required(VERSION 3.0.2)
project(dogrun)

find_package(catkin REQUIRED COMPONENTS
  OpenCV
  cv_bridge
  image_transport
  roscpp
  sensor_msgs
  geometry_msgs
)

include_directories(${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_executable(recognize_ball src/circle.cpp)
target_link_libraries(recognize_ball ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

#これ
add_executable(twist_serial src/twist_serial.cpp)
target_link_libraries(twist_serial ${catkin_LIBRARIES})

#target_link_libraries(twist_serial ${CMAKE_SOURCE_DIR}/SerialBridge/bin/libSerialBridge.a)
#target_include_directories(twist_serial PRIVATE ${CMAKE_SOURCE_DIR}/SerialBridge/bin/libSerialBridge.a)

target_link_libraries(twist_serial /home/ishi14/catkin_ws/src/dogrun/SerialBridge/bin/libSerialBridge.a)
target_include_directories(twist_serial PRIVATE /home/ishi14/catkin_ws/src/dogrun/SerialBridge/src)