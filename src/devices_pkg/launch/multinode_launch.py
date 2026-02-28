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
    IP: 192.168.3.106
    # Port: 19001
"""
    # dev_config_2 = '1'
    # dev_config_3 = '1'
    

    node_mapping = {
        'y_hand_node': (config_dev1, 'Y_Hand_Node','sub_name1', 'pub_name1'),
        #'y_hand_node_2': (node1_Config, 'Y_Hand_Node','sub_name2', 'pub_name2'),
        #'y_hand_node_3': (node1_Config, 'Y_Hand_Node','sub_name3', 'pub_name3'),
    }

    def create_custom_node(node_name, exe_name, dev_config, sub_name, pub_name):
        return Node(
            package='devices_pkg',
            executable=exe_name,  # CMakeLists.txt 中定义的可执行文件名
            name=node_name,           # 节点运行时的名称
            output='screen',         # 将节点的输出打印到屏幕
            arguments=[node_name,pub_name,sub_name,dev_config],  # 传递命令行参数给main函数
            on_exit=Shutdown(reason="launch is shutting down")  # launch退出时关闭节点
        )

    node_list = []
    for node_name, (dev_config, exe_name, sub_name, pub_name) in node_mapping.items():
        node = create_custom_node(node_name, exe_name, dev_config, sub_name, pub_name)
        node_list.append(node)

    # 将所有节点传入LaunchDescription并返回
    return LaunchDescription(node_list)

