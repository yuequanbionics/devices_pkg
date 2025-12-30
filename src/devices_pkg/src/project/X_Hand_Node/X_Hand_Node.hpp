#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "UDP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Custom_TOP.hpp"
#include "Switch_Board.hpp"
#include <filesystem>
#include <iostream>
#include "devices_pkg/msg/x_hand_msg.hpp"
#include "Feetech_SCS0009.hpp"

using namespace std;


extern shared_ptr<Device_Struct> Device_Servo_SCS0009;
extern Servo_SCS0009 *Servo_Ctrl;

class X_Hand_Node : public rclcpp::Node
{
public:
     X_Hand_Node(Robot_Hardware *Robot_x_hand)
     : Node("x_hand_node")
     , Robot_ptr(Robot_x_hand)
     {
        subscription_ = this->create_subscription<devices_pkg::msg::XHandMsg>("x_hand_subscriber", 10, std::bind(&X_Hand_Node::topic_callback, this, std::placeholders::_1));
        publisher_ = this->create_publisher<devices_pkg::msg::XHandMsg>("x_hand_publisher", 10);
     }

private:
    void topic_callback(const devices_pkg::msg::XHandMsg::SharedPtr msg) const
    {
        //RCLCPP_INFO(this->get_logger(), "I heard[%d].", msg->cmd);
    }

    void publish_message()
    {
        auto message = devices_pkg::msg::XHandMsg();
        // message.frist_data = 4.56;
        // message.last_data = "X_Hand Node Message Data";
        publisher_->publish(message);
    }

    rclcpp::Subscription<devices_pkg::msg::XHandMsg>::SharedPtr subscription_;
    rclcpp::Publisher<devices_pkg::msg::XHandMsg>::SharedPtr publisher_;
    Robot_Hardware *Robot_ptr;
};