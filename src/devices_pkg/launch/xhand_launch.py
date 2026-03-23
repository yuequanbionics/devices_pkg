# launch/talker_launch.py
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import Shutdown

#根据SN后缀判断设备类型
def check_sn_suffix(config_str):
    sn_value = None
    # 1. 解析配置字符串，提取 SN 值
    for line in config_str.strip().split('\n'):
        line_clean = line.strip()
        if not line_clean or line_clean.startswith('#'):
            continue
        if line_clean.startswith('SN:'):
            sn_value = line_clean.split(':', 1)[1].strip()
            break
    
    # 2. 如果未提取到 SN 值，直接返回 self_1.0
    if sn_value is None:
        return 'self_1.0'
    
    # 3. 提取 SN 值最后非数字的两位字符（核心逻辑）
    # 倒序遍历，先过滤掉末尾的所有数字，再取最后两位非数字字符
    non_digit_chars = []
    # 倒序遍历每个字符，收集非数字字符
    for char in reversed(sn_value):
        if not char.isdigit():
            non_digit_chars.append(char)
        else:
            # 遇到数字就停止（只取末尾数字前的非数字字符）
            break
    # 反转回正序，得到 "末尾数字前的非数字字符序列"
    non_digit_suffix = ''.join(reversed(non_digit_chars))
    # 取最后两位（不足两位则取全部）
    check_suffix = non_digit_suffix[-2:] if len(non_digit_suffix) >= 2 else non_digit_suffix
    
    # 4. 根据后缀规则返回对应结果
    # 规则1：不是纯字母 → 返回 self_1.0
    if not check_suffix.isalpha():
        return 'self_1.0'
    # 规则2：匹配 SG → 返回 self_g
    elif check_suffix == 'SG':
        return 'self_g_1.0'
    # 规则3：匹配 MG → 返回 m2_g
    elif check_suffix == 'MG':
        return 'mz_g_1.0'
    # 规则4：匹配 M（仅1位） → 返回 m2
    elif check_suffix == 'M':
        return 'mz_1.0'
    # 其他纯字母情况（如 AB、S 等）→ 返回 self_1.0
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

