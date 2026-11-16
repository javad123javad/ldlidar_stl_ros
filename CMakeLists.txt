cmake_minimum_required(VERSION 3.14)
project(ldlidar_stl_ros)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(Threads REQUIRED)

file(GLOB LDLIDAR_DRIVER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/include/ldlidar_driver/*.cpp)

add_executable(${PROJECT_NAME}_node
  src/publish_node/main.cpp
  ${LDLIDAR_DRIVER_SRC}
)
target_include_directories(${PROJECT_NAME}_node PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ldlidar_driver/include
)
target_link_libraries(${PROJECT_NAME}_node
  rclcpp::rclcpp
  ${sensor_msgs_TARGETS}
  Threads::Threads
)

add_executable(${PROJECT_NAME}_listen_node src/listen_node/listen_node.cpp)
target_link_libraries(${PROJECT_NAME}_listen_node
  rclcpp::rclcpp
  ${sensor_msgs_TARGETS}
)

install(TARGETS ${PROJECT_NAME}_node ${PROJECT_NAME}_listen_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch rviz
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
