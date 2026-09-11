#ifndef W_BOT_DOUBLE_NODE_H_
#define W_BOT_DOUBLE_NODE_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "UDP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Custom_TOP.hpp"
#include "Switch_Board.hpp"
#include <filesystem>
#include <iostream>
#include "devices_pkg/msg/w_bot_double_motor.hpp"
#include "Motor_TaiHu.hpp"

using namespace std;

//声明外部变量
extern Robot_Hardware *Test_Robot;

extern shared_ptr<Device_class> TaiHu_Device_1;
extern shared_ptr<Device_class> TaiHu_Device_2;
extern shared_ptr<Device_class> TaiHu_Device_3;
extern shared_ptr<Device_class> TaiHu_Device_4;
extern shared_ptr<Device_class> TaiHu_Device_5;
extern shared_ptr<Device_class> TaiHu_Device_6;
extern shared_ptr<Device_class> TaiHu_Device_7;
extern shared_ptr<Device_class> TaiHu_Device_8;
extern shared_ptr<Device_class> TaiHu_Device_9;
extern shared_ptr<Device_class> TaiHu_Device_10;
extern shared_ptr<Device_class> TaiHu_Device_11;
extern shared_ptr<Device_class> TaiHu_Device_12;
extern shared_ptr<Device_class> TaiHu_Device_13;
extern shared_ptr<Device_class> TaiHu_Device_14;
extern shared_ptr<Device_class> TaiHu_Device_15;
extern shared_ptr<Device_class> TaiHu_Device_16;

extern Motor_TaiHu *TaiHu_Device_T1;
extern Motor_TaiHu *TaiHu_Device_T2;
extern Motor_TaiHu *TaiHu_Device_T3;
extern Motor_TaiHu *TaiHu_Device_T4;
extern Motor_TaiHu *TaiHu_Device_T5;
extern Motor_TaiHu *TaiHu_Device_T6;
extern Motor_TaiHu *TaiHu_Device_T7;
extern Motor_TaiHu *TaiHu_Device_T8;
extern Motor_TaiHu *TaiHu_Device_T9;
extern Motor_TaiHu *TaiHu_Device_T10;
extern Motor_TaiHu *TaiHu_Device_T11;
extern Motor_TaiHu *TaiHu_Device_T12;
extern Motor_TaiHu *TaiHu_Device_T13;
extern Motor_TaiHu *TaiHu_Device_T14;
extern Motor_TaiHu *TaiHu_Device_T15;
extern Motor_TaiHu *TaiHu_Device_T16;

extern RGB_Data RGB_Datas[2];


extern int hardware_init(string ADDR, string Config);

float Eyou_Speed_ = 1;
float Eyou_Acc_ = 1;
float Eyou_Dec_ = 1;
float qqqq_ = 0.0f;
float pos_,vel_,cur_ = NAN;
devices_pkg::msg::WBotDoubleMotor::SharedPtr get_message_motor = nullptr;


class W_Bot_Double_Node : public rclcpp::Node
{
public:
    W_Bot_Double_Node(
        const std::string& node_name,
        // const std::string& pub_topic,
        // const std::string& sub_topic,
         const std::string& dev_config
    )
    : Node(node_name)
    {
        std::string path = "install/devices_pkg/share/devices_pkg/YAML/W_Bot_Double/out/TOP.yaml";
        hardware_init(path, dev_config);
        publisher_Motor = this->create_publisher<devices_pkg::msg::WBotDoubleMotor>("wbot_motor_double_data", 10);
        subscription_Motor = this->create_subscription<devices_pkg::msg::WBotDoubleMotor>("wbot_motor_double_cmd", 10, \
            std::bind(&W_Bot_Double_Node::Motor_topic_callback, this, std::placeholders::_1));

        timer_Motor = this->create_wall_timer(
            std::chrono::milliseconds(10),
            std::bind(&W_Bot_Double_Node::motor_timer_callback, this));
 }
private:

    void Motor_topic_callback(const devices_pkg::msg::WBotDoubleMotor::SharedPtr msg) const
    {
        get_message_motor = msg;  
    }


