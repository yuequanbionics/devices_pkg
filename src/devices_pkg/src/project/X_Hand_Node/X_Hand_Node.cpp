#include "X_Hand_Node.hpp"
#include <filesystem>
#include <iostream>

using namespace std;

//extern Robot_Hardware *E_Hand;
//int hardware_init(string ADDR);

int main(int argc, char *argv[])
{
    UDP_Def UDP_Def_one;
    Init_One_UDP_Connect(&UDP_Def_one, inet_addr("127.0.0.1"), htons(16201));


    rclcpp::init(argc, argv);

    //hardware_init("src/devices_pkg/src/sdk/config/YAML/E_Bot/li_wenzhi/Servo_Config/TOP.yaml");

    //const auto ebot_node = std::make_shared<E_Bot_Node>(E_Hand);

    // 创建多线程执行器
    //rclcpp::executors::MultiThreadedExecutor executor;

    //executor.add_node(ebot_node);
    //executor.spin();

    rclcpp::shutdown();

    return 0;
}