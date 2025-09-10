
# Project Title

A brief description of what this project does and who it's for

# [1]Topic
The topic is an essential communication method when working with ROS 2.
On this page, you will learn the following:

- message
- publisher, subscriber
- ros2 topic command
- ros2 interface command


# message
The information exchanged through topics is called a message.
Message types can be freely defined.
For example, by using ```msg/Bool``` included in the ```std_msgs``` package, you can perform topic communication with the ```bool``` type.
In ```minecraft_ros2```, custom types are defined in a package called ```minecraft_msgs```.

# publisher, subscriber
In ROS 2, the publisher is the node that sends out messages, and the subscriber is the node that receives them.
When a publisher publishes a message to a topic, all subscribers that are subscribed to that same topic will receive the message and can perform some action in response.

# ros2 topic Command

The ros2 topic command allows you to inspect and interact with topic communication.
Here, we will introduce some commonly used subcommands: list, echo, info, and pub.

## list
By running ```ros2 topic list```, you can check the list of topics that currently exist.
You can also use the``` -v``` option to display more detailed information.
For example, if you start ```minecraft_ros2``` and join a world, the output should look like the following:
```bash
ros2 topic list
/cmd_vel
/parameter_events
/player/image_raw
/player/imu
/player/nearby_living_entities
/player/pointcloud
/player/status
/rosout
/tf
```
## echo
```ros2 topic echo```
By running ```ros2 topic echo <topic_name>```, you can view the data being published on a given topic.
For example, if ```minecraft_ros2``` is running and you execute:
```bash
$ ros2 topic echo /player/status
```
You should be able to see the player s status, such as:
```bash
name: Dev
dimension: minecraft:overworld
food_level: 20
score: 0
...
```
## info
```ros2 topic info```
By running ```ros2 topic info <topic_name>```, you can see information about the publishers and subscribers for that topic.
```bash
$ ros2 topic info /player/status
Type: minecraft_msgs/msg/PlayerStatus
Publisher count: 1
Subscription count: 0
```
## pub
```ros2 topic pub```
By running ```ros2 topic pub <topic_name> <type> <message>```, you can publish a message to a topic.
For example, to send a command to ```/cmd_vel``` that moves forward at 1 m/s and left at 0.3 m/s, run:
```bash
$ ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 1.0, y: 0.3}}"
```
The type geometry_msgs/msg/Twist represents translation and rotation in 3D space. Here:
```
linear.x ? forward translation
linear.y ? left translation
```
- *Note: After running the above command, the robot may keep moving. To stop it, press Ctrl + C, then send an empty message to stop movement.*

If you omit the message, an empty message will be sent automatically:

```bash
$ ros2 topic pub /cmd_vel geometry_msgs/msg/Twist
```

# ros2 interface Command
The ```ros2 interface``` command lets you inspect message and service definitions. The most common subcommands are package and show.

## package
```ros2 interface package```
By running ```ros2 interface package <package_name>```, you can list all messages and services defined in that package.
```bash
$ ros2 interface package minecraft_msgs
minecraft_msgs/srv/Command
minecraft_msgs/msg/BlockArray
minecraft_msgs/msg/LivingEntity
minecraft_msgs/msg/LivingEntityArray
minecraft_msgs/msg/PlayerStatus
minecraft_msgs/msg/Item
minecraft_msgs/msg/Block
minecraft_msgs/msg/MobCategory
```
## show
```ros2 interface show```
By running ros2 interface show <message_type>, you can inspect the structure of a specific message type.
```bash
$ ros2 interface show minecraft_msgs/msg/Item
string name
uint8 count
int16 damage
int16 max_damage
```

-------------------------------------------------------------------------------------------

# [2]RViz2
RViz is a 3D visualizer included with ROS (Robot Operating System).
Using RViz, you can visualize player information, LiDAR scans, camera images, and other sensor data in a 3D space.
## Launch RViz2
Simply run:
```bash
rviz2
```
Note: This assumes you have already sourced your ROS 2 workspace.

Default Screen

