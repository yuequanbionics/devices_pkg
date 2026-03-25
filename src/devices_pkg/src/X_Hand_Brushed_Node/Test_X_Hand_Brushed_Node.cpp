#include "Test_X_Hand_Brushed_Node.hpp"

int main(int argc, char const *argv[]){
    rclcpp::init(argc,argv);

    const auto X_Hand_Brushed_Node = std::make_shared<Test_X_Hand_Brushed_Node>();

    rclcpp::executors::MultiThreadedExecutor executor;

    executor.add_node(X_Hand_Brushed_Node);
    executor.spin();

    rclcpp::shutdown();
    return 0;
}
