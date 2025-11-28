# SDK_2_ROS

> 本项目是 [SDK_2]项目develop分支 的 ROS 封装，提供 ROS 节点、话题、服务和参数接口，便于在系统中集成设备的功能。  
 
--- 

## 环境要求

-当前开发环境  
-Ubuntu24.04  
-ROS2 jazzy  
-其他环境暂未测试  

---

## 克隆仓库

因为包含子模块需要添加 --recurse-submodules  
git clone --recurse-submodules git@gitee.com:beijing-daqi-yuequan-bionics/sdk2ros.git  

编译命令  
colcon build  

执行ROS的shell配置脚本  
source ./install/setup.sh  

可执行文件路径  
/install/devices_pkg/lib/devices_pkg  
