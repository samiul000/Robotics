
# Executable Setup

## 1. Turtlesim Installation
```bash
sudo apt update
sudo apt install ros_humble_turtlesim
```
## 2. See list of packages
```bash
ros2 pkg list
```
## 3. See list of <packages, executables>
```bash
ros2 pkg executables
```
## 4. See list of executables from Turtlesim package
```bash
ros2 pkg executables turtlesim
```
## 5. Where is turtlesim?
```bash
cd /opt/ros/humble/
```
Or, Type on the terminal
```bash
code.
```
It will take you to the VS Code. Then Press
```
Ctrl+P
```
And search 
```
turtlesim
```
It should show the location in *share/ament_index/resource_index/packages*

## 6. Run turtlesim
```bash
ros2 run turtlesim turtlesim_node
```
In other terminal, run *teleop mode*
```bash
ros2 run turtlesim turtle_teleop_key
```