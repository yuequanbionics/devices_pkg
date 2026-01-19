#include "X_Hand_Node.hpp"
#include <filesystem>
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <thread>
#include <vector>
#include <map>
#include <csignal> 
using namespace std;

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    const auto xhand_node = std::make_shared<X_Hand_Node>();
    
    // 创建多线程执行器
    rclcpp::executors::MultiThreadedExecutor executor;

    executor.add_node(xhand_node);
    executor.spin();

    // 关闭 ROS 2
    rclcpp::shutdown();

    return 0;
}