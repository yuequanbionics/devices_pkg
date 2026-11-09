# launch/talker_launch.py
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import Shutdown

def check_sn_suffix(config_str):
    return "defalut"


def generate_launch_description():

    config_dev_1 = """
PC_IP: 192.168.3.245
SN: 1Y2S2608003M
Boards:
  - Id: 101
    IP: 192.168.3.101
    # Port: 19001
"""
    #最大电流限制参数(28个)
    max_cur_limit_1 = [0.5,0.5,0.5,0.5,0.5,0.5,0.5,
                       0.5,0.5,0.5,0.5,0.5,0.5,0.5,
                       0.5,0.5,0.5,0.5,0.5,0.5,0.5,
                       0.5,0.5,0.5,0.5,0.5,0.5,0.5]
    # dev_config_2 = '1'
    # dev_config_3 = '1'
    

    node_mapping = {
        'y_hand_node': ('Y_Hand_Node','y_hand_cmd', 'y_hand_data', config_dev_1, max_cur_limit_1 ),
        'test_y_hand_node': ("Test_Y_Hand_Node", 'y_hand_data','y_hand_cmd', 'None',max_cur_limit_1)
    }

    def create_custom_node(node_name, exe_name, sub_name, pub_name,dev_config,limit_array):
        return Node(
            package='devices_pkg',
            executable=exe_name,  # CMakeLists.txt 中定义的可执行文件名
            name=node_name,           # 节点运行时的名称
            output='screen',         # 将节点的输出打印到屏幕
            arguments=[node_name,pub_name,sub_name,dev_config],  # 传递命令行参数给main函数
            parameters=[
                {"max_current_limit": limit_array}
            ],
            on_exit=Shutdown(reason="launch is shutting down")  # launch退出时关闭节点
        )

    node_list = []
    for node_name, ( exe_name, sub_name, pub_name, dev_config, limit_array) in node_mapping.items():
        node = create_custom_node(node_name, exe_name, sub_name, pub_name,dev_config,limit_array)
        node_list.append(node)

    # 将所有节点传入LaunchDescription并返回
    return LaunchDescription(node_list)

