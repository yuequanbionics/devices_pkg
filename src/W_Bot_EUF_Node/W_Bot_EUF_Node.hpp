#ifndef W_BOT_EUF_NODE_H_
#define W_BOT_EUF_NODE_H_

#include <math.h>

#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include "Battery_BMS_V2.hpp"
#include "Custom_TOP.hpp"
#include "GPIO.hpp"
#include "HARDWARE_TOP.hpp"
#include "IMU_YuanJi.hpp"
#include "Led_Device.hpp"
#include "Motor_BM_M1502D.hpp"
#include "Switch_Board.hpp"
#include "Phu_Motor_TOP.hpp"
#include "UDP.hpp"
#include "devices_pkg/msg/w_bot_battery.hpp"
#include "devices_pkg/msg/w_bot_collision.hpp"
#include "devices_pkg/msg/w_bot_imu.hpp"
#include "devices_pkg/msg/w_bot_led.hpp"
#include "devices_pkg/msg/w_bot_motor.hpp"
#include "devices_pkg/msg/w_bot_motor_tem.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "syst.hpp"
#include <ament_index_cpp/get_package_share_directory.hpp>

using namespace std;

// 声明外部变量
extern Robot_Hardware* Test_Robot;
extern shared_ptr<Device_class> Chassis_Main_Switch_Board;
extern shared_ptr<Device_class> Lower_Limbs_Motor_Ankel_N31;
extern shared_ptr<Device_class> Lower_Limbs_Motor_Knee_N32;
extern shared_ptr<Device_class> Lower_Limbs_Motor_Hip_N33;
extern shared_ptr<Device_class> Lower_Limbs_Motor_Waist_Roll_N34;
extern shared_ptr<Device_class> Lower_Limbs_Motor_Waist_Yaw_N35;

extern shared_ptr<Device_class> left_shoulder_pitch_N11;
extern shared_ptr<Device_class> left_shoulder_roll_N12;
extern shared_ptr<Device_class> left_shoulder_yaw_N13;
extern shared_ptr<Device_class> left_elbow_N14;
extern shared_ptr<Device_class> left_wrist_yaw_N15;
extern shared_ptr<Device_class> left_wrist_pitch_N16;
extern shared_ptr<Device_class> left_wrist_roll_N17;

extern shared_ptr<Device_class> right_shoulder_pitch_N21;
extern shared_ptr<Device_class> right_shoulder_roll_N22;
extern shared_ptr<Device_class> right_shoulder_yaw_N23;
extern shared_ptr<Device_class> right_elbow_N24;
extern shared_ptr<Device_class> right_wrist_yaw_N25;
extern shared_ptr<Device_class> right_wrist_pitch_N26;
extern shared_ptr<Device_class> right_wrist_roll_N27;

// extern shared_ptr<Device_class> TaiHu_Device_15;
// extern shared_ptr<Device_class> TaiHu_Device_16;

extern shared_ptr<Device_class> Classis_Motor_1;
extern shared_ptr<Device_class> Classis_Motor_2;
extern shared_ptr<Device_class> IMU_Device_1;
extern shared_ptr<Device_class> IMU_Device_2;
extern shared_ptr<Device_class> Led_Shoulder_Devices;
extern shared_ptr<Device_class> Led_Chassis_Devices;
extern shared_ptr<Device_class> Battery_BMS_V2_1;
extern shared_ptr<Device_class> Battery_BMS_V2_2;
extern shared_ptr<Device_class> Battery_BMS_V2_3;
extern shared_ptr<Device_class> Battery_BMS_V2_4;

extern Main_B* Chassis_Main_Switch_Board_Control;

extern Phu_Motor* Lower_Limbs_Motor_Ankel_N31_Ctl;
extern Phu_Motor* Lower_Limbs_Motor_Knee_N32_Ctl;
extern Phu_Motor* Lower_Limbs_Motor_Hip_N33_Ctl;
extern Phu_Motor* Lower_Limbs_Motor_Waist_Roll_N34_Ctl;
extern Phu_Motor* Lower_Limbs_Motor_Waist_Yaw_N35_Ctl;

extern Phu_Motor* left_shoulder_pitch_N11_Ctl;
extern Phu_Motor* left_shoulder_roll_N12_Ctl;
extern Phu_Motor* left_shoulder_yaw_N13_Ctl;
extern Phu_Motor* left_elbow_N14_Ctl;
extern Phu_Motor* left_wrist_yaw_N15_Ctl;
extern Phu_Motor* left_wrist_pitch_N16_Ctl;
extern Phu_Motor* left_wrist_roll_N17_Ctl;

