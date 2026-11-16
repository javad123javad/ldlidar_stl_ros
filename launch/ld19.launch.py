from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    port_name = LaunchConfiguration('port_name')
    frame_id = LaunchConfiguration('frame_id')
    topic_name = LaunchConfiguration('topic_name')

    return LaunchDescription([
        DeclareLaunchArgument('port_name', default_value='/dev/ttyUSB0'),
        DeclareLaunchArgument('frame_id', default_value='base_laser'),
        DeclareLaunchArgument('topic_name', default_value='scan'),

        # ldlidar message publisher node
        Node(
            package='ldlidar_stl_ros',
            executable='ldlidar_stl_ros_node',
            name='LD19',
            output='screen',
            parameters=[{
                'product_name': 'LDLiDAR_LD19',
                'topic_name': topic_name,
                'port_name': port_name,
                'frame_id': frame_id,
                # true = counterclockwise, false = clockwise
                'laser_scan_dir': True,
                # Mask data with angle_crop_min <= angle <= angle_crop_max (degrees)
                'enable_angle_crop_func': False,
                'angle_crop_min': 135.0,
                'angle_crop_max': 225.0,
            }],
        ),

        # static transform base_link -> base_laser (x y z, yaw pitch roll)
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='base_link_to_base_laser',
            arguments=['--x', '0', '--y', '0', '--z', '0.18',
                       '--yaw', '0', '--pitch', '0', '--roll', '0',
                       '--frame-id', 'base_link', '--child-frame-id', frame_id],
        ),
    ])
