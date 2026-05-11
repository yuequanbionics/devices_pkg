#ifndef X_HAND_BRUSHED_NODE_H_
#define X_HAND_BRUSHED_NODE_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "UDP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Custom_TOP.hpp"
#include "Switch_Board.hpp"
#include <filesystem>
#include <iostream>
#include <chrono>
#include "devices_pkg/msg/x_hand_brushed_msg.hpp"
#include "X_Hand_Protocol.hpp"

using namespace std;

extern Robot_Hardware* robot;
extern shared_ptr<Device_class> X_Hand_1_D;
extern shared_ptr<Device_class> X_Hand_2_D;
extern shared_ptr<Device_class> X_Hand_3_D;
extern shared_ptr<Device_class> X_Hand_4_D;
extern shared_ptr<Device_class> X_Hand_5_D;
extern shared_ptr<Device_class> X_Hand_6_D;
extern X_Hand_Protocol* X_Hand_1_Control;
extern X_Hand_Protocol* X_Hand_2_Control;
extern X_Hand_Protocol* X_Hand_3_Control;
extern X_Hand_Protocol* X_Hand_4_Control;
extern X_Hand_Protocol* X_Hand_5_Control;
extern X_Hand_Protocol* X_Hand_6_Control;

//声明外部函数
extern int hardware_init(string ADDR, string Config);

devices_pkg::msg::XHandBrushedMsg::SharedPtr get_message_motor = nullptr;

class X_Hand_Brushed_Node : public rclcpp::Node{
public:
    X_Hand_Brushed_Node(
        // const std::string& node_name,
        // const std::string& pub_topic,
        // const std::string& sub_topic,
        const std::string& dev_config
    )
    : Node("x_hand_brushed_node"){
        std::string path = "src/devices_pkg/sdk/config/YAML/X_Hand_Brushed/out/TOP.yaml";
        hardware_init(path, dev_config);
        publisher_Motor = this->create_publisher<devices_pkg::msg::XHandBrushedMsg>("x_hand_brushed_data", 10);
        subscription_Motor = this->create_subscription<devices_pkg::msg::XHandBrushedMsg>("x_hand_brushed_cmd", 10, \
            std::bind(&X_Hand_Brushed_Node::Motor_topic_callback, this, std::placeholders::_1));
        timer_Motor = this->create_wall_timer(
            std::chrono::milliseconds(10),
            std::bind(&X_Hand_Brushed_Node::motor_timer_callback, this));
    }

    void reset_motors() {
        const u16 reset = 0;

        X_Hand_1_Control->Set_Motor(reset, 0);
        X_Hand_2_Control->Set_Motor(reset, 0);
        X_Hand_3_Control->Set_Motor(reset, 0);
        X_Hand_4_Control->Set_Motor(reset, 0);
        X_Hand_5_Control->Set_Motor(reset, 0);
        X_Hand_6_Control->Set_Motor(reset, 0);

        robot->Send_Buff_Data();

        RCLCPP_INFO(this->get_logger(), "Reset X_Hand brushed motors before shutdown.");
    }
private:
    void Motor_topic_callback(const devices_pkg::msg::XHandBrushedMsg::SharedPtr msg) const{
        get_message_motor = msg;
    }

    void motor_timer_callback(){
        if(get_message_motor != nullptr){
            X_Hand_1_Control->Set_Motor(get_message_motor->motors[0].pos, 0);
            X_Hand_2_Control->Set_Motor(get_message_motor->motors[1].pos, 0);
            X_Hand_3_Control->Set_Motor(get_message_motor->motors[2].pos, 0);
            X_Hand_4_Control->Set_Motor(get_message_motor->motors[3].pos, 0);
            X_Hand_5_Control->Set_Motor(get_message_motor->motors[4].pos, 0);
            X_Hand_6_Control->Set_Motor(get_message_motor->motors[5].pos, 0);
            robot->Send_Buff_Data();

            publisher_Motor->publish(*get_message_motor);
            get_message_motor = nullptr;
        }
    }

    rclcpp::Subscription<devices_pkg::msg::XHandBrushedMsg>::SharedPtr subscription_Motor;
    rclcpp::Publisher<devices_pkg::msg::XHandBrushedMsg>::SharedPtr publisher_Motor;
    rclcpp::TimerBase::SharedPtr timer_Motor;
};

#endif //X_HAND_BRUSHED_NODE_H_ 