![Alt](https://minecraft-ros2.github.io/minecraft_ros2/assets/rviz2_default.BM7JBXRR.png)

When you first launch RViz2, a default empty screen will appear.


## Visualization 
In this tutorial, we will visualize Image (Minecraft screen) and LiDAR data. Make sure minecraft_ros2 is running.

### Image 
The Image topic sends the Minecraft gameplay screen.

In the bottom left of RViz, *click Add → By topic → Image*.

Select the relevant topic.

![Alt](https://minecraft-ros2.github.io/minecraft_ros2/assets/rviz2_add_image.wrw669cC.png)
You should now see the Minecraft screen displayed in the bottom left of RViz!

## LiDAR
Equipping the player with a LiDAR sensor allows you to detect distances to surrounding blocks and entities.

As with Image, **click Add → By topic → PointCloud2**.

At first, nothing may appear.

*Reason: The displayed coordinate frame may be incorrect*.

Go to **Global Options** → **Fixed Frame** and change it to ```player```.

![Alt](https://minecraft-ros2.github.io/minecraft_ros2/assets/rviz2_fixed_frame.CfjIZBOS.png)

Now the LiDAR point cloud should be visible.

Tip: If the point cloud is hard to see, adjust PointCloud2 → Size (m) to improve visibility.

This setup allows you to visualize both the Minecraft screen and surrounding environment in 3D, giving a better understanding of the player’s perspective and LiDAR readings.


-------------------------------------------------------------------------------------------

# [3]Control Player
In this tutorial, we will use Python to remotely control a Minecraft player.
You will learn:
-	How to create a package and node
-	How to create a Python publisher

## 1. Create a Package
Navigate to your desired workspace:
```bash
mkdir -p ~/minecraft_ros2_tutorial_ws/src
cd ~/minecraft_ros2_tutorial_ws/src
```
Create a ROS 2 Python package template:
```bash
ros2 pkg create --build-type ament_python minecraft_ros2_tutorial
```
Next, modify the template as needed.

### Edit *package.xml*

Add the dependency on ```geometry_msgs```. Insert the following between ```<test_depend>``` and ```<export>```:

```xml
<depend>geometry_msgs</depend>
```
Also edit the *description*, *maintainer*, and *license* tags as needed.

### Edit setup.py
Register the node by modifying the console_scripts section:
```python
entry_points={
        'console_scripts': [
                'control_player = minecraft_ros2_tutorial.control_player:main',
        ],
},
```

## 2. Write the Python Code

Create ```minecraft_ros2_tutorial/control_player.py``` with the following code:

```python
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist

class ControlPlayer(Node):
    def __init__(self):
        super().__init__('control_player')
        self.publisher_ = self.create_publisher(Twist, '/cmd_vel', 10)
        timer_period = 0.01
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.speed = 1.0
        print("Control Player Node Initialized. Use wasd keys to move, Space to jump, WASD for rotation.")

    def timer_callback(self):
        key = input("enter key (wasd,space,WASD) >>>")
        twist = Twist()

        # Linear movement
        if 'w' in key:
            twist.linear.x += self.speed
        if 's' in key:
            twist.linear.x -= self.speed
        if 'a' in key:
            twist.linear.y += self.speed
        if 'd' in key:
            twist.linear.y -= self.speed
        if ' ' in key:
            twist.linear.z = 1.0

        # Angular movement (view rotation)
        if 'W' in key:
            twist.angular.y += self.speed
        if 'S' in key:
            twist.angular.y -= self.speed
        if 'A' in key:
            twist.angular.z += self.speed
        if 'D' in key:
            twist.angular.z -= self.speed

        self.publisher_.publish(twist)

def main(args=None):
    rclpy.init(args=args)
    control_player = ControlPlayer()
    rclpy.spin(control_player)
    control_player.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

## How It Works
-	wasd → move the player horizontally
-	Space → jump
-	WASD → rotate the player’s view

The program publishes a Twist message to /cmd_vel based on your keyboard input.


## Build and Run

### 1. Navigate to your workspace:
```bash
cd ~/minecraft_ros2_tutorial_ws
```
### 2. Build the package:
```bash
colcon build
```
### 3. Source the workspace (first time only):
```bash
source install/setup.bash
```
### 4. Run the node:
```bash
ros2 run minecraft_ros2_tutorial control_player
```
Now you can control the Minecraft player remotely from **ROS 2**!


-------------------------------------------------------------------------------------------

# [4]Break Block
In this tutorial, we will use Python to control the player and break surrounding blocks in Minecraft.
You will learn about:
-	Quaternions
-	Service communication
-	Position control using a proportional (P) controller

## 1. Trying Out Service Communication

**Service communication** is a synchronous or asynchronous communication method where a server and client exchange a request and response.
In **minecraft_ros2**, services are used for actions such as:
-	breaking blocks,
-	executing game commands,
-	summoning entities, etc.

First, run the client and enter the Minecraft world.
Check the available services with:

```bash
ros2 service list
```
For this tutorial, we’ll use the */dig_block* service.
Point the player’s view toward a block and run:

```bash
ros2 service call /dig_block minecraft_msgs/srv/DigBlock
```
If the block is destroyed, the service call worked! 

ℹ️ Info: If it doesn’t work, re-source the minecraft_msgs package.


## 2. Getting the Player’s Orientation
Now we can break blocks, but we also need to adjust the player’s viewpoint to aim at them automatically.

For debugging, we’ll use the */player/ground_truth* topic, which publishes the player’s pose.

Check it with:

```bash
ros2 topic echo /player/ground_truth
```
Example output:
```yaml
position:
  x: 14.9883
  y: -589.1905
  z: 74.0
orientation:
  x: 0.0256
  y: 0.0314
  z: -0.6318
  w: 0.7740
```
This gives both the position (x, y, z) and orientation (x, y, z, w).
You might be wondering: what is w?

This is a quaternion, which represents rotations in a way that avoids issues like gimbal lock.

Quaternions can also be converted into roll, pitch, and yaw angles if needed.

## 3. Adding New Code

Create a new file:

```bash
~/minecraft_ros2_tutorial_ws/src/minecraft_ros2_tutorial/minecraft_ros2_tutorial/break_block.py
```
Paste the following code:

```python
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist, Pose
import math

class BreakBlock(Node):
    def __init__(self):
        super().__init__('break_block')
        self.publisher_ = self.create_publisher(Twist, '/cmd_vel', 10)
        self.subscription = self.create_subscription(
            Pose, '/player/ground_truth', self.pose_callback, 10
        )  # Calls pose_callback on each /player/ground_truth message

        self.player_pose = None
        self.Kp = 2.0
        self.timer = self.create_timer(0.01, self.timer_callback)
        self.get_logger().info("Break Block Node Initialized.")

    def pose_callback(self, msg: Pose):
        self.player_pose = msg

    def quaternion_to_yaw_pitch(self, q):
        x, y, z, w = q.x, q.y, q.z, q.w
        siny_cosp = 2.0 * (w * z + x * y)
        cosy_cosp = 1.0 - 2.0 * (y * y + z * z)
        yaw = -math.atan2(siny_cosp, cosy_cosp)

        sinp = 2.0 * (w * y - z * x)
        pitch = math.copysign(math.pi / 2, sinp) if abs(sinp) >= 1 else math.asin(sinp)
        return yaw, pitch

    def timer_callback(self):
        if self.player_pose is None:
            return

        yaw, pitch = self.quaternion_to_yaw_pitch(self.player_pose.orientation)

        self.target_block_position = [1, 0, -1]  # front, left, down
        dx, dy, dz = self.target_block_position
        dz = dz - 1  # player’s head is offset
        target_yaw = math.atan2(-dy, dx)
        target_pitch = math.atan2(-dz, math.sqrt(dx**2 + (-dy)**2))

        # Error calculation
        yaw_error = target_yaw - yaw
        pitch_error = target_pitch - pitch

        twist = Twist()
        twist.angular.y = -self.Kp * pitch_error
        twist.angular.z = -self.Kp * yaw_error
        self.publisher_.publish(twist)

def main(args=None):
    rclpy.init(args=args)
    node = BreakBlock()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```
Then ```edit setup.py``` to add a console entry:

```python
'break_block = minecraft_ros2_tutorial.break_block:main',
```
Build and source your workspace:

```bash
colcon build
source install/setup.bash
```
Run it:

```bash
ros2 run minecraft_ros2_tutorial break_block
```
The viewpoint should automatically move toward the target block position.

This works by combining coordinate transformations with P-control.

Here, ```target_block_position``` is the block’s relative position to the player. (In this case: front + diagonal down.)

### What is P-control?
Proportional control calculates the command as a constant multiplier of the error:
-	Larger errors → stronger commands
-	Smaller errors → weaker commands
Try changing ```target_block_position``` to test different aiming directions.

## 4. Breaking a Specific Block
Next, let’s extend the code to automatically break blocks and move forward to create a staircase.

Because block breaking currently pauses the game, we’ll use a flag to handle this properly.

```python
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist, Pose
from minecraft_msgs.srv import DigBlock
import math

class BreakBlock(Node):
    def __init__(self):
        super().__init__('break_block')
        self.publisher_ = self.create_publisher(Twist, '/cmd_vel', 10)
        self.subscription = self.create_subscription(
            Pose, '/player/ground_truth', self.pose_callback, 10
        )

        # DigBlock service client
        self.dig_client = self.create_client(DigBlock, '/dig_block')
        while not self.dig_client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('Waiting for /dig_block service...')

        self.player_pose = None
        self.in_progress = False
        self.Kp = 2.0
        self.timer = self.create_timer(0.1, self.timer_callback)
        self.get_logger().info("Break Block Node Initialized.")

    def pose_callback(self, msg: Pose):
        self.player_pose = msg

    def quaternion_to_yaw_pitch(self, q):
        x, y, z, w = q.x, q.y, q.z, q.w
        siny_cosp = 2.0 * (w * z + x * y)
        cosy_cosp = 1.0 - 2.0 * (y * y + z * z)
        yaw = -math.atan2(siny_cosp, cosy_cosp)
        sinp = 2.0 * (w * y - z * x)
        pitch = math.copysign(math.pi / 2, sinp) if abs(sinp) >= 1 else math.asin(sinp)
        return yaw, pitch

    def timer_callback(self):
        if self.player_pose is None:
            return

        yaw, pitch = self.quaternion_to_yaw_pitch(self.player_pose.orientation)

        self.target_block_position = [1, 0, -1]  # front, left, down
        dx, dy, dz = self.target_block_position
        target_yaw = math.atan2(-dy, dx)
        target_pitch = math.atan2(-dz, math.sqrt(dx**2 + (-dy)**2))

        # Error calculation
        yaw_error = target_yaw - yaw
        pitch_error = target_pitch - pitch

        twist = Twist()
        twist.angular.y = -self.Kp * pitch_error
        twist.angular.z = -self.Kp * yaw_error
        self.publisher_.publish(twist)

        if abs(yaw_error) < 0.05 and abs(pitch_error) < 0.05:
            if not self.in_progress:
                self.in_progress = True
                self.get_logger().info('Target reached, sending dig command...')
                twist = Twist()
                self.publisher_.publish(twist)
                req = DigBlock.Request()
                future = self.dig_client.call_async(req)
                future.add_done_callback(self.dig_response_callback)

    def dig_response_callback(self, future):
        try:
            response = future.result()
            self.get_logger().info(f'DigBlock response received: {response}')
        except Exception as e:
            self.get_logger().error(f'Service call failed: {e}')
        self.in_progress = False
        
        # Move forward after breaking
        twist = Twist()
        twist.linear.x = 1.0
        self.publisher_.publish(twist)

def main(args=None):
    rclpy.init(args=args)
    node = BreakBlock()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```
Add a dependency in ```package.xml```:

```xml
<depend>minecraft_msgs</depend>
```
## Result
When you run this program, the player will automatically dig blocks diagonally and move forward, effectively creating a staircase.


-------------------------------------------------------------------------------------------

# [4]Navigation
In this tutorial, you will use **Navigation2 (Nav2)** to perform **2D autonomous navigation**.
You will learn:
-	How Navigation2 works in detail
-	How to use Navigation2 in practice

## What is Navigation2?
**Navigation2 (Nav2)** is a collection of ROS 2 packages that provide autonomous navigation capabilities for mobile robots.
It integrates localization, path planning, obstacle avoidance, and path following, enabling a robot to move automatically to a specified goal.

### Common use cases:
-	Indoor mobile robots (delivery robots, cleaning robots, etc.)
-	Research robots with autonomous movement
-	Robotics competitions or experimental projects
-	Automated control of a Minecraft player

### How Navigation2 Works
Nav2 is structured into several main layers:
-	**Costmap**
Represents the environment as a costmap using a map, LiDAR, or other sensors.
This allows the system to know the distance to obstacles and generate safe paths.
-	**Planner**
Generates a path based on the costmap and robot geometry.
This determines the route from the start position to the goal.
-	**Controller**
Follows the path generated by the planner.
It calculates and outputs velocity commands using the path and localization data to drive the robot.
-	**Behavior Tree**
Controls the overall navigation flow to reach the goal.
Tasks are written in an XML tree structure, enabling complex behaviors beyond simple driving.

## Requirements
-	ROS 2 Humble (Ubuntu 22.04 assumed)
-	Build and fetch tools: *git, vcstool, colcon, rosdep*
-	Repository: *minecraft_ros2_example*
-	Package group: *minecraft_ros2 series* (fetched via .repos)
-	Minecraft client (launched with *runClient.sh*)

### Setup
**1. Install development tools (if not already installed)**

```bash
sudo apt update
sudo apt install -y git python3-vcstool python3-colcon-common-extensions python3-rosdep
# Initialize rosdep (first time only)
sudo rosdep init || true
rosdep update
```
**2. Source the ROS 2 environment**

```bash
source /opt/ros/humble/setup.bash
```
This enables ```ROS_DISTRO=humble```.

**2. Create a workspace & fetch the sample code**
Follow the Sample Code Setup instructions to prepare the example workspace.

## Running the Tutorial
**Terminal 1: Launch Nav2**

```bash
source ~/ros2_java_ws/install/setup.bash
source ~/minecraft_ros2_example_ws/install/setup.bash
ros2 launch minecraft_ros2_example nav2_bringup.launch.xml
```
This will start the Nav2-related nodes.
The TF chain from *world → player* is used to enable autonomous navigation.

**Terminal 2: Launch the Minecraft client**

```bash
cd ~/minecraft_ros2
./runClient.sh
```
Once Minecraft starts, create a superflat world, build some walls, and equip the LiDAR.
If the costmap is being generated correctly, you’re ready to go!

## Running Autonomous Navigation
In RViz2, use the 2D Goal Pose tool to send a target location.
The robot (or Minecraft player) will automatically navigate to the goal using Nav2.


-------------------------------------------------------------------------------------------

# [5]Localization and Mapping
This tutorial lets you experience LiDAR-SLAM in a Minecraft world using ROS 2 Humble.
The goal is to understand how a robot estimates its own position (localization), starting from the basics and progressing step by step through actual commands.

## What is Localization?
Localization is the technique by which a robot estimates where it is right now.
It uses sensor information such as:
wheel odometry,
-	IMU,
-	LiDAR,
-	cameras.
From these, the robot calculates its pose:
-	position (x, y, z)
-	orientation (yaw, pitch, roll).
If a map already exists, the robot matches its sensor observations to the map in order to estimate its current position.
Everyday analogy: Think of a smartphone map app. It uses GPS and sensors to move your location icon around the map.

## What is SLAM (Simultaneous Localization and Mapping)?
SLAM is the process of creating a map and estimating the robot’s position within it at the same time, especially when no map exists.
With LiDAR-SLAM, the LiDAR scans the environment (walls, obstacles, etc.) to:
-	build an occupancy grid map (```/map```),
-	and continuously estimate the robot’s position (```/tf: map → base_link```).
-	Localization: assumes a map exists → only estimates position.
-	SLAM: no map (or a changing one) → builds the map and estimates position simultaneously.
In this tutorial, you will experience LiDAR-based SLAM inside a Minecraft world.

## Tutorial Overview
-	Set up ROS 2 Humble and development tools
-	Download the sample packages and build the workspace
-	Launch SLAM in Terminal 1
-	Launch the Minecraft client (ROS-integrated) in Terminal 2
-	Watch as the robot moves around the virtual world, updating /map (map) and localization in real time

## Requirements
-	ROS 2 Humble (Ubuntu 22.04 assumed)
-	Build and fetch tools: git, vcstool, colcon, rosdep
-	Repository: minecraft_ros2_example
-	Package group: minecraft_ros2 series (fetched via .repos)
-	Minecraft client (launched via runClient.sh)

## Setup

### 1. Install development tools (if not already installed)
```bash
sudo apt update
sudo apt install -y git python3-vcstool python3-colcon-common-extensions python3-rosdep
# Initialize rosdep (only once)
sudo rosdep init || true
rosdep update
```
### 2. Source ROS 2 environment
```bash
source /opt/ros/humble/setup.bash
```
This sets``` ROS_DISTRO=humble```.

### 3. Create workspace & fetch samples
Follow the instructions in Sample Code Setup to prepare the sample workspace.


### **Running the Tutorial**

### Terminal 1: Launch SLAM
```bash
source ~/ros2_java_ws/install/setup.bash
source ~/minecraft_ros2_example_ws/install/setup.bash
ros2 launch minecraft_ros2_example lidarslam_ros2.launch.xml
```
This starts the LiDAR-SLAM node and bridge nodes.
You will see topics such as:
-	```/scan``` (LiDAR scan)
-	```/map``` (occupancy grid map)
-	```/tf``` (coordinate transforms)

### Terminal 2: Launch Minecraft client
```bash
cd ~/minecraft_ros2
./runClient.sh
```
This launches the Minecraft client in ROS-integrated mode.

## How to Check Operation
• **List topics:**
```bash
source ~/minecraft_ros2/install/setup.bash
ros2 topic list
```
• **Check if the map is updating:**
```bash
ros2 topic echo /map --qos-durability=transient_local
```
• **Check if localization (tf) is being published:**
```bash
ros2 run tf2_tools view_frames
# Inspect frames.pdf (should show map → odom → base_link, etc.)
```
• **Optional: Visualization in RViz2**
```bash
rviz2
```
Display ```/map``` (Map), ```/scan``` (LaserScan/PointCloud), and TF for easier understanding.

## Common Issues & Fixes
- command not found: ros2/colcon
→ ROS 2 not installed or forgot to source /opt/ros/humble/setup.bash.
- rosdep errors
→ Run sudo rosdep init and rosdep update. Check proxy settings if behind a firewall.
- runClient.sh missing / not executable
→ Check file location and run chmod +x runClient.sh.
- Build failure (missing dependencies)
→ Re-run rosdep install .... If apt errors, try sudo apt -f install.
- No topics published
→ Ensure Terminal 1 (SLAM) is running properly and Terminal 2 (client) is connected.

## Summary
-	Localization = estimating your position on an existing map.
-	SLAM = creating a map and localizing at the same time in an unknown or changing environment.
In this tutorial, you used LiDAR-SLAM in a Minecraft virtual world to update both /map and /tf in real time.
The provided setup should work out of the box. If needed, use RViz to visualize the robot’s motion and mapping process.

-------------------------------------------------------------------------------------------

***Note : This document is purely a tutorial and reference for Minecraft ROS 2. All rights are retained by the original developer. More details in the LICENSE.md file***