    void motor_timer_callback()
    {

        if (get_message_motor != nullptr)
        {
            TaiHu_Device_T1->Send_MIT_PD_Control_Data(TaiHu_Device_1, get_message_motor->right_shoulder_pitch.pos,get_message_motor->right_shoulder_pitch.vel,get_message_motor->right_shoulder_pitch.tor, get_message_motor->right_shoulder_pitch.kp, get_message_motor->right_shoulder_pitch.kd);
            TaiHu_Device_T2->Send_MIT_PD_Control_Data(TaiHu_Device_2, get_message_motor->right_shoulder_roll.pos, get_message_motor->right_shoulder_roll.vel, get_message_motor->right_shoulder_roll.tor, get_message_motor->right_shoulder_roll.kp, get_message_motor->right_shoulder_roll.kd);
            TaiHu_Device_T3->Send_MIT_PD_Control_Data(TaiHu_Device_3, get_message_motor->right_shoulder_yaw.pos, get_message_motor->right_shoulder_yaw.vel, get_message_motor->right_shoulder_yaw.tor, get_message_motor->right_shoulder_yaw.kp, get_message_motor->right_shoulder_yaw.kd);
            TaiHu_Device_T4->Send_MIT_PD_Control_Data(TaiHu_Device_4, get_message_motor->right_elbow.pos, get_message_motor->right_elbow.vel, get_message_motor->right_elbow.tor, get_message_motor->right_elbow.kp, get_message_motor->right_elbow.kd);
            TaiHu_Device_T5->Send_MIT_PD_Control_Data(TaiHu_Device_5, get_message_motor->right_wrist_yaw.pos, get_message_motor->right_wrist_yaw.vel, get_message_motor->right_wrist_yaw.tor, get_message_motor->right_wrist_yaw.kp, get_message_motor->right_wrist_yaw.kd);
            TaiHu_Device_T6->Send_MIT_PD_Control_Data(TaiHu_Device_6, get_message_motor->right_wrist_pitch.pos, get_message_motor->right_wrist_pitch.vel, get_message_motor->right_wrist_pitch.tor, get_message_motor->right_wrist_pitch.kp, get_message_motor->right_wrist_pitch.kd);
            TaiHu_Device_T7->Send_MIT_PD_Control_Data(TaiHu_Device_7, get_message_motor->right_wrist_roll.pos, get_message_motor->right_wrist_roll.vel, get_message_motor->right_wrist_roll.tor, get_message_motor->right_wrist_roll.kp, get_message_motor->right_wrist_roll.kd);

            TaiHu_Device_T8->Send_MIT_PD_Control_Data(TaiHu_Device_8, get_message_motor->left_shoulder_pitch.pos, get_message_motor->left_shoulder_pitch.vel, get_message_motor->left_shoulder_pitch.tor, get_message_motor->left_shoulder_pitch.kp, get_message_motor->left_shoulder_pitch.kd);
            TaiHu_Device_T9->Send_MIT_PD_Control_Data(TaiHu_Device_9, get_message_motor->left_shoulder_roll.pos, get_message_motor->left_shoulder_roll.vel, get_message_motor->left_shoulder_roll.tor, get_message_motor->left_shoulder_roll.kp, get_message_motor->left_shoulder_roll.kd);
            TaiHu_Device_T10->Send_MIT_PD_Control_Data(TaiHu_Device_10, get_message_motor->left_shoulder_yaw.pos, get_message_motor->left_shoulder_yaw.vel, get_message_motor->left_shoulder_yaw.tor, get_message_motor->left_shoulder_yaw.kp, get_message_motor->left_shoulder_yaw.kd);
            TaiHu_Device_T11->Send_MIT_PD_Control_Data(TaiHu_Device_11, get_message_motor->left_elbow.pos, get_message_motor->left_elbow.vel, get_message_motor->left_elbow.tor, get_message_motor->left_elbow.kp, get_message_motor->left_elbow.kd);
            TaiHu_Device_T12->Send_MIT_PD_Control_Data(TaiHu_Device_12, get_message_motor->left_wrist_yaw.pos, get_message_motor->left_wrist_yaw.vel, get_message_motor->left_wrist_yaw.tor, get_message_motor->left_wrist_yaw.kp, get_message_motor->left_wrist_yaw.kd);
            TaiHu_Device_T13->Send_MIT_PD_Control_Data(TaiHu_Device_13, get_message_motor->left_wrist_pitch.pos, get_message_motor->left_wrist_pitch.vel, get_message_motor->left_wrist_pitch.tor, get_message_motor->left_wrist_pitch.kp, get_message_motor->left_wrist_pitch.kd);
            TaiHu_Device_T14->Send_MIT_PD_Control_Data(TaiHu_Device_14, get_message_motor->left_wrist_roll.pos, get_message_motor->left_wrist_roll.vel, get_message_motor->left_wrist_roll.tor, get_message_motor->left_wrist_roll.kp, get_message_motor->left_wrist_roll.kd);

            // TaiHu_Device_T15->Send_MIT_PD_Control_Data(TaiHu_Device_15, get_message_motor->head_pitch.pos, get_message_motor->head_pitch.vel, get_message_motor->head_pitch.tor, get_message_motor->head_pitch.kp, get_message_motor->head_pitch.kd);
            // TaiHu_Device_T16->Send_MIT_PD_Control_Data(TaiHu_Device_16, get_message_motor->head_yaw.pos, get_message_motor->head_yaw.vel, get_message_motor->head_yaw.tor, get_message_motor->head_yaw.kp, get_message_motor->head_yaw.kd);

            get_message_motor = nullptr;
        }

        auto send_message = devices_pkg::msg::WBotDoubleMotor();


        TaiHu_Device_T1->Get_Motor_FB_Data(TaiHu_Device_1, &send_message.right_shoulder_pitch.pos, &vel_, &cur_);
        TaiHu_Device_T2->Get_Motor_FB_Data(TaiHu_Device_2, &send_message.right_shoulder_roll.pos, &vel_, &cur_);
        TaiHu_Device_T3->Get_Motor_FB_Data(TaiHu_Device_3, &send_message.right_shoulder_yaw.pos, &vel_, &cur_);
        TaiHu_Device_T4->Get_Motor_FB_Data(TaiHu_Device_4, &send_message.right_elbow.pos, &vel_, &cur_);
        TaiHu_Device_T5->Get_Motor_FB_Data(TaiHu_Device_5, &send_message.right_wrist_yaw.pos, &vel_, &cur_);
        TaiHu_Device_T6->Get_Motor_FB_Data(TaiHu_Device_6, &send_message.right_wrist_pitch.pos, &vel_, &cur_);
        TaiHu_Device_T7->Get_Motor_FB_Data(TaiHu_Device_7, &send_message.right_wrist_roll.pos, &vel_, &cur_);

        TaiHu_Device_T8->Get_Motor_FB_Data(TaiHu_Device_8, &send_message.left_shoulder_pitch.pos, &vel_, &cur_);
        TaiHu_Device_T9->Get_Motor_FB_Data(TaiHu_Device_9, &send_message.left_shoulder_roll.pos, &vel_, &cur_);
        TaiHu_Device_T10->Get_Motor_FB_Data(TaiHu_Device_10, &send_message.left_shoulder_yaw.pos, &vel_, &cur_);
        TaiHu_Device_T11->Get_Motor_FB_Data(TaiHu_Device_11, &send_message.left_elbow.pos, &vel_, &cur_);
        TaiHu_Device_T12->Get_Motor_FB_Data(TaiHu_Device_12, &send_message.left_wrist_yaw.pos, &vel_, &cur_);
        TaiHu_Device_T13->Get_Motor_FB_Data(TaiHu_Device_13, &send_message.left_wrist_pitch.pos, &vel_, &cur_);
        TaiHu_Device_T14->Get_Motor_FB_Data(TaiHu_Device_14, &send_message.left_wrist_roll.pos, &vel_, &cur_);

        // TaiHu_Device_T15->Get_Motor_FB_Data(TaiHu_Device_15, &send_message.head_pitch.pos, &vel_, &cur_);
        // TaiHu_Device_T16->Get_Motor_FB_Data(TaiHu_Device_16, &send_message.head_yaw.pos, &vel_, &cur_);


        publisher_Motor->publish(send_message);

    }


    rclcpp::Publisher<devices_pkg::msg::WBotDoubleMotor>::SharedPtr publisher_Motor;
    rclcpp::Subscription<devices_pkg::msg::WBotDoubleMotor>::SharedPtr subscription_Motor;
    rclcpp::TimerBase::SharedPtr timer_Motor;
};

#endif // W_BOT_DOUBLE_NODE_H_


