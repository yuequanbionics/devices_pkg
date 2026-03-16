# launch/talker_launch.py
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import Shutdown

#根据SN后缀判断设备类型
def check_sn_suffix(config_str):
    sn_value = None
    for line in config_str.strip().split('\n'):
        line_clean = line.strip()
        if not line_clean or line_clean.startswith('#'):
            continue
        if line_clean.startswith('SN:'):
            sn_value = line_clean.split(':', 1)[1].strip()
            break
    if sn_value is None:
        return 'self_1.0'
    if len(sn_value) > 0 and sn_value[-1] == 'M':
        return 'mz_1.0'
    else:
        return 'self_1.0'
    

def generate_launch_description():

    config_dev1 = """
PC_IP: 192.168.3.245
SN: asdf1234567
Boards:
  - Id: 101
    IP: 192.168.3.106
    # Port: 19001
"""
    

    node_mapping = {
        'x_hand_node': ('X_Hand_Node','x_hand_cmd', 'x_hand_data',config_dev1),
        'test_x_hand_node': ("Test_X_Hand_Node", 'x_hand_data','x_hand_cmd', 'None')
    }

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

