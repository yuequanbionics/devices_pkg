# launch/talker_launch.py
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import Shutdown
from ament_index_python.packages import get_package_share_directory

#import os
import sys
from pathlib import Path
# Get_SN.py在launch目录
script_path = Path(__file__).parent
sys.path.insert(0, str(script_path))
sys.path.insert(0, str(script_path) + "/../Tool")

from Get_SN import DEC
from yhand_launch import check_sn_suffix


def generate_launch_description():
    pkg_path = get_package_share_directory("devices_pkg")
    #sdk_path = os.path.join(pkg_path, "/install/devices_pkg/share/devices_pkg")
    #print(sdk_path)
    print(pkg_path)
    SNs = DEC(pkg_path)
    print(SNs)

    node_mapping = {
        'test_y_hand_node_L': ("Test_Y_Hand_Node", 'y_hand_data_L','y_hand_cmd_L', 'None'),
        'test_y_hand_node_R': ("Test_Y_Hand_Node", 'y_hand_data_R','y_hand_cmd_R', 'None')
    }

    for sn in SNs:
        if(sn[:2] == "1Y"):
            if(int(sn[10]) % 2 != 0):
                print("y_hand_node_L SN:" + sn)
                config_dev_L = """
PC_IP: 192.168.3.245
SN: """ + sn + """
Boards:
  - Id: 101
    IP: 192.168.3.106
    # Port: 19001
"""
                node_mapping['y_hand_node_L'] = ('Y_Hand_Node','y_hand_cmd_L', 'y_hand_data_L',config_dev_L)
            else:
                print("y_hand_node_R SN:" + sn)
                config_dev_R = """
PC_IP: 192.168.3.245
SN: """ + sn + """
Boards:
  - Id: 101
    IP: 192.168.3.107
    # Port: 19001
"""
                node_mapping['y_hand_node_R'] = ('Y_Hand_Node','y_hand_cmd_R', 'y_hand_data_R',config_dev_R)

    def create_custom_node(node_name, exe_name, sub_name, pub_name,dev_type,dev_config):
        return Node(
            package='devices_pkg',
            executable=exe_name,  # CMakeLists.txt 中定义的可执行文件名
            name=node_name,           # 节点运行时的名称
            output='screen',         # 将节点的输出打印到屏幕
            arguments=[node_name,pub_name,sub_name,dev_type,dev_config],  # 传递命令行参数给main函数
            on_exit=Shutdown(reason="launch is shutting down")  # launch退出时关闭节点
        )

    node_list = []
    for node_name, ( exe_name, sub_name, pub_name, dev_config) in node_mapping.items():
        dev_type = check_sn_suffix(dev_config)
        node = create_custom_node(node_name, exe_name, sub_name, pub_name,dev_type,dev_config)
        node_list.append(node)

    # 将所有节点传入LaunchDescription并返回
    return LaunchDescription(node_list)

