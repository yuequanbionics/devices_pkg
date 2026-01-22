#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "UDP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Custom_TOP.hpp"
#include "Switch_Board.hpp"
#include <filesystem>
#include <iostream>
#include "devices_pkg/msg/x_hand_msg.hpp"

using namespace std;

class W_Bot_Node : public rclcpp::Node
{
public:
    W_Bot_Node()
    : Node("w_bot_node")
    {
        publisher_ = this->create_publisher<devices_pkg::msg::XHandMsg>("x_hand_subscriber", 10);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(10),
            std::bind(&W_Bot_Node::timer_callback, this));

        subscription_ = this->create_subscription<devices_pkg::msg::XHandMsg>("x_hand_publisher", 10, std::bind(&W_Bot_Node::topic_callback, this, std::placeholders::_1));
 }
private:

    void timer_callback()
    {
        auto message = devices_pkg::msg::XHandMsg();

        message.motors[1].kp = 500.0;
        message.motors[1].kd = 10.0;
        message.motors[1].pos = 0.5;
        publisher_->publish(message);
    }

     void topic_callback(const devices_pkg::msg::XHandMsg::SharedPtr msg) const
    {
        for(size_t i = 0; i < 6; i++)
        {
            RCLCPP_INFO(this->get_logger(), "Received motor %d command: pos=%f, vel=%f, tor=%f, kp=%f, kd=%f", 
                i+1, msg->motors[i].pos, msg->motors[i].vel, msg->motors[i].tor, msg->motors[i].kp, msg->motors[i].kd);
        }

    }
    rclcpp::Publisher<devices_pkg::msg::XHandMsg>::SharedPtr publisher_;
    rclcpp::Subscription<devices_pkg::msg::XHandMsg>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
};



