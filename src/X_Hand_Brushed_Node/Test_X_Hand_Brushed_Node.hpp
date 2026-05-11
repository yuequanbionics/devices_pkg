#ifndef TEST_X_HAND_BRUSHED_NODE_H_
#define TEST_X_HAND_BRUSHED_NODE_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "UDP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Custom_TOP.hpp"
#include "Switch_Board.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include "devices_pkg/msg/x_hand_brushed_msg.hpp"
#include <chrono>

using namespace std;

class Test_X_Hand_Brushed_Node : public rclcpp::Node{
public:
    Test_X_Hand_Brushed_Node()
    : Node("test_x_hand_brushed_node"){
        publisher_Motor_cmd = this->create_publisher<devices_pkg::msg::XHandBrushedMsg>("x_hand_brushed_cmd", 10);
        subscription_Motor_Data = this->create_subscription<devices_pkg::msg::XHandBrushedMsg>("x_hand_brushed_data", 10, \
            std::bind(&Test_X_Hand_Brushed_Node::Motor_data_topic_callback, this, std::placeholders::_1));
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        timer_Motor_cmd = this->create_wall_timer(
        std::chrono::milliseconds(10),
        std::bind(&Test_X_Hand_Brushed_Node::Motor_timer_callback, this));
    }

    void Motor_data_topic_callback(const devices_pkg::msg::XHandBrushedMsg msg){
        auto get_message = msg;
    }

    void Motor_timer_callback(){
        auto send_message = devices_pkg::msg::XHandBrushedMsg();

        float pos = 5000.0f;

        send_message.motors[0].pos = pos;
        send_message.motors[1].pos = pos;
        send_message.motors[2].pos = pos;
        send_message.motors[3].pos = pos;
        send_message.motors[4].pos = pos;
        send_message.motors[5].pos = pos;
        publisher_Motor_cmd->publish(send_message);
    }

private:
    rclcpp::Publisher<devices_pkg::msg::XHandBrushedMsg>::SharedPtr publisher_Motor_cmd;
    rclcpp::Subscription<devices_pkg::msg::XHandBrushedMsg>::SharedPtr subscription_Motor_Data;
    rclcpp::TimerBase::SharedPtr timer_Motor_cmd;
};

#endif // TEST_X_HAND_BRUSHED_NODE_H_