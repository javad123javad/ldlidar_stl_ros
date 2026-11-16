/**
* @file         listen_node.cpp
* @author       LDRobot (marketing1@ldrobot.com)
* @brief         
* @version      0.1
* @date         2022.04.08
* @note          
* @copyright    Copyright (c) 2020  SHENZHEN LDROBOT CO., LTD. All rights reserved.
* Licensed under the MIT License (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License in the file LICENSE
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>

#include <memory>
#include <string>
#include <stdlib.h>

#define RADIAN_TO_DEGREES(angle) ((angle)*180000/3141.59)

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("ldlidar_listen_node");
  auto logger = node->get_logger();

  std::string topic_name = node->declare_parameter<std::string>("topic_name", "scan");
  if (topic_name.empty()) {
    RCLCPP_ERROR(logger, "[ldrobot] [ldlidar_listen_node] input param <topic_name> is null");
    rclcpp::shutdown();
    return EXIT_FAILURE;
  }
  RCLCPP_INFO(logger, "[ldrobot] [ldlidar_listen_node] input param <topic_name> is %s", topic_name.c_str());

  auto sub = node->create_subscription<sensor_msgs::msg::LaserScan>(
    topic_name, rclcpp::SensorDataQoS(),
    [logger](const sensor_msgs::msg::LaserScan::ConstSharedPtr data) {
      RCLCPP_INFO_STREAM(logger, "[ldrobot]------listen lidar message-------");
      unsigned int lens = static_cast<unsigned int>((data->angle_max - data->angle_min) / data->angle_increment);
      RCLCPP_INFO_STREAM(logger, "[ldrobot] angle_min: " << RADIAN_TO_DEGREES(data->angle_min) << " "
            << "angle_max: " << RADIAN_TO_DEGREES(data->angle_max));
      RCLCPP_INFO_STREAM(logger, "[ldrobot] point size: " << data->ranges.size());
      for (unsigned int i = 0; i < lens && i < data->ranges.size(); i++) {
        RCLCPP_INFO_STREAM(logger, "[ldrobot] angle: " << RADIAN_TO_DEGREES((data->angle_min + i * data->angle_increment)) << " "
              << "range: " << data->ranges[i] << " "
              << "intensites: " << data->intensities[i]);
      }
    });
  RCLCPP_INFO(logger, "[ldrobot] start ldlidar message subscribe node");

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}

/********************* (C) COPYRIGHT SHENZHEN LDROBOT CO., LTD *******END OF FILE ********/