extern Phu_Motor* right_shoulder_pitch_N21_Ctl;
extern Phu_Motor* right_shoulder_roll_N22_Ctl;
extern Phu_Motor* right_shoulder_yaw_N23_Ctl;
extern Phu_Motor* right_elbow_N24_Ctl;
extern Phu_Motor* right_wrist_yaw_N25_Ctl;
extern Phu_Motor* right_wrist_pitch_N26_Ctl;
extern Phu_Motor* right_wrist_roll_N27_Ctl;

// extern Phu_Motor* TaiHu_Device_T15;
// extern Phu_Motor* TaiHu_Device_T16;

extern BMS_V2_Protocol* Battery_BMS_V2_T1;
extern BMS_V2_Protocol* Battery_BMS_V2_T2;
extern BMS_V2_Protocol* Battery_BMS_V2_T3;
extern BMS_V2_Protocol* Battery_BMS_V2_T4;

extern shared_ptr<Device_class> Classis_Motor_1;
extern shared_ptr<Device_class> Classis_Motor_2;
extern Motor_BM_M1502D* Classis_Motor_T1;
extern Motor_BM_M1502D* Classis_Motor_T2;

extern IMU_YuanJi* IMU_Device_T1;
extern IMU_YuanJi* IMU_Device_T2;

extern Led_Device* Led_Device_Shoulder_Ptr;
extern Led_Device* Led_Device_Chassis_Ptr;
extern RGB_Data RGB_Datas[2];

BMS_Consolidated_Data current_data_1;  // 电池数据
BMS_Consolidated_Data current_data_2;  // 电池数据
BMS_Consolidated_Data current_data_3;  // 电池数据
BMS_Consolidated_Data current_data_4;  // 电池数据

// extern int (*Wbot_Error_DL_ROS)(shared_ptr<Device_class> Device, std::string Info, Err_Level Level);

extern int hardware_init(const string& ADDR, const string& Config);

float Eyou_Speed_ = 1;
float Eyou_Acc_ = 1;
float Eyou_Dec_ = 1;
float qqqq_ = 0.0f;
float pos_, vel_, cur_ = NAN;
devices_pkg::msg::WBotMotor::SharedPtr get_message_motor = nullptr;

class W_Bot_EUF_Node : public rclcpp::Node {
   public:

   static W_Bot_EUF_Node* self_ptr;
    W_Bot_EUF_Node(
        const std::string& node_name,
        // const std::string& pub_topic,
        // const std::string& sub_topic,
        const std::string& dev_config)
        : Node(node_name) {
        self_ptr = this;
        //std::string yaml_path = "install/devices_pkg/share/devices_pkg/YAML/W_Bot/out/TOP.yaml";
        auto yaml_path = ament_index_cpp::get_package_share_directory("devices_pkg") + "/YAML/W_Bot/out/TOP.yaml";
        hardware_init(yaml_path, dev_config);
        std::string set_name = "wbot";
        // std::string set_name = node_name;

        auto SensorQos = rclcpp::SensorDataQoS();
        publisher_Motor = this->create_publisher<devices_pkg::msg::WBotMotor>(set_name + "_motor_data", SensorQos);
        publisher_IMU = this->create_publisher<devices_pkg::msg::WBotIMU>(set_name + "_imu_data", SensorQos);
        publisher_Battery = this->create_publisher<devices_pkg::msg::WBotBattery>(set_name + "_battery_data", SensorQos);
        publisher_Collision = this->create_publisher<devices_pkg::msg::WBotCollision>(set_name + "_collision_data", SensorQos);
        publisher_MotorTem = this->create_publisher<devices_pkg::msg::WBotMotorTem>(set_name + "_motor_tem_data", SensorQos);
        subscription_Motor = this->create_subscription<devices_pkg::msg::WBotMotor>(set_name + "_motor_cmd", SensorQos,
                                                                                    std::bind(&W_Bot_EUF_Node::Motor_topic_callback, this, std::placeholders::_1));
        subscription_LED = this->create_subscription<devices_pkg::msg::WBotLED>(set_name + "_led_cmd", SensorQos,
                                                                                std::bind(&W_Bot_EUF_Node::led_topic_callback, this, std::placeholders::_1));
        subscription_Motor_Stop = this->create_subscription<std_msgs::msg::String>(set_name + "_motor_stop", SensorQos, \
            std::bind(&W_Bot_EUF_Node::Motor_Stop_topic_callback, this, std::placeholders::_1));

        publisher_Error = this->create_publisher<std_msgs::msg::String>(set_name + "_error_data", SensorQos);

        timer_imu = this->create_wall_timer(
            std::chrono::milliseconds(10),
            std::bind(&W_Bot_EUF_Node::imu_timer_callback, this));

        timer_Motor = this->create_wall_timer(
            std::chrono::milliseconds(10),
            std::bind(&W_Bot_EUF_Node::motor_timer_callback, this));

        timer_Battery = this->create_wall_timer(
            std::chrono::milliseconds(1000),
            std::bind(&W_Bot_EUF_Node::battery_timer_callback, this));

        timer_Collision_Bar = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&W_Bot_EUF_Node::Collision_timer_callback, this));

        timer_Temp_Get = this->create_wall_timer(
            std::chrono::milliseconds(1000),
            std::bind(&W_Bot_EUF_Node::Temp_Get_callback, this));

        // Wbot_Error_DL_ROS = Wbot_Error_callback;
    }

   private:
    void Motor_topic_callback(const devices_pkg::msg::WBotMotor::SharedPtr msg) const {
        get_message_motor = msg;
    }

    void imu_timer_callback() {
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

    void motor_timer_callback() {
         if (get_message_motor != nullptr) {
            Lower_Limbs_Motor_Waist_Yaw_N35_Ctl  -> Send_MIT_PD_Control_Data(Lower_Limbs_Motor_Waist_Yaw_N35,  get_message_motor->waist_yaw.pos,   Eyou_Speed_, 1, Eyou_Acc_, Eyou_Dec_);
            Lower_Limbs_Motor_Waist_Roll_N34_Ctl -> Send_MIT_PD_Control_Data(Lower_Limbs_Motor_Waist_Roll_N34, get_message_motor->waist_roll.pos,  Eyou_Speed_, 1, Eyou_Acc_, Eyou_Dec_);
            Lower_Limbs_Motor_Knee_N32_Ctl       -> Send_MIT_PD_Control_Data(Lower_Limbs_Motor_Knee_N32,       get_message_motor->knee.pos,        Eyou_Speed_, 1, Eyou_Acc_, Eyou_Dec_);
            Lower_Limbs_Motor_Hip_N33_Ctl        -> Send_MIT_PD_Control_Data(Lower_Limbs_Motor_Hip_N33,        get_message_motor->hip.pos,         Eyou_Speed_, 1, Eyou_Acc_, Eyou_Dec_);
            Lower_Limbs_Motor_Ankel_N31_Ctl      -> Send_MIT_PD_Control_Data(Lower_Limbs_Motor_Ankel_N31,      get_message_motor->ankle.pos,       Eyou_Speed_, 1, Eyou_Acc_, Eyou_Dec_);

            left_shoulder_pitch_N11_Ctl -> Send_MIT_PD_Control_Data(left_shoulder_pitch_N11, get_message_motor->left_shoulder_pitch.pos, get_message_motor->left_shoulder_pitch.vel, get_message_motor->left_shoulder_pitch.tor, get_message_motor->left_shoulder_pitch.kp, get_message_motor->left_shoulder_pitch.kd);
            left_shoulder_roll_N12_Ctl  -> Send_MIT_PD_Control_Data(left_shoulder_roll_N12,  get_message_motor->left_shoulder_roll.pos,  get_message_motor->left_shoulder_roll.vel,  get_message_motor->left_shoulder_roll.tor,  get_message_motor->left_shoulder_roll.kp,  get_message_motor->left_shoulder_roll.kd );
            left_shoulder_yaw_N13_Ctl   -> Send_MIT_PD_Control_Data(left_shoulder_yaw_N13,   get_message_motor->left_shoulder_yaw.pos,   get_message_motor->left_shoulder_yaw.vel,   get_message_motor->left_shoulder_yaw.tor,   get_message_motor->left_shoulder_yaw.kp,   get_message_motor->left_shoulder_yaw.kd  );
            left_elbow_N14_Ctl          -> Send_MIT_PD_Control_Data(left_elbow_N14,          get_message_motor->left_elbow.pos,          get_message_motor->left_elbow.vel,          get_message_motor->left_elbow.tor,          get_message_motor->left_elbow.kp,          get_message_motor->left_elbow.kd         );
            left_wrist_yaw_N15_Ctl      -> Send_MIT_PD_Control_Data(left_wrist_yaw_N15,      get_message_motor->left_wrist_yaw.pos,      get_message_motor->left_wrist_yaw.vel,      get_message_motor->left_wrist_yaw.tor,      get_message_motor->left_wrist_yaw.kp,      get_message_motor->left_wrist_yaw.kd     );
            left_wrist_pitch_N16_Ctl    -> Send_MIT_PD_Control_Data(left_wrist_pitch_N16,    get_message_motor->left_wrist_pitch.pos,    get_message_motor->left_wrist_pitch.vel,    get_message_motor->left_wrist_pitch.tor,    get_message_motor->left_wrist_pitch.kp,    get_message_motor->left_wrist_pitch.kd   );
            left_wrist_roll_N17_Ctl     -> Send_MIT_PD_Control_Data(left_wrist_roll_N17,     get_message_motor->left_wrist_roll.pos,     get_message_motor->left_wrist_roll.vel,     get_message_motor->left_wrist_roll.tor,     get_message_motor->left_wrist_roll.kp,     get_message_motor->left_wrist_roll.kd    );

            right_shoulder_pitch_N21_Ctl -> Send_MIT_PD_Control_Data(right_shoulder_pitch_N21, get_message_motor->right_shoulder_pitch.pos, get_message_motor->right_shoulder_pitch.vel, get_message_motor->right_shoulder_pitch.tor, get_message_motor->right_shoulder_pitch.kp, get_message_motor->right_shoulder_pitch.kd);
            right_shoulder_roll_N22_Ctl  -> Send_MIT_PD_Control_Data(right_shoulder_roll_N22,  get_message_motor->right_shoulder_roll.pos,  get_message_motor->right_shoulder_roll.vel,  get_message_motor->right_shoulder_roll.tor,  get_message_motor->right_shoulder_roll.kp,  get_message_motor->right_shoulder_roll.kd );
            right_shoulder_yaw_N23_Ctl   -> Send_MIT_PD_Control_Data(right_shoulder_yaw_N23,   get_message_motor->right_shoulder_yaw.pos,   get_message_motor->right_shoulder_yaw.vel,   get_message_motor->right_shoulder_yaw.tor,   get_message_motor->right_shoulder_yaw.kp,   get_message_motor->right_shoulder_yaw.kd  );
            right_elbow_N24_Ctl          -> Send_MIT_PD_Control_Data(right_elbow_N24,          get_message_motor->right_elbow.pos,          get_message_motor->right_elbow.vel,          get_message_motor->right_elbow.tor,          get_message_motor->right_elbow.kp,          get_message_motor->right_elbow.kd         );
            right_wrist_yaw_N25_Ctl      -> Send_MIT_PD_Control_Data(right_wrist_yaw_N25,      get_message_motor->right_wrist_yaw.pos,      get_message_motor->right_wrist_yaw.vel,      get_message_motor->right_wrist_yaw.tor,      get_message_motor->right_wrist_yaw.kp,      get_message_motor->right_wrist_yaw.kd     );
            right_wrist_pitch_N26_Ctl    -> Send_MIT_PD_Control_Data(right_wrist_pitch_N26,    get_message_motor->right_wrist_pitch.pos,    get_message_motor->right_wrist_pitch.vel,    get_message_motor->right_wrist_pitch.tor,    get_message_motor->right_wrist_pitch.kp,    get_message_motor->right_wrist_pitch.kd   );
            right_wrist_roll_N27_Ctl     -> Send_MIT_PD_Control_Data(right_wrist_roll_N27,     get_message_motor->right_wrist_roll.pos,     get_message_motor->right_wrist_roll.vel,     get_message_motor->right_wrist_roll.tor,     get_message_motor->right_wrist_roll.kp,     get_message_motor->right_wrist_roll.kd    );

            // TaiHu_Device_T15->Send_MIT_PD_Control_Data(TaiHu_Device_15, get_message_motor->head_pitch.pos, get_message_motor->head_pitch.vel, get_message_motor->head_pitch.tor, get_message_motor->head_pitch.kp, get_message_motor->head_pitch.kd);
            // TaiHu_Device_T16->Send_MIT_PD_Control_Data(TaiHu_Device_16, get_message_motor->head_yaw.pos, get_message_motor->head_yaw.vel, get_message_motor->head_yaw.tor, get_message_motor->head_yaw.kp, get_message_motor->head_yaw.kd);

            Classis_Motor_T1->Send_MIT_PD_Control_Data(Classis_Motor_1, get_message_motor->wheel_left.pos, get_message_motor->wheel_left.vel, 0.0f, get_message_motor->wheel_left.kp, get_message_motor->wheel_left.kd);
            Classis_Motor_T2->Send_MIT_PD_Control_Data(Classis_Motor_2, get_message_motor->wheel_right.pos, get_message_motor->wheel_right.vel, 0.0f, get_message_motor->wheel_right.kp, get_message_motor->wheel_right.kd);

            get_message_motor = nullptr;
        }

        auto send_message = devices_pkg::msg::WBotMotor();

        Lower_Limbs_Motor_Waist_Yaw_N35_Ctl  -> Get_Motor_FB_Data(Lower_Limbs_Motor_Waist_Yaw_N35,  &send_message.waist_yaw.pos,   &send_message.waist_yaw.vel,  &send_message.waist_yaw.tor );
        Lower_Limbs_Motor_Waist_Roll_N34_Ctl -> Get_Motor_FB_Data(Lower_Limbs_Motor_Waist_Roll_N34, &send_message.waist_roll.pos,  &send_message.waist_roll.vel, &send_message.waist_roll.tor);
        Lower_Limbs_Motor_Knee_N32_Ctl       -> Get_Motor_FB_Data(Lower_Limbs_Motor_Knee_N32,       &send_message.knee.pos,        &send_message.knee.vel,       &send_message.knee.tor      );
        Lower_Limbs_Motor_Hip_N33_Ctl        -> Get_Motor_FB_Data(Lower_Limbs_Motor_Hip_N33,        &send_message.hip.pos,         &send_message.hip.vel,        &send_message.hip.tor       );
        Lower_Limbs_Motor_Ankel_N31_Ctl      -> Get_Motor_FB_Data(Lower_Limbs_Motor_Ankel_N31,      &send_message.ankle.pos,       &send_message.ankle.vel,      &send_message.ankle.tor     );

        left_shoulder_pitch_N11_Ctl -> Get_Motor_FB_Data(left_shoulder_pitch_N11, &send_message.left_shoulder_pitch.pos, &send_message.left_shoulder_pitch.vel, &send_message.left_shoulder_pitch.tor);
        left_shoulder_roll_N12_Ctl  -> Get_Motor_FB_Data(left_shoulder_roll_N12,  &send_message.left_shoulder_roll.pos,  &send_message.left_shoulder_roll.vel,  &send_message.left_shoulder_roll.tor );
        left_shoulder_yaw_N13_Ctl   -> Get_Motor_FB_Data(left_shoulder_yaw_N13,   &send_message.left_shoulder_yaw.pos,   &send_message.left_shoulder_yaw.vel,   &send_message.left_shoulder_yaw.tor  );
        left_elbow_N14_Ctl          -> Get_Motor_FB_Data(left_elbow_N14,          &send_message.left_elbow.pos,          &send_message.left_elbow.vel,          &send_message.left_elbow.tor         );
        left_wrist_yaw_N15_Ctl      -> Get_Motor_FB_Data(left_wrist_yaw_N15,      &send_message.left_wrist_yaw.pos,      &send_message.left_wrist_yaw.vel,      &send_message.left_wrist_yaw.tor     );
        left_wrist_pitch_N16_Ctl    -> Get_Motor_FB_Data(left_wrist_pitch_N16,    &send_message.left_wrist_pitch.pos,    &send_message.left_wrist_pitch.vel,    &send_message.left_wrist_pitch.tor   );
        left_wrist_roll_N17_Ctl     -> Get_Motor_FB_Data(left_wrist_roll_N17,     &send_message.left_wrist_roll.pos,     &send_message.left_wrist_roll.vel,     &send_message.left_wrist_roll.tor    );

        right_shoulder_pitch_N21_Ctl -> Get_Motor_FB_Data(right_shoulder_pitch_N21, &send_message.right_shoulder_pitch.pos, &send_message.right_shoulder_pitch.vel, &send_message.right_shoulder_pitch.tor);
        right_shoulder_roll_N22_Ctl  -> Get_Motor_FB_Data(right_shoulder_roll_N22,  &send_message.right_shoulder_roll.pos,  &send_message.right_shoulder_roll.vel,  &send_message.right_shoulder_roll.tor );
        right_shoulder_yaw_N23_Ctl   -> Get_Motor_FB_Data(right_shoulder_yaw_N23,   &send_message.right_shoulder_yaw.pos,   &send_message.right_shoulder_yaw.vel,   &send_message.right_shoulder_yaw.tor  );
        right_elbow_N24_Ctl          -> Get_Motor_FB_Data(right_elbow_N24,          &send_message.right_elbow.pos,          &send_message.right_elbow.vel,          &send_message.right_elbow.tor         );
        right_wrist_yaw_N25_Ctl      -> Get_Motor_FB_Data(right_wrist_yaw_N25,      &send_message.right_wrist_yaw.pos,      &send_message.right_wrist_yaw.vel,      &send_message.right_wrist_yaw.tor     );
        right_wrist_pitch_N26_Ctl    -> Get_Motor_FB_Data(right_wrist_pitch_N26,    &send_message.right_wrist_pitch.pos,    &send_message.right_wrist_pitch.vel,    &send_message.right_wrist_pitch.tor   );
        right_wrist_roll_N27_Ctl     -> Get_Motor_FB_Data(right_wrist_roll_N27,     &send_message.right_wrist_roll.pos,     &send_message.right_wrist_roll.vel,     &send_message.right_wrist_roll.tor    );

        // TaiHu_Device_T15->Get_Motor_FB_Data(TaiHu_Device_15, &send_message.head_pitch.pos, &send_message.head_pitch.vel, &send_message.head_pitch.tor);
        // TaiHu_Device_T16->Get_Motor_FB_Data(TaiHu_Device_16, &send_message.head_yaw.pos, &send_message.head_yaw.vel, &send_message.head_yaw.tor);

        Classis_Motor_T1->Get_Motor_FB_Data(Classis_Motor_1, &send_message.wheel_left.pos, &send_message.wheel_left.vel, &send_message.wheel_left.tor);
        Classis_Motor_T2->Get_Motor_FB_Data(Classis_Motor_2, &send_message.wheel_right.pos, &send_message.wheel_right.vel, &send_message.wheel_right.tor);

        publisher_Motor->publish(send_message);
    }

    void battery_timer_callback() {
        auto battery_message = devices_pkg::msg::WBotBattery();

        current_data_1 = Battery_BMS_V2_T1->Get_Consolidated_Data();
        current_data_2 = Battery_BMS_V2_T2->Get_Consolidated_Data();
        current_data_3 = Battery_BMS_V2_T3->Get_Consolidated_Data();
        current_data_4 = Battery_BMS_V2_T4->Get_Consolidated_Data();

        battery_message.battery_left.total_voltage = current_data_1.total_voltage;
        battery_message.battery_left.current = current_data_1.current;
        battery_message.battery_left.soc = current_data_1.soc;
        battery_message.battery_left.soh = current_data_1.soh;

        battery_message.battery_right.total_voltage = current_data_2.total_voltage;
        battery_message.battery_right.current = current_data_2.current;
        battery_message.battery_right.soc = current_data_2.soc;
        battery_message.battery_right.soh = current_data_2.soh;

        battery_message.battery_lower.total_voltage = current_data_3.total_voltage;
        battery_message.battery_lower.current = current_data_3.current;
        battery_message.battery_lower.soc = current_data_3.soc;
        battery_message.battery_lower.soh = current_data_3.soh;

        battery_message.battery_upper.total_voltage = current_data_4.total_voltage;
        battery_message.battery_upper.current = current_data_4.current;
        battery_message.battery_upper.soc = current_data_4.soc;
        battery_message.battery_upper.soh = current_data_4.soh;

        // 发布电池状态消息
        publisher_Battery->publish(battery_message);
    }

    void Collision_timer_callback() {
        auto collision_message = devices_pkg::msg::WBotCollision();
        if (Chassis_Main_Switch_Board_Control == nullptr || Chassis_Main_Switch_Board == nullptr) {
            cout << "Fun Get_Buttons_State() param invalid.";
            return;
        }
        Chassis_Main_Switch_Board_Control->m_GPIO.GPIOx_Read(Chassis_Main_Switch_Board, GPIOD, GPIO_PIN_11, 1000);
        collision_message.cb[0] = Chassis_Main_Switch_Board_Control->m_GPIO.Get_GPIOx_Value(GPIOD, GPIO_PIN_11);
        publisher_Collision->publish(collision_message);
    }

    void Temp_Get_callback() {
        auto send_message = devices_pkg::msg::WBotMotorTem();

        left_shoulder_pitch_N11_Ctl -> Get_Motor_Temp_Data(left_shoulder_pitch_N11,  &send_message.left_shoulder_pitch.mtemp);
        left_shoulder_roll_N12_Ctl  -> Get_Motor_Temp_Data(left_shoulder_roll_N12,   &send_message.left_shoulder_roll.mtemp );
        left_shoulder_yaw_N13_Ctl   -> Get_Motor_Temp_Data(left_shoulder_yaw_N13,    &send_message.left_shoulder_yaw.mtemp  );
        left_elbow_N14_Ctl          -> Get_Motor_Temp_Data(left_elbow_N14,           &send_message.left_elbow.mtemp         );
        left_wrist_yaw_N15_Ctl      -> Get_Motor_Temp_Data(left_wrist_yaw_N15,       &send_message.left_wrist_yaw.mtemp     );
        left_wrist_pitch_N16_Ctl    -> Get_Motor_Temp_Data(left_wrist_pitch_N16,     &send_message.left_wrist_pitch.mtemp   );
        left_wrist_roll_N17_Ctl     -> Get_Motor_Temp_Data(left_wrist_roll_N17,      &send_message.left_wrist_roll.mtemp    );

        right_shoulder_pitch_N21_Ctl -> Get_Motor_Temp_Data(right_shoulder_pitch_N21, &send_message.right_shoulder_pitch.mtemp);
        right_shoulder_roll_N22_Ctl  -> Get_Motor_Temp_Data(right_shoulder_roll_N22,  &send_message.right_shoulder_roll.mtemp );
        right_shoulder_yaw_N23_Ctl   -> Get_Motor_Temp_Data(right_shoulder_yaw_N23,   &send_message.right_shoulder_yaw.mtemp  );
        right_elbow_N24_Ctl          -> Get_Motor_Temp_Data(right_elbow_N24,          &send_message.right_elbow.mtemp         );
        right_wrist_yaw_N25_Ctl      -> Get_Motor_Temp_Data(right_wrist_yaw_N25,      &send_message.right_wrist_yaw.mtemp     );
        right_wrist_pitch_N26_Ctl    -> Get_Motor_Temp_Data(right_wrist_pitch_N26,    &send_message.right_wrist_pitch.mtemp   );
        right_wrist_roll_N27_Ctl     -> Get_Motor_Temp_Data(right_wrist_roll_N27,     &send_message.right_wrist_roll.mtemp    );

        // TaiHu_Device_T15->Get_Motor_Temp(TaiHu_Device_15, &send_message.head_pitch.mtemp);
        // TaiHu_Device_T16->Get_Motor_Temp(TaiHu_Device_16, &send_message.head_yaw.mtemp);

        Lower_Limbs_Motor_Ankel_N31_Ctl      -> Get_Motor_Temp_Data(Lower_Limbs_Motor_Ankel_N31,        &send_message.ankle.mtemp     );
        Lower_Limbs_Motor_Knee_N32_Ctl       -> Get_Motor_Temp_Data(Lower_Limbs_Motor_Knee_N32,         &send_message.knee.mtemp      );
        Lower_Limbs_Motor_Hip_N33_Ctl        -> Get_Motor_Temp_Data(Lower_Limbs_Motor_Hip_N33,          &send_message.hip.mtemp       );
        Lower_Limbs_Motor_Waist_Roll_N34_Ctl -> Get_Motor_Temp_Data(Lower_Limbs_Motor_Waist_Roll_N34,   &send_message.waist_roll.mtemp);
        Lower_Limbs_Motor_Waist_Yaw_N35_Ctl  -> Get_Motor_Temp_Data(Lower_Limbs_Motor_Waist_Yaw_N35,    &send_message.waist_yaw.mtemp );

        publisher_MotorTem->publish(send_message);
    }

    void led_topic_callback(const devices_pkg::msg::WBotLED::SharedPtr msg) const {
        auto get_message = msg;

        for (int i = 0; i < 340; i++) {
            RGB_Datas[1].R_data[i] = get_message->led_upper.arr_r[i];
            RGB_Datas[1].G_data[i] = get_message->led_upper.arr_g[i];
            RGB_Datas[1].B_data[i] = get_message->led_upper.arr_b[i];

            RGB_Datas[0].R_data[i] = get_message->led_lower.arr_r[i];
            RGB_Datas[0].G_data[i] = get_message->led_lower.arr_g[i];
            RGB_Datas[0].B_data[i] = get_message->led_lower.arr_b[i];
        }
        Led_Device_Chassis_Ptr->Led_Set_Every_Color(Led_Chassis_Devices, 1, 1, 20, &RGB_Datas[0]);
        Led_Device_Shoulder_Ptr->Led_Set_Every_Color(Led_Shoulder_Devices, 1, 1, 11, &RGB_Datas[1]);
    }

    void Motor_Stop_topic_callback(const std_msgs::msg::String::SharedPtr msg) const
    {
        RCLCPP_INFO_STREAM(this->get_logger(), "Motor_Stop_topic_callback: " << msg->data);
        if(!msg->data.empty()){

            Lower_Limbs_Motor_Waist_Yaw_N35_Ctl->Phu_Motor_Status_Ctrl(Lower_Limbs_Motor_Waist_Yaw_N35, 0x04);
            Lower_Limbs_Motor_Waist_Yaw_N35_Ctl->Phu_Motor_Status_Ctrl(Lower_Limbs_Motor_Waist_Yaw_N35, 0x02);

            Lower_Limbs_Motor_Waist_Roll_N34_Ctl->Phu_Motor_Status_Ctrl(Lower_Limbs_Motor_Waist_Roll_N34, 0x04);
            Lower_Limbs_Motor_Waist_Roll_N34_Ctl->Phu_Motor_Status_Ctrl(Lower_Limbs_Motor_Waist_Roll_N34, 0x02);

            Lower_Limbs_Motor_Hip_N33_Ctl->Phu_Motor_Status_Ctrl(Lower_Limbs_Motor_Hip_N33, 0x04);
            Lower_Limbs_Motor_Hip_N33_Ctl->Phu_Motor_Status_Ctrl(Lower_Limbs_Motor_Hip_N33, 0x02);

            Lower_Limbs_Motor_Knee_N32_Ctl->Phu_Motor_Status_Ctrl(Lower_Limbs_Motor_Knee_N32, 0x04);
            Lower_Limbs_Motor_Knee_N32_Ctl->Phu_Motor_Status_Ctrl(Lower_Limbs_Motor_Knee_N32, 0x02);

            Lower_Limbs_Motor_Ankel_N31_Ctl->Phu_Motor_Status_Ctrl(Lower_Limbs_Motor_Ankel_N31, 0x04);
            Lower_Limbs_Motor_Ankel_N31_Ctl->Phu_Motor_Status_Ctrl(Lower_Limbs_Motor_Ankel_N31, 0x02);
        }
    }

    static int Wbot_Error_callback(shared_ptr<Device_class> Device, std::string Info, Err_Level Level)
    {

        if (self_ptr)
        {
            std_msgs::msg::String error_message;
            shared_ptr<std::map<std::string, Err_Level>> history_error_Map = Device->Error_Map;

            error_message.data = "Error: " + Info + ", Level: " + to_string(Level) + "\n";
            error_message.data += "history:\n";
            for (const auto& error_entry : *history_error_Map)
            {
                error_message.data += "Error: " + error_entry.first + ", Level: " + to_string(error_entry.second) + "\n";
            }
            self_ptr->publisher_Error->publish(error_message);
        }
        return 0;
    }

    rclcpp::Publisher<devices_pkg::msg::WBotMotor>::SharedPtr publisher_Motor;
    rclcpp::Publisher<devices_pkg::msg::WBotIMU>::SharedPtr publisher_IMU;
    rclcpp::Publisher<devices_pkg::msg::WBotBattery>::SharedPtr publisher_Battery;
    rclcpp::Publisher<devices_pkg::msg::WBotCollision>::SharedPtr publisher_Collision;
    rclcpp::Publisher<devices_pkg::msg::WBotMotorTem>::SharedPtr publisher_MotorTem;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_Error;
    rclcpp::Subscription<devices_pkg::msg::WBotMotor>::SharedPtr subscription_Motor;
    rclcpp::Subscription<devices_pkg::msg::WBotLED>::SharedPtr subscription_LED;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_Motor_Stop;
    rclcpp::TimerBase::SharedPtr timer_imu;
    rclcpp::TimerBase::SharedPtr timer_Motor;
    rclcpp::TimerBase::SharedPtr timer_Battery;
    rclcpp::TimerBase::SharedPtr timer_Collision_Bar;
    rclcpp::TimerBase::SharedPtr timer_Temp_Get;
    
};

// 静态指针初始化
W_Bot_EUF_Node* W_Bot_EUF_Node::self_ptr = nullptr;

#endif  // W_BOT_NODE_H_
