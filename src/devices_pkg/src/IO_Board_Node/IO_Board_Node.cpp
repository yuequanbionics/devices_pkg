//
// Created by lwz on 2026/3/12.
//
#include "IO_Board_Node.h"

using namespace std;

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    const auto IOboard_node = std::make_shared<IO_Board_Node>(argv[1], argv[2], argv[3], argv[4]);

    // 创建多线程执行器
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(IOboard_node);

    // 关闭 ROS 2
    executor.spin();

    return 0;
}

