#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "UDP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Custom_TOP.hpp"
#include "Switch_Board.hpp"
#include <filesystem>
#include <iostream>
#include "devices_pkg/msg/e_bot_msg.hpp"
#include "Feetech_SCS0009.hpp"

using namespace std;


extern shared_ptr<Device_Struct> Device_Servo_SCS0009;
extern Servo_SCS0009 *Servo_Ctrl;

class E_Bot_Node : public rclcpp::Node
{
public:
     E_Bot_Node(Robot_Hardware *Robot_e_bot)
     : Node("e_bot_node")
     , Robot_ptr(Robot_e_bot)
     {
        subscription_ = this->create_subscription<devices_pkg::msg::EBotMsg>("e_bot_subscriber", 10, std::bind(&E_Bot_Node::topic_callback, this, std::placeholders::_1));
        publisher_ = this->create_publisher<devices_pkg::msg::EBotMsg>("e_bot_publisher", 10);
     }

private:
    void topic_callback(const devices_pkg::msg::EBotMsg::SharedPtr msg) const
    {
       RCLCPP_INFO(this->get_logger(), "I heard[%d].", msg->cmd);
    }

    void publish_message()
    {
        auto message = devices_pkg::msg::EBotMsg();
        // message.frist_data = 4.56;
        // message.last_data = "E_Bot Node Message Data";
        publisher_->publish(message);
    }

    rclcpp::Subscription<devices_pkg::msg::EBotMsg>::SharedPtr subscription_;
    rclcpp::Publisher<devices_pkg::msg::EBotMsg>::SharedPtr publisher_;
    Robot_Hardware *Robot_ptr;
};
