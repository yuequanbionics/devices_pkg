#include "Y_Hand_Node.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "UDP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Custom_TOP.hpp"
#include "Switch_Board.hpp"

#include <filesystem>
#include <iostream>

using namespace std;


int main(int argc, char *argv[])

{
    rclcpp::init(argc, argv);

    const auto yhand_node = std::make_shared<Y_Hand_Node>();
    
    // 创建多线程执行器
    rclcpp::executors::MultiThreadedExecutor executor;

    executor.add_node(yhand_node);
    executor.spin();

    // 关闭 ROS 2
    rclcpp::shutdown();

    return 0;
}