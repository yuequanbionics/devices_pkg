# SDK_2_ROS

> 本项目是 [SDK]项目 的 ROS 封装，提供 ROS 节点、话题、服务和参数接口，便于在系统中集成设备的功能。 

 
--- 

## 环境要求

-当前开发环境  
-Ubuntu24.04  
-ROS2 jazzy  
-其他环境暂未测试  

---

## 克隆仓库

因为包含子模块需要添加 --recurse-submodules  
git clone --recurse-submodules git@xxxx.com:xxxxxxxxxxxxx.git  

拉取子模块SDK_2的最新分支  
git submodule update --remote --merge  

编译命令  
colcon build  

执行ROS的shell配置脚本  
source ./install/setup.sh  

可执行文件路径  
/install/devices_pkg/lib/devices_pkg  

执行可执行文件  
ros2 run devices_pkg 可执行文件名  
