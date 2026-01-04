#include "X_Hand_Node.hpp"
#include <filesystem>
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <thread>
#include <vector>
//#include "Hw_Pressure_Sensor.hpp"
#include <map>
#include <csignal> 
using namespace std;

std::atomic<bool> g_should_exit(false);
// 3. 定义信号处理函数
void signal_handler(int signum)
{
    // 当接收到 SIGINT (Ctrl+C) 信号时，设置退出标志
    if (signum == SIGINT) {
        g_should_exit = true;
    }
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    //auto logger = rclcpp::get_logger("SensorDataLogger");
    const auto xhand_node = std::make_shared<X_Hand_Node>();
    
    // 创建多线程执行器
    rclcpp::executors::MultiThreadedExecutor executor;

    executor.add_node(xhand_node);
    executor.spin();

    // 关闭 ROS 2
    rclcpp::shutdown();

    return 0;
}