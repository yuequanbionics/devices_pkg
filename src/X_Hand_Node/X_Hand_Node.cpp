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

     if (argc < 6)
    {
        cout << "Invalid or incomplete parameters" << endl;
        return -1;
    }
    const auto xhand_node = std::make_shared<X_Hand_Node>(argv[1]);

    if(xhand_node->create_objects(argv[2], argv[3], argv[4], argv[5]) != 0)
    {
        std::cerr << "Failed to create X_Hand_Node objects" << std::endl;
        rclcpp::shutdown();
        return -1;
    }

    // 创建多线程执行器
    rclcpp::executors::MultiThreadedExecutor executor;

    executor.add_node(xhand_node);
    executor.spin();

    // 关闭 ROS 2
    rclcpp::shutdown();

    return 0;
}
