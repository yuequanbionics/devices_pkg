#include "Remote_Node.hpp"


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Remote_Node>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

