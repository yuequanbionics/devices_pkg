#ifndef TEST_WBOT_NODE_H_
#define TEST_WBOT_NODE_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "UDP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Custom_TOP.hpp"
#include "Switch_Board.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include "devices_pkg/msg/w_bot_motor.hpp"
#include "devices_pkg/msg/w_bot_imu.hpp"
#include "devices_pkg/msg/w_bot_led.hpp"

using namespace std;

class Test_W_Bot_Node : public rclcpp::Node
{
public:
    Test_W_Bot_Node()
    : Node("test_wbot_node")
    {
        publisher_Motor_cmd = this->create_publisher<devices_pkg::msg::WBotMotor>("wbot_motor_cmd", 10);
        publisher_LED_cmd = this->create_publisher<devices_pkg::msg::WBotLED>("wbot_led_cmd", 10);
        subscription_IMU_data = this->create_subscription<devices_pkg::msg::WBotIMU>("wbot_imu_data", 10, \
            std::bind(&Test_W_Bot_Node::imu_data_topic_callback, this,std::placeholders::_1));
        subscription_Motor_Data = this->create_subscription<devices_pkg::msg::WBotMotor>("wbot_motor_data", 10, \
            std::bind(&Test_W_Bot_Node::Motor_data_topic_callback, this, std::placeholders::_1));

        std::this_thread::sleep_for(std::chrono::seconds(2));
        timer_Motor_cmd = this->create_wall_timer(
        std::chrono::milliseconds(10),
        std::bind(&Test_W_Bot_Node::Motor_timer_callback, this));
    }

    void imu_data_topic_callback(const devices_pkg::msg::WBotIMU::SharedPtr msg)
    {
        auto get_message = *msg;
        RCLCPP_INFO(this->get_logger(), "Received IMU1 data: Chest Pitch: %f, Chest Yaw: %f, Chest Roll: %f",
                    get_message.imu_chest.pitch, get_message.imu_chest.yaw, get_message.imu_chest.roll);

        RCLCPP_INFO(this->get_logger(), "Received IMU2 data: Chassis Pitch: %f, Chassis Yaw: %f, Chassis Roll: %f",
                    get_message.imu_chassis.pitch, get_message.imu_chassis.yaw, get_message.imu_chassis.roll);
    }

    void Motor_data_topic_callback(const devices_pkg::msg::WBotMotor::SharedPtr msg)
    {
        auto get_message = *msg;
        RCLCPP_INFO(this->get_logger(), "Received Motor data: Lower_Limbs_Motor_Waist_Yaw Pos: %f", get_message.waist_yaw.pos);
        RCLCPP_INFO(this->get_logger(), "Received Motor data: left_shoulder_pitch Pos: %f", get_message.left_shoulder_pitch.pos);
        RCLCPP_INFO(this->get_logger(), "Received Motor data: wheel_left vel: %f", get_message.wheel_left.vel);
    }

    void Motor_timer_callback()
    {
        auto send_message = devices_pkg::msg::WBotMotor();

        float pos = 5.0f;

        send_message.waist_roll.pos = pos;
        send_message.waist_yaw.pos = pos;
        send_message.knee.pos = pos;
        send_message.hip.pos = pos;
        send_message.ankle.pos = pos;
        send_message.left_shoulder_pitch.pos = pos;
        send_message.left_shoulder_roll.pos = pos;
        send_message.left_shoulder_yaw.pos = pos;
        send_message.left_elbow.pos = pos;
        send_message.left_wrist_yaw.pos = pos;
        send_message.left_wrist_pitch.pos = pos;
        send_message.left_wrist_roll.pos = pos;
        send_message.right_shoulder_pitch.pos = pos;
        send_message.right_shoulder_roll.pos = pos;
        send_message.right_shoulder_yaw.pos = pos;
        send_message.right_elbow.pos = pos;
        send_message.right_wrist_yaw.pos = pos;
        send_message.right_wrist_pitch.pos = pos;
        send_message.right_wrist_roll.pos = pos;
        send_message.head_pitch.pos = pos;
        send_message.head_yaw.pos = pos;
        send_message.wheel_left.vel = 50.0f;
        send_message.wheel_right.vel = -50.0f;


        float kp = 1.0f;
        send_message.left_shoulder_pitch.kp = kp;
        send_message.left_shoulder_roll.kp = kp;
        send_message.left_shoulder_yaw.kp = kp;
        send_message.left_elbow.kp = kp;
        send_message.left_wrist_yaw.kp = kp;
        send_message.left_wrist_pitch.kp = kp;
        send_message.left_wrist_roll.kp = kp;
        send_message.right_shoulder_pitch.kp = kp;
        send_message.right_shoulder_roll.kp = kp;
        send_message.right_shoulder_yaw.kp = kp;
        send_message.right_elbow.kp = kp;
        send_message.right_wrist_yaw.kp = kp;
        send_message.right_wrist_pitch.kp = kp;
        send_message.right_wrist_roll.kp = kp;
        send_message.head_pitch.kp = kp;
        send_message.head_yaw.kp = kp;



        publisher_Motor_cmd->publish(send_message);
    }

private:
    rclcpp::Publisher<devices_pkg::msg::WBotMotor>::SharedPtr publisher_Motor_cmd;
    rclcpp::Publisher<devices_pkg::msg::WBotLED>::SharedPtr publisher_LED_cmd;

    rclcpp::Subscription<devices_pkg::msg::WBotIMU>::SharedPtr subscription_IMU_data;
    rclcpp::Subscription<devices_pkg::msg::WBotMotor>::SharedPtr subscription_Motor_Data;
    rclcpp::TimerBase::SharedPtr timer_Motor_cmd;

};


#endif // TEST_WBOT_NODE_H_