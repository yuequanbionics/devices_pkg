# launch/talker_launch.py
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import Shutdown

def generate_launch_description():

    curr_project = 'y_hand'  # y_hand  x_hand w_bot E_bot

    node_mapping = {
        'y_hand': ('y_hand_test_node', 'Y_Hand_Node', 'y_hand_node'),
        'x_hand': ('x_hand_test_node', 'X_Hand_Node', 'x_hand_node'),
        'w_bot': ('w_bot_test_node', 'W_Bot_Node', 'w_bot_node'),
        'E_bot': ('E_bot_test_node', 'E_Bot_Node', 'e_bot_node')
    }

    test_node_name, dev_executable, dev_node_name = node_mapping[curr_project]

    return LaunchDescription([
        
        Node(                            #测试节点
            package='devices_pkg',
            executable='Test_Node',      # CMakeLists.txt 中定义的可执行文件名
            name=test_node_name,            # 节点运行时的名称
            output='screen',             # 将节点的输出打印到屏幕
            arguments=[curr_project],  # 传递命令行参数给main函数
            on_exit=Shutdown(reason="launch is shutting down") #launch退出时关闭节点
        ),

        Node(                            #设备节点
            package='devices_pkg',
            executable=dev_executable, 
            name=dev_node_name,
            output='screen',
            on_exit=Shutdown(reason="launch is shutting down")
        )

    ])