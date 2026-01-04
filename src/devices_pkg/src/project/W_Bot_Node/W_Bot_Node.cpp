#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "W_Bot_Node.hpp"
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

    //auto logger = rclcpp::get_logger("SensorDataLogger");

    const auto WBot_Node = std::make_shared<W_Bot_Node>();

   
    // 创建多线程执行器
    rclcpp::executors::MultiThreadedExecutor executor;

    executor.add_node(WBot_Node);
    executor.spin();

    // 关闭 ROS 2
    rclcpp::shutdown();
    //RCLCPP_INFO(logger, "ROS 2 node shutdown.");

    return 0;
}