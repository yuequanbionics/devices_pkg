from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import Shutdown

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='devices_pkg',
            executable='X_Hand_Brushed_Node',
            name='x_hand_brushed_node',
            output='screen',
            arguments=['None'],
            on_exit=Shutdown(reason="launch is shutting down")
        ),

        Node(
            package='devices_pkg',
            executable='Test_X_Hand_Brushed_Node',
            name='test_x_hand_double_node',
            output='screen',
            on_exit=Shutdown(reason="launch is shutting down")
        )

    ])