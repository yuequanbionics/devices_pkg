# SDK_2_ROS

## 📋 项目介绍

> **本项目是 [SDK]项目 的 ROS 封装，提供 ROS 节点、话题、服务和参数接口，支持设备通信、数据解析、节点发布等功能。** 


📎项目Gitee仓库: https://gitee.com/beijing-daqi-yuequan-bionics/sdk2ros


📎项目GitHub仓库: https://github.com/yuequanbionics/YQ_SDK_ROS2.git

 
--- 

## 📌 环境要求

| 环境         | 版本要求 | 状态  |
| :----------- | :------- | :---- |
| 系统         | Ubuntu24.04 | ✅ 支持 |
| ROS版本      | jazzy   | ✅ 支持 |
| Python       | 3.12     | ✅ 支持 |
| C++标准      | C++17    | ✅ 支持 |

---

## 🚀 克隆部署运行

> ⚠️ **注意事项：电脑IP要设置为192.168.3.245**

> ⚠️ 注意事项：编译前请确保已安装所有ROS2依赖

> ✅ 支持平台：包括x86/ARM架构的嵌入式设备



1. SSH || HTTPS克隆包含子仓库(二选一)

    git clone git@xxxx.com:xxxxxxxxxxxxx.git

    git clone https://xxxxx.com/xxxxxxxx.git

2. cd 工程文件夹

3. 运行拉取SDK的shell脚本(二选一)

    拉取gitee上的SDK源码仓库

    ./pull_sdk.sh gitee

    拉取github上的SDK对外输出仓库

    ./pull_sdk.sh github

4. 编译命令

    colcon build  

5. 执行ROS的shell配置脚本

    source ./install/setup.sh  

6. 可执行文件路径

    /install/devices_pkg/lib/devices_pkg/  

7. 运行节点  || 通过launch启动多节点(二选一)

    ros2 run devices_pkg 可执行文件名

    ros2 launch devices_pkg xxx.py

    以Y_Hand为例：

    启动 Y_Hand 单节点  ros2 run devices_pkg Y_Hand_Node

    启动 Y_Hand + test 多节点 ros2 launch devices_pkg launch.py



