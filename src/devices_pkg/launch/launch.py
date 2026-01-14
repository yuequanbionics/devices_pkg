# launch/talker_launch.py
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import Shutdown

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='devices_pkg',
            executable='Test_Node',      # CMakeLists.txt 中定义的可执行文件名
            name='test_node',            # 节点运行时的名称
            output='screen',             # 将节点的输出打印到屏幕
            on_exit=Shutdown(reason="launch is shutting down") #launch退出时关闭节点
        ),
        Node(
            package='devices_pkg',
            executable='X_Hand_Node', 
            name='x_hand_node',
            output='screen',
            on_exit=Shutdown(reason="launch is shutting down")
        )

    ])