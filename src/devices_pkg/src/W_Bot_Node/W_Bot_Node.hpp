#ifndef W_BOT_NODE_H_
#define W_BOT_NODE_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "UDP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Custom_TOP.hpp"
#include "Switch_Board.hpp"
#include <filesystem>
#include <iostream>
#include "devices_pkg/msg/w_bot_motor.hpp"
#include "devices_pkg/msg/w_bot_imu.hpp"
#include "devices_pkg/msg/w_bot_led.hpp"
#include "Eyou_Motor_TOP.hpp"
#include "Motor_TaiHu.hpp"
#include "IMU_YuanJi.hpp"
#include "Led_Device.hpp"
#include "Motor_BM_M1502D.hpp"

using namespace std;

//声明外部变量
extern Robot_Hardware *Test_Robot;
extern shared_ptr<Device_class>  Lower_Limbs_Motor_Waist_Roll;
extern shared_ptr<Device_class>  Lower_Limbs_Motor_Waist_Yaw;
extern shared_ptr<Device_class>  Lower_Limbs_Motor_Knee;
extern shared_ptr<Device_class>  Lower_Limbs_Motor_Hip;
extern shared_ptr<Device_class>  Lower_Limbs_Motor_Ankel;

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
extern shared_ptr<Device_class>  Classis_Motor_1;
extern shared_ptr<Device_class>  Classis_Motor_2;
extern shared_ptr<Device_class> IMU_Device_1;
extern shared_ptr<Device_class> IMU_Device_2;
extern shared_ptr<Device_class>  Led_Shoulder_Devices;
extern shared_ptr<Device_class>  Led_Chassis_Devices;
extern Eyou_Motor *Lower_Limbs_Motor_Waist_Roll_Ctl;
extern Eyou_Motor *Lower_Limbs_Motor_Waist_Yaw_Ctl;
extern Eyou_Motor *Lower_Limbs_Motor_Knee_Ctl;
extern Eyou_Motor *Lower_Limbs_Motor_Hip_Ctl;
extern Eyou_Motor *Lower_Limbs_Motor_Ankel_Ctl;
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

extern shared_ptr<Device_class>  Classis_Motor_1;
extern shared_ptr<Device_class>  Classis_Motor_2;
extern Motor_BM_M1502D *Classis_Motor_T1;
extern Motor_BM_M1502D *Classis_Motor_T2;

extern IMU_YuanJi *IMU_Device_T1;
extern IMU_YuanJi *IMU_Device_T2;

extern Led_Device *Led_Device_Shoulder_Ptr;
extern Led_Device *Led_Device_Chassis_Ptr;
extern RGB_Data RGB_Datas[2];


extern int hardware_init(string ADDR, string Config);

float Eyou_Speed_ = 1;
float Eyou_Acc_ = 1;
float Eyou_Dec_ = 1;
float qqqq_ = 0.0f;
float pos_,vel_,cur_ = 0.0f;
devices_pkg::msg::WBotMotor::SharedPtr get_message_motor = nullptr;


class W_Bot_Node : public rclcpp::Node
{
public:
    W_Bot_Node(
        // const std::string& node_name,
        // const std::string& pub_topic,
        // const std::string& sub_topic,
         const std::string& dev_config
    )
    : Node("w_bot_node")
    {
        std::string path = "src/devices_pkg/sdk/config/YAML/W_Bot/out/TOP.yaml";
        hardware_init(path, dev_config);
        publisher_Motor = this->create_publisher<devices_pkg::msg::WBotMotor>("wbot_motor_data", 10);
        publisher_IMU = this->create_publisher<devices_pkg::msg::WBotIMU>("wbot_imu_data", 10);
        subscription_Motor = this->create_subscription<devices_pkg::msg::WBotMotor>("wbot_motor_cmd", 10, \
            std::bind(&W_Bot_Node::Motor_topic_callback, this, std::placeholders::_1));
        subscription_LED = this->create_subscription<devices_pkg::msg::WBotLED>("wbot_led_cmd", 10, \
            std::bind(&W_Bot_Node::led_topic_callback, this, std::placeholders::_1));
        timer_imu = this->create_wall_timer(
            std::chrono::milliseconds(10),
            std::bind(&W_Bot_Node::imu_timer_callback, this));

        timer_Motor = this->create_wall_timer(
            std::chrono::milliseconds(10),
            std::bind(&W_Bot_Node::motor_timer_callback, this));
 }
private:

    void Motor_topic_callback(const devices_pkg::msg::WBotMotor::SharedPtr msg) const
    {
        get_message_motor = msg;  
    }

    void imu_timer_callback()
    {
        auto imu_message = devices_pkg::msg::WBotIMU();

        imu_message.imu_chest.pitch = IMU_Device_T1->Get_IMU_Frame_Data().pitch;
        imu_message.imu_chest.yaw = IMU_Device_T1->Get_IMU_Frame_Data().yaw;
        imu_message.imu_chest.roll = IMU_Device_T1->Get_IMU_Frame_Data().roll;
        imu_message.imu_chest.acc_x = IMU_Device_T1->Get_IMU_Frame_Data().acc_x;
        imu_message.imu_chest.acc_y = IMU_Device_T1->Get_IMU_Frame_Data().acc_y;
        imu_message.imu_chest.acc_z = IMU_Device_T1->Get_IMU_Frame_Data().acc_z;
        imu_message.imu_chest.gyro_x = IMU_Device_T1->Get_IMU_Frame_Data().gyro_x;
        imu_message.imu_chest.gyro_y = IMU_Device_T1->Get_IMU_Frame_Data().gyro_y;
        imu_message.imu_chest.gyro_z = IMU_Device_T1->Get_IMU_Frame_Data().gyro_z;

        imu_message.imu_chassis.pitch = IMU_Device_T2->Get_IMU_Frame_Data().pitch;
        imu_message.imu_chassis.yaw = IMU_Device_T2->Get_IMU_Frame_Data().yaw;
        imu_message.imu_chassis.roll = IMU_Device_T2->Get_IMU_Frame_Data().roll;
        imu_message.imu_chassis.acc_x = IMU_Device_T2->Get_IMU_Frame_Data().acc_x;
        imu_message.imu_chassis.acc_y = IMU_Device_T2->Get_IMU_Frame_Data().acc_y;
        imu_message.imu_chassis.acc_z = IMU_Device_T2->Get_IMU_Frame_Data().acc_z;
        imu_message.imu_chassis.gyro_x = IMU_Device_T2->Get_IMU_Frame_Data().gyro_x;
        imu_message.imu_chassis.gyro_y = IMU_Device_T2->Get_IMU_Frame_Data().gyro_y;
        imu_message.imu_chassis.gyro_z = IMU_Device_T2->Get_IMU_Frame_Data().gyro_z;

        publisher_IMU->publish(imu_message);
    }

