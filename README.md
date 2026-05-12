# SDK_2_ROS

## 📋 项目介绍

> **本项目是 [SDK]项目 的 ROS 封装，提供 ROS 节点、话题、服务和参数接口，支持设备通信、数据解析、节点发布等功能。** 


📎项目Gitee仓库: https://gitee.com/beijing-daqi-yuequan-bionics/sdk2ros


📎项目GitHub仓库: https://github.com/yuequanbionics/YQ_SDK_ROS2.git

 
--- 

## 📌 环境要求

| 环境         | 版本要求 | 状态  |
| :----------- | :------- | :---- |
| 系统         | Ubuntu22.04、Ubuntu24.04 | ✅ 支持 |
| ROS版本      | humble、jazzy   | ✅ 支持 |
| Python       | 3.12     | ✅ 支持 |
| C++标准      | C++17    | ✅ 支持 |

---

## 🚀 克隆部署运行

> ⚠️ **注意事项：电脑IP要设置为192.168.3.245**

> ⚠️ 注意事项：编译前请确保已安装所有ROS2依赖

> ✅ 支持平台：包括x86/ARM架构的嵌入式设备



1. 代码拉取后 devices_pkg 为 ros2 package 便于集成到自己的工程中

2. cd devices_pkg

3. 如果想成为一个独立的工程 需要创建 workspace 。否则集成到自己工程后跳转到第 4. 步

   ./create_workspace.sh

    cd ..

    此时看到 devices_pkg_ws 文件夹就是工程文件 。


4. 运行拉取 SDK 的 shell 脚本(二选一)

    cd devices_pkg_ws/src/devices_pkg

    拉取gitee上的SDK源码仓库 (需要手动编译)

    ./pull_sdk.sh gitee

    拉取github上的SDK对外输出仓库

    ./pull_sdk.sh github

5. (如果是ARM平台）进sdk/目录下编译ARM平台下的第三方库

   ./Tool.sh clean
  
   ./Tool.sh make_third
   
   ./Tool.sh make
   
6. 回到 devices_pkg_ws 目录下编译命令

    colcon build  

7. 执行ROS的shell配置脚本

    source ./install/setup.sh  

8. 可执行文件路径

    /install/devices_pkg/lib/devices_pkg/  

9. 运行节点  || 通过launch启动多节点(二选一)

    ros2 run devices_pkg 可执行文件名

    ros2 launch devices_pkg xxx.py

    以Y_Hand为例：

    启动 Y_Hand 单节点  ros2 run devices_pkg Y_Hand_Node

    启动 Y_Hand + test 多节点 ros2 launch devices_pkg launch.py



