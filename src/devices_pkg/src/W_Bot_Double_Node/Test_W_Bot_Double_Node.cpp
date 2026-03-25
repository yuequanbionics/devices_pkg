#include "Test_W_Bot_Double_Node.hpp"


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    //std::string string_arg = argv[1];
    const auto WBot_Double_Node = std::make_shared<Test_W_Bot_Double_Node>();

    rclcpp::executors::MultiThreadedExecutor executor;

    executor.add_node(WBot_Double_Node);
    executor.spin();

    // 关闭 ROS 2
    rclcpp::shutdown();
}