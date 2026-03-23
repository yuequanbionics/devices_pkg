# launch/talker_launch.py
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import Shutdown

def generate_launch_description():

    config_dev1 = """
PC_IP: 192.168.3.245
SN: YQ212232602001
Boards:
  - Id: 101
    IP: 192.168.3.101
    # Port: 
  - Id: 102
    IP: 192.168.3.102
    # Port: 19001
  - Id: 103
    IP: 192.168.3.103
    # Port: 19001
    
"""

    return LaunchDescription([
        
        Node(                            #测试节点
            package='devices_pkg',
            executable='W_Bot_Node',      # CMakeLists.txt 中定义的可执行文件名
            name='w_bot_node',            # 节点运行时的名称
            output='screen',             # 将节点的输出打印到屏幕
            arguments=[config_dev1],  # 传递命令行参数给main函数
            on_exit=Shutdown(reason="launch is shutting down") #launch退出时关闭节点
        ),

        # Node(                            #设备节点
        #     package='devices_pkg',
        #     executable='Test_W_Bot_Node', 
        #     name='test_wbot_node',
        #     output='screen',
        #     on_exit=Shutdown(reason="launch is shutting down")
        # )

    ])
