#include "X_Hand_Brushed_Node.hpp"
#include <filesystem>
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <thread>
#include <vector>
#include <map>
#include <csignal>

using namespace std;

int main(int argc, char const *argv[]){
    rclcpp::init(argc,argv);
    
    const auto x_hand_brushed_node = std::make_shared<X_Hand_Brushed_Node>(argv[1]);

    rclcpp::executors::MultiThreadedExecutor executor;
    
    executor.add_node(x_hand_brushed_node);
    executor.spin();

    x_hand_brushed_node->reset_motors();
    rclcpp::shutdown();

    return 0;
}
