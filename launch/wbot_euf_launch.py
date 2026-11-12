# launch/talker_launch.py
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import Shutdown


def generate_launch_description():

    config_dev1 = """
PC_IP: 192.168.3.245
SN: asdf1234567
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
    # dev_config_2 = '1'
    # dev_config_3 = '1'
    

    node_mapping = {
        'w_bot_1': ('W_Bot_EUF_Node','None'),
        # 'test_wbot_euf_node': ('Test_W_Bot_EUF_Node',config_dev1)
    }

    # 创建自定义节点的函数 parameters: 节点名称、可执行文件名、通信配置
    def create_custom_node(node_name, exe_name,dev_config):
        return Node(
            package='devices_pkg',
            executable=exe_name,  # CMakeLists.txt 中定义的可执行文件名
            name=node_name,           # 节点运行时的名称
            output='screen',         # 将节点的输出打印到屏幕
            arguments=[node_name,dev_config],  # 传递命令行参数给main函数
            on_exit=Shutdown(reason="launch is shutting down")  # launch退出时关闭节点
        )

    node_list = []
    for node_name, (exe_name,dev_config) in node_mapping.items():
        node = create_custom_node(node_name, exe_name, dev_config)
        node_list.append(node)

    # 将所有节点传入LaunchDescription并返回
    return LaunchDescription(node_list)
