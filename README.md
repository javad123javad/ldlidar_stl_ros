- [en](#instructions)
- [cn](#操作指南)

# Instructions

ROS 2 driver for the LDROBOT LiDAR **LD19**. It publishes `sensor_msgs/msg/LaserScan` on `/scan` at about 10 Hz.

> Tested on ROS 2 Lyrical. It uses only standard `rclcpp` / `sensor_msgs` / `tf2_ros` APIs and is expected to work on ROS 2 Humble and newer.

## Step 0: get the package

```bash
mkdir -p ~/ldlidar_stl_ros_ws/src
cd ~/ldlidar_stl_ros_ws/src
git clone https://github.com/DFRobotdl/ldlidar_stl_ros.git
```

This gives a colcon workspace `~/ldlidar_stl_ros_ws` containing the package at `~/ldlidar_stl_ros_ws/src/ldlidar_stl_ros`. All commands below use these paths; adjust them if you cloned somewhere else.

## Step 1: system setup

1. Connect the LiDAR to your computer through an onboard serial port or a USB-to-serial adapter (for example a CP2102 module).
2. Find the device it appears as (usually `/dev/ttyUSB0`):

   ```bash
   ls -l /dev/ttyUSB*
   ```

3. Give your user access to the serial port. Either add yourself to the `dialout` group (log out and back in afterwards):

   ```bash
   sudo usermod -aG dialout $USER
   ```

   or install the udev rule, which also creates a stable `/dev/ldlidar` symlink for CP210x adapters:

   ```bash
   cd ~/ldlidar_stl_ros_ws/src/ldlidar_stl_ros/scripts && ./create_udev_rules.sh
   ```

## Step 2: build

```bash
cd ~/ldlidar_stl_ros_ws
source /opt/ros/$ROS_DISTRO/setup.bash
rosdep install --from-paths src --ignore-src -y
colcon build --cmake-args -DCMAKE_BUILD_TYPE=Release
```

> If `rosdep` reports that it has not been initialized, run `sudo rosdep init` and `rosdep update` once. The `rosdep install` step only installs missing dependencies and can be skipped if `rclcpp`, `sensor_msgs`, `tf2_ros`, `launch_ros` and `rviz2` are already installed.

## Step 3: run

### Step 3.1: source the workspace

Run this in every new terminal:

```bash
source ~/ldlidar_stl_ros_ws/install/setup.bash
```

To make it permanent:

```bash
echo "source ~/ldlidar_stl_ros_ws/install/setup.bash" >> ~/.bashrc
```

### Step 3.2: start the LiDAR node

- Driver plus the `base_link → base_laser` static transform:

  ```bash
  ros2 launch ldlidar_stl_ros ld19.launch.py
  ```

- The same, and show the scan in RViz2:

  ```bash
  ros2 launch ldlidar_stl_ros viewer_ld19.launch.py
  ```

- Use a different serial port, frame or topic:

  ```bash
  ros2 launch ldlidar_stl_ros ld19.launch.py port_name:=/dev/ldlidar frame_id:=laser topic_name:=scan
  ```

- Run the node directly without a launch file:

  ```bash
  ros2 run ldlidar_stl_ros ldlidar_stl_ros_node --ros-args -p port_name:=/dev/ttyUSB0
  ```

- Optional example subscriber that prints every point:

  ```bash
  ros2 run ldlidar_stl_ros ldlidar_stl_ros_listen_node --ros-args -p topic_name:=scan
  ```

### Parameters (`ldlidar_stl_ros_node`)

| Parameter                | Type   | Default         | Description                                                            |
| ------------------------ | ------ | --------------- | ---------------------------------------------------------------------- |
| `product_name`           | string | `LDLiDAR_LD19`  | Name used in log messages                                              |
| `port_name`              | string | `/dev/ttyUSB0`  | Serial device of the LiDAR                                             |
| `topic_name`             | string | `scan`          | Output `LaserScan` topic                                               |
| `frame_id`               | string | `base_laser`    | `header.frame_id` of the scan                                          |
| `laser_scan_dir`         | bool   | `true`          | `true` = counterclockwise, `false` = clockwise                         |
| `enable_angle_crop_func` | bool   | `false`         | Mask (set to NaN) points with `angle_crop_min <= angle <= angle_crop_max` |
| `angle_crop_min`         | double | `0.0`           | Start of the masked range, degrees (launch file sets `135.0`)          |
| `angle_crop_max`         | double | `0.0`           | End of the masked range, degrees (launch file sets `225.0`)            |

To change the mounting position of the LiDAR, edit the `static_transform_publisher` arguments in `launch/ld19.launch.py` (default: 0.18 m above `base_link`).

## Step 4: test

```bash
ros2 topic hz /scan --qos-reliability best_effort
ros2 topic echo /scan --once --qos-reliability best_effort
```

> `/scan` is published with sensor-data QoS (**best effort**). Subscribers that request *reliable* QoS will not receive it: in RViz2 set the LaserScan display's *Reliability Policy* to *Best Effort* (the bundled `rviz/ldlidar.rviz` already does this), and pass `--qos-reliability best_effort` to `ros2 topic` commands.

| Product            | Fixed Frame                 | Topic   |
| ------------------ | --------------------------- | ------- |
| LDROBOT LiDAR LD19 | `base_link` or `base_laser` | `/scan` |

If the node exits with `lidar pub data is time out`, no data arrived from the serial port for more than one second. Check the cable, the `port_name` and the port permissions.

# 操作指南

LDROBOT LiDAR **LD19** 的 ROS 2 驱动，在 `/scan` 话题上以约 10 Hz 发布 `sensor_msgs/msg/LaserScan`。

> 已在 ROS 2 Lyrical 上测试。代码只使用标准的 `rclcpp` / `sensor_msgs` / `tf2_ros` 接口，预计可用于 ROS 2 Humble 及更新版本。

## 0. 获取功能包

```bash
mkdir -p ~/ldlidar_stl_ros_ws/src
cd ~/ldlidar_stl_ros_ws/src
git clone https://github.com/DFRobotdl/ldlidar_stl_ros.git
```

这样会得到 colcon 工作空间 `~/ldlidar_stl_ros_ws`，功能包位于 `~/ldlidar_stl_ros_ws/src/ldlidar_stl_ros`。下文所有命令都使用这些路径；如果克隆到其他位置，请相应修改。

## 1. 系统设置

1. 通过板载串口或 USB 转串口模块（例如 CP2102）将雷达连接到电脑。
2. 查看雷达对应的串口设备（通常为 `/dev/ttyUSB0`）：

   ```bash
   ls -l /dev/ttyUSB*
   ```

3. 设置串口访问权限。将当前用户加入 `dialout` 组（之后需重新登录）：

   ```bash
   sudo usermod -aG dialout $USER
   ```

   或者安装 udev 规则（同时为 CP210x 模块创建固定的 `/dev/ldlidar` 链接）：

   ```bash
   cd ~/ldlidar_stl_ros_ws/src/ldlidar_stl_ros/scripts && ./create_udev_rules.sh
   ```

## 2. 编译方法

```bash
cd ~/ldlidar_stl_ros_ws
source /opt/ros/$ROS_DISTRO/setup.bash
rosdep install --from-paths src --ignore-src -y
colcon build --cmake-args -DCMAKE_BUILD_TYPE=Release
```

> 如果 `rosdep` 提示尚未初始化，请先执行一次 `sudo rosdep init` 和 `rosdep update`。`rosdep install` 只用于安装缺失的依赖；如果已安装 `rclcpp`、`sensor_msgs`、`tf2_ros`、`launch_ros` 和 `rviz2`，可以跳过这一步。

## 3. 运行方法

### 3.1. 设置环境变量

每个新终端都需要执行：

```bash
source ~/ldlidar_stl_ros_ws/install/setup.bash
```

如需永久生效：

```bash
echo "source ~/ldlidar_stl_ros_ws/install/setup.bash" >> ~/.bashrc
```

### 3.2. 启动激光雷达节点

- 启动驱动及 `base_link → base_laser` 静态坐标变换：

  ```bash
  ros2 launch ldlidar_stl_ros ld19.launch.py
  ```

- 启动驱动并在 RViz2 中显示：

  ```bash
  ros2 launch ldlidar_stl_ros viewer_ld19.launch.py
  ```

- 指定其他串口、坐标系或话题：

  ```bash
  ros2 launch ldlidar_stl_ros ld19.launch.py port_name:=/dev/ldlidar frame_id:=laser topic_name:=scan
  ```

- 不使用 launch 文件直接运行：

  ```bash
  ros2 run ldlidar_stl_ros ldlidar_stl_ros_node --ros-args -p port_name:=/dev/ttyUSB0
  ```

- 示例订阅节点（打印每个点）：

  ```bash
  ros2 run ldlidar_stl_ros ldlidar_stl_ros_listen_node --ros-args -p topic_name:=scan
  ```

节点参数见英文部分的 [Parameters](#parameters-ldlidar_stl_ros_node) 表格。雷达安装位置可在 `launch/ld19.launch.py` 中的 `static_transform_publisher` 参数里修改（默认在 `base_link` 上方 0.18 m）。

## 4. 测试

```bash
ros2 topic hz /scan --qos-reliability best_effort
ros2 topic echo /scan --once --qos-reliability best_effort
```

> `/scan` 使用传感器数据 QoS（**best effort**）。请求 *reliable* 的订阅者收不到数据：在 RViz2 中将 LaserScan 显示的 *Reliability Policy* 设为 *Best Effort*（自带的 `rviz/ldlidar.rviz` 已设置），使用 `ros2 topic` 命令时加上 `--qos-reliability best_effort`。

| 产品型号           | Fixed Frame                 | Topic   |
| ------------------ | --------------------------- | ------- |
| LDROBOT LiDAR LD19 | `base_link` 或 `base_laser` | `/scan` |

如果节点因 `lidar pub data is time out` 退出，说明超过 1 秒没有收到串口数据，请检查连线、`port_name` 和串口权限。
