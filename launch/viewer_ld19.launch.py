import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node


def generate_launch_description():
    share = get_package_share_directory('ldlidar_stl_ros')
    return LaunchDescription([
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(os.path.join(share, 'launch', 'ld19.launch.py'))),
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz_show_ldlidar',
            arguments=['-d', os.path.join(share, 'rviz', 'ldlidar.rviz')],
        ),
    ])
