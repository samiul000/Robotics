
# WSL ROS2 Setup

## 1. Lacale Configuration
``` bash
locale      # Check current locale settings
sudo apt update && sudo apt install locales
sudo locale-ge en_US en_US.UTF-8
sudo update-locale LC_ALL = en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8
locale      # Confirm locale settings update
```

## 2. Software Properties & Universe Repository
```bash
sudo apt install software-properties-common
sudo add-apt-repository universe
```
## 3. Curl Installation & ROS key
```bash
sudo apt update && sudo apt install curl -y
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/usr/share/keyrings/ros-archive-keyring.gpg
```
## 4. ROS2 Sources Configuration
```bash
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] https://packages.ros.org/ros2/ubuntu $(source /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
```
## 5. Update & Upgrade
```bash
sudo apt update && sudo apt upgrade
```
## 6. ROS Desktop Installation
```bash
sudo apt install ros-humble-desktop
```
## 7. ROS Enviroment Setup
```bash
source /opt/ros/humble/setup.bash
```
## 8. Run to check ROS2 Installation
```bash
ros2
```
## ***Sourcing setup.bash***
```bash
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
```
Or, Go to **.bashrc** file and add :
```bash
source /opt/ros/humble/setup.bash
```

🔗Next follow the **Executable.md** setup to test and run ROS2