    void motor_timer_callback()
    {

        if (get_message_motor != nullptr)
        {
            Lower_Limbs_Motor_Waist_Yaw_Ctl->Send_MIT_PD_Control_Data(Lower_Limbs_Motor_Waist_Yaw, get_message_motor->waist_yaw.pos, Eyou_Speed_, 1, Eyou_Acc_, Eyou_Dec_);
            Lower_Limbs_Motor_Waist_Roll_Ctl->Send_MIT_PD_Control_Data(Lower_Limbs_Motor_Waist_Roll, get_message_motor->waist_roll.pos, Eyou_Speed_, 1, Eyou_Acc_, Eyou_Dec_);
            Lower_Limbs_Motor_Knee_Ctl->Send_MIT_PD_Control_Data(Lower_Limbs_Motor_Knee, get_message_motor->knee.pos, Eyou_Speed_, 1, Eyou_Acc_, Eyou_Dec_);
            Lower_Limbs_Motor_Hip_Ctl->Send_MIT_PD_Control_Data(Lower_Limbs_Motor_Hip, get_message_motor->hip.pos, Eyou_Speed_, 1, Eyou_Acc_, Eyou_Dec_);
            Lower_Limbs_Motor_Ankel_Ctl->Send_MIT_PD_Control_Data(Lower_Limbs_Motor_Ankel, get_message_motor->ankle.pos, Eyou_Speed_, 1, Eyou_Acc_, Eyou_Dec_);
            TaiHu_Device_T1->Send_MIT_PD_Control_Data(TaiHu_Device_1, get_message_motor->left_shoulder_pitch.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T2->Send_MIT_PD_Control_Data(TaiHu_Device_2, get_message_motor->left_shoulder_roll.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T3->Send_MIT_PD_Control_Data(TaiHu_Device_3, get_message_motor->left_shoulder_yaw.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T4->Send_MIT_PD_Control_Data(TaiHu_Device_4, get_message_motor->left_elbow.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T5->Send_MIT_PD_Control_Data(TaiHu_Device_5, get_message_motor->left_wrist_yaw.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T6->Send_MIT_PD_Control_Data(TaiHu_Device_6, get_message_motor->left_wrist_pitch.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T7->Send_MIT_PD_Control_Data(TaiHu_Device_7, get_message_motor->left_wrist_roll.pos, 0.0f, 0.0f, 0.0f, 0.0f);

            TaiHu_Device_T8->Send_MIT_PD_Control_Data(TaiHu_Device_8, get_message_motor->right_shoulder_pitch.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T9->Send_MIT_PD_Control_Data(TaiHu_Device_9, get_message_motor->right_shoulder_roll.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T10->Send_MIT_PD_Control_Data(TaiHu_Device_10, get_message_motor->right_shoulder_yaw.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T11->Send_MIT_PD_Control_Data(TaiHu_Device_11, get_message_motor->right_elbow.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T12->Send_MIT_PD_Control_Data(TaiHu_Device_12, get_message_motor->right_wrist_yaw.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T13->Send_MIT_PD_Control_Data(TaiHu_Device_13, get_message_motor->right_wrist_pitch.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T14->Send_MIT_PD_Control_Data(TaiHu_Device_14, get_message_motor->right_wrist_roll.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T15->Send_MIT_PD_Control_Data(TaiHu_Device_15, get_message_motor->head_pitch.pos, 0.0f, 0.0f, 0.0f, 0.0f);
            TaiHu_Device_T16->Send_MIT_PD_Control_Data(TaiHu_Device_16, get_message_motor->head_yaw.pos, 0.0f, 0.0f, 0.0f, 0.0f);

            Classis_Motor_T1->Send_MIT_PD_Control_Data(Classis_Motor_1, 0.0f, get_message_motor->wheel_left.vel, 0.0f, 0.0f, 0.0f);
            Classis_Motor_T2->Send_MIT_PD_Control_Data(Classis_Motor_2, 0.0f, get_message_motor->wheel_right.vel, 0.0f, 0.0f, 0.0f);
        }

        auto send_message = devices_pkg::msg::WBotMotor();

        Lower_Limbs_Motor_Waist_Yaw_Ctl->Get_Motor_FB_Data(Lower_Limbs_Motor_Waist_Yaw, &send_message.waist_yaw.pos, &Eyou_Speed_, &qqqq_);
        Lower_Limbs_Motor_Waist_Roll_Ctl->Get_Motor_FB_Data(Lower_Limbs_Motor_Waist_Roll, &send_message.waist_roll.pos, &Eyou_Speed_, &qqqq_);
        Lower_Limbs_Motor_Knee_Ctl->Get_Motor_FB_Data(Lower_Limbs_Motor_Knee, &send_message.knee.pos, &Eyou_Speed_, &qqqq_);
        
        Lower_Limbs_Motor_Hip_Ctl->Get_Motor_FB_Data(Lower_Limbs_Motor_Hip, &send_message.hip.pos, &Eyou_Speed_, &qqqq_);
        Lower_Limbs_Motor_Ankel_Ctl->Get_Motor_FB_Data(Lower_Limbs_Motor_Ankel, &send_message.ankle.pos, &Eyou_Speed_, &qqqq_);
        TaiHu_Device_T1->Get_Motor_FB_Data(TaiHu_Device_1, &send_message.left_shoulder_pitch.pos, &vel_, &cur_);
        TaiHu_Device_T2->Get_Motor_FB_Data(TaiHu_Device_2, &send_message.left_shoulder_roll.pos, &vel_, &cur_);
        TaiHu_Device_T3->Get_Motor_FB_Data(TaiHu_Device_3, &send_message.left_shoulder_yaw.pos, &vel_, &cur_);
        TaiHu_Device_T4->Get_Motor_FB_Data(TaiHu_Device_4, &send_message.left_elbow.pos, &vel_, &cur_);
        TaiHu_Device_T5->Get_Motor_FB_Data(TaiHu_Device_5, &send_message.left_wrist_yaw.pos, &vel_, &cur_);
        TaiHu_Device_T6->Get_Motor_FB_Data(TaiHu_Device_6, &send_message.left_wrist_roll.pos, &vel_, &cur_);

        TaiHu_Device_T7->Get_Motor_FB_Data(TaiHu_Device_7, &send_message.right_shoulder_pitch.pos, &vel_, &cur_);
        TaiHu_Device_T8->Get_Motor_FB_Data(TaiHu_Device_8, &send_message.right_shoulder_roll.pos, &vel_, &cur_);
        TaiHu_Device_T9->Get_Motor_FB_Data(TaiHu_Device_9, &send_message.right_shoulder_yaw.pos, &vel_, &cur_);
        TaiHu_Device_T10->Get_Motor_FB_Data(TaiHu_Device_10, &send_message.right_elbow.pos, &vel_, &cur_);
        TaiHu_Device_T11->Get_Motor_FB_Data(TaiHu_Device_11, &send_message.right_wrist_yaw.pos, &vel_, &cur_);
        TaiHu_Device_T12->Get_Motor_FB_Data(TaiHu_Device_12, &send_message.right_wrist_pitch.pos, &vel_, &cur_);
        TaiHu_Device_T13->Get_Motor_FB_Data(TaiHu_Device_13, &send_message.right_wrist_roll.pos, &vel_, &cur_);
        TaiHu_Device_T14->Get_Motor_FB_Data(TaiHu_Device_14, &send_message.head_pitch.pos, &vel_, &cur_);
        TaiHu_Device_T15->Get_Motor_FB_Data(TaiHu_Device_15, &send_message.head_yaw.pos, &vel_, &cur_);

        Classis_Motor_T1->Get_Motor_FB_Data(Classis_Motor_1, &send_message.wheel_left.pos, &send_message.wheel_left.vel, &send_message.wheel_left.tor);
        Classis_Motor_T2->Get_Motor_FB_Data(Classis_Motor_2, &send_message.wheel_right.pos, &send_message.wheel_right.vel, &send_message.wheel_right.tor);


        publisher_Motor->publish(send_message);

    }


    void led_topic_callback(const devices_pkg::msg::WBotLED::SharedPtr msg) const
    {
        auto get_message = msg;

            for(int i = 0; i < 340; i++)
            {
                RGB_Datas[0].R_data[i] = get_message->led_upper.arr_r[i];
                RGB_Datas[0].G_data[i] = get_message->led_upper.arr_g[i];
                RGB_Datas[0].B_data[i] = get_message->led_upper.arr_b[i];
    
                RGB_Datas[1].R_data[i] = get_message->led_lower.arr_r[i];
                RGB_Datas[1].G_data[i] = get_message->led_lower.arr_g[i];
                RGB_Datas[1].B_data[i] = get_message->led_lower.arr_b[i];
            }
        Led_Device_Chassis_Ptr ->Led_Set_Every_Color(Led_Chassis_Devices,  1, 1, 11, &RGB_Datas[0]);
        Led_Device_Shoulder_Ptr->Led_Set_Every_Color(Led_Shoulder_Devices, 1, 1, 11, &RGB_Datas[1]);

    }

    rclcpp::Publisher<devices_pkg::msg::WBotMotor>::SharedPtr publisher_Motor;
    rclcpp::Publisher<devices_pkg::msg::WBotIMU>::SharedPtr publisher_IMU;
    rclcpp::Subscription<devices_pkg::msg::WBotMotor>::SharedPtr subscription_Motor;
    rclcpp::Subscription<devices_pkg::msg::WBotLED>::SharedPtr subscription_LED;
    rclcpp::TimerBase::SharedPtr timer_imu;
    rclcpp::TimerBase::SharedPtr timer_Motor;
};

#endif // W_BOT_NODE_H_


