#ifndef Y_HAND_NODE_H_
#define Y_HAND_NODE_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "UDP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Custom_TOP.hpp"
#include "Motor_TOP.hpp"
#include "Switch_Board.hpp"
#include <filesystem>
#include <iostream>
#include "devices_pkg/msg/y_hand_msg.hpp"

using namespace std;

//声明外部变量
extern Robot_Hardware *Test_Robot;
extern shared_ptr<Device_class> Motor1_Device;
extern shared_ptr<Device_class> Motor2_Device;
extern shared_ptr<Device_class> Motor3_Device;
extern shared_ptr<Device_class> Motor4_Device;
extern shared_ptr<Device_class> Motor5_Device;
extern shared_ptr<Device_class> Motor6_Device;
extern shared_ptr<Device_class> Motor7_Device;
extern shared_ptr<Device_class> Motor8_Device;
extern shared_ptr<Device_class> Motor9_Device;
extern shared_ptr<Device_class> Motor10_Device;
extern shared_ptr<Device_class> Motor11_Device;
extern shared_ptr<Device_class> Motor12_Device;
extern shared_ptr<Device_class> Motor13_Device;
extern shared_ptr<Device_class> Motor14_Device;

extern Motor *Motor1_Control;
extern Motor *Motor2_Control;
extern Motor *Motor3_Control;
extern Motor *Motor4_Control;
extern Motor *Motor5_Control;
extern Motor *Motor6_Control;
extern Motor *Motor7_Control;
extern Motor *Motor8_Control;
extern Motor *Motor9_Control;
extern Motor *Motor10_Control;
extern Motor *Motor11_Control;
extern Motor *Motor12_Control;
extern Motor *Motor13_Control;
extern Motor *Motor14_Control;

//声明外部函数
int hardware_init(const string& ADDR, const string& Config);
int hand_pretighten(float F, uint32_t time_ms);

const int MOTOR_NUM = 14;
class Y_Hand_M1_Node : public rclcpp::Node
{
public:
     Y_Hand_M1_Node(
        const std::string& node_name)
     : Node(node_name)
     {
     }

     int create_objects(
        const std::string& pub_topic,
        const std::string& sub_topic,
        const std::string& dev_config) {

        if(hardware_init("src/devices_pkg/sdk/config/YAML/Y_Hand_M1/out/TOP.yaml", dev_config) != 0)
        {
            cout << "Hardware initialization failed" << endl;
            return -1;
        }
        publisher_ = this->create_publisher<devices_pkg::msg::YHandMsg>(pub_topic, 10);
        subscription_ = this->create_subscription<devices_pkg::msg::YHandMsg>(sub_topic, 10, \
            std::bind(&Y_Hand_M1_Node::topic_callback, this, std::placeholders::_1));

        return 0;
     }



private:
    void topic_callback(const devices_pkg::msg::YHandMsg::SharedPtr msg) const
    {
        if (msg->pretightenforce.flag)
        {
            hand_pretighten(msg->pretightenforce.torque, msg->pretightenforce.ms);
        }
        else
        {
            Motor1_Control->Send_MIT_PD_Control_Data(Motor1_Device, msg->motors[0].pos, msg->motors[0].vel, msg->motors[0].tor, msg->motors[0].kp, msg->motors[0].kd);
            Motor2_Control->Send_MIT_PD_Control_Data(Motor2_Device, msg->motors[1].pos, msg->motors[1].vel, msg->motors[1].tor, msg->motors[1].kp, msg->motors[1].kd);
            Motor3_Control->Send_MIT_PD_Control_Data(Motor3_Device, msg->motors[2].pos, msg->motors[2].vel, msg->motors[2].tor, msg->motors[2].kp, msg->motors[2].kd);
            Motor4_Control->Send_MIT_PD_Control_Data(Motor4_Device, msg->motors[3].pos, msg->motors[3].vel, msg->motors[3].tor, msg->motors[3].kp, msg->motors[3].kd);
            Motor5_Control->Send_MIT_PD_Control_Data(Motor5_Device, msg->motors[4].pos, msg->motors[4].vel, msg->motors[4].tor, msg->motors[4].kp, msg->motors[4].kd);
            Motor6_Control->Send_MIT_PD_Control_Data(Motor6_Device, msg->motors[5].pos, msg->motors[5].vel, msg->motors[5].tor, msg->motors[5].kp, msg->motors[5].kd);
            Motor7_Control->Send_MIT_PD_Control_Data(Motor7_Device, msg->motors[6].pos, msg->motors[6].vel, msg->motors[6].tor, msg->motors[6].kp, msg->motors[6].kd);
            Motor8_Control->Send_MIT_PD_Control_Data(Motor8_Device, msg->motors[7].pos, msg->motors[7].vel, msg->motors[7].tor, msg->motors[7].kp, msg->motors[7].kd);
            Motor9_Control->Send_MIT_PD_Control_Data(Motor9_Device, msg->motors[8].pos, msg->motors[8].vel, msg->motors[8].tor, msg->motors[8].kp, msg->motors[8].kd);
            Motor10_Control->Send_MIT_PD_Control_Data(Motor10_Device, msg->motors[9].pos, msg->motors[9].vel, msg->motors[9].tor, msg->motors[9].kp, msg->motors[9].kd);
            Motor11_Control->Send_MIT_PD_Control_Data(Motor11_Device, msg->motors[10].pos, msg->motors[10].vel, msg->motors[10].tor, msg->motors[10].kp, msg->motors[10].kd);
            Motor12_Control->Send_MIT_PD_Control_Data(Motor12_Device, msg->motors[11].pos, msg->motors[11].vel, msg->motors[11].tor, msg->motors[11].kp, msg->motors[11].kd);
            Motor13_Control->Send_MIT_PD_Control_Data(Motor13_Device, msg->motors[12].pos, msg->motors[12].vel, msg->motors[12].tor, msg->motors[12].kp, msg->motors[12].kd);
            Motor14_Control->Send_MIT_PD_Control_Data(Motor14_Device, msg->motors[13].pos, msg->motors[13].vel, msg->motors[13].tor, msg->motors[13].kp, msg->motors[13].kd);
          
            // 发送控制缓冲区数据到硬件
            Test_Robot->Send_Buff_Data();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        Y_Hand_M1_Node* mu_this = const_cast<Y_Hand_M1_Node*>(this);

        Motor1_Control->Get_Motor_FB_Data(  Motor1_Device,   &mu_this->sendMes.motors[0 ].pos,  &mu_this->sendMes.motors[0 ].vel,  &mu_this->sendMes.motors[0 ].tor, &mu_this->sendMes.motors[0 ].temp[0], &mu_this->sendMes.motors[0 ].error);
        Motor2_Control->Get_Motor_FB_Data(  Motor2_Device,   &mu_this->sendMes.motors[1 ].pos,  &mu_this->sendMes.motors[1 ].vel,  &mu_this->sendMes.motors[1 ].tor, &mu_this->sendMes.motors[1 ].temp[0], &mu_this->sendMes.motors[1 ].error);
        Motor3_Control->Get_Motor_FB_Data(  Motor3_Device,   &mu_this->sendMes.motors[2 ].pos,  &mu_this->sendMes.motors[2 ].vel,  &mu_this->sendMes.motors[2 ].tor, &mu_this->sendMes.motors[2 ].temp[0], &mu_this->sendMes.motors[2 ].error);
        Motor4_Control->Get_Motor_FB_Data(  Motor4_Device,   &mu_this->sendMes.motors[3 ].pos,  &mu_this->sendMes.motors[3 ].vel,  &mu_this->sendMes.motors[3 ].tor, &mu_this->sendMes.motors[3 ].temp[0], &mu_this->sendMes.motors[3 ].error);
        Motor5_Control->Get_Motor_FB_Data(  Motor5_Device,   &mu_this->sendMes.motors[4 ].pos,  &mu_this->sendMes.motors[4 ].vel,  &mu_this->sendMes.motors[4 ].tor, &mu_this->sendMes.motors[4 ].temp[0], &mu_this->sendMes.motors[4 ].error);
        Motor6_Control->Get_Motor_FB_Data(  Motor6_Device,   &mu_this->sendMes.motors[5 ].pos,  &mu_this->sendMes.motors[5 ].vel,  &mu_this->sendMes.motors[5 ].tor, &mu_this->sendMes.motors[5 ].temp[0], &mu_this->sendMes.motors[5 ].error);
        Motor7_Control->Get_Motor_FB_Data(  Motor7_Device,   &mu_this->sendMes.motors[6 ].pos,  &mu_this->sendMes.motors[6 ].vel,  &mu_this->sendMes.motors[6 ].tor, &mu_this->sendMes.motors[6 ].temp[0], &mu_this->sendMes.motors[6 ].error);
        Motor8_Control->Get_Motor_FB_Data(  Motor8_Device,   &mu_this->sendMes.motors[7 ].pos,  &mu_this->sendMes.motors[7 ].vel,  &mu_this->sendMes.motors[7 ].tor, &mu_this->sendMes.motors[7 ].temp[0], &mu_this->sendMes.motors[7 ].error);
        Motor9_Control->Get_Motor_FB_Data(  Motor9_Device,   &mu_this->sendMes.motors[8 ].pos,  &mu_this->sendMes.motors[8 ].vel,  &mu_this->sendMes.motors[8 ].tor, &mu_this->sendMes.motors[8 ].temp[0], &mu_this->sendMes.motors[8 ].error);
        Motor10_Control->Get_Motor_FB_Data( Motor10_Device,  &mu_this->sendMes.motors[9 ].pos,  &mu_this->sendMes.motors[9 ].vel,  &mu_this->sendMes.motors[9 ].tor, &mu_this->sendMes.motors[9 ].temp[0], &mu_this->sendMes.motors[9 ].error);
        Motor11_Control->Get_Motor_FB_Data( Motor11_Device,  &mu_this->sendMes.motors[10].pos,  &mu_this->sendMes.motors[10].vel,  &mu_this->sendMes.motors[10].tor, &mu_this->sendMes.motors[10].temp[0], &mu_this->sendMes.motors[10].error);
        Motor12_Control->Get_Motor_FB_Data( Motor12_Device,  &mu_this->sendMes.motors[11].pos,  &mu_this->sendMes.motors[11].vel,  &mu_this->sendMes.motors[11].tor, &mu_this->sendMes.motors[11].temp[0], &mu_this->sendMes.motors[11].error);
        Motor13_Control->Get_Motor_FB_Data( Motor13_Device,  &mu_this->sendMes.motors[12].pos,  &mu_this->sendMes.motors[12].vel,  &mu_this->sendMes.motors[12].tor, &mu_this->sendMes.motors[12].temp[0], &mu_this->sendMes.motors[12].error);
        Motor14_Control->Get_Motor_FB_Data( Motor14_Device,  &mu_this->sendMes.motors[13].pos,  &mu_this->sendMes.motors[13].vel,  &mu_this->sendMes.motors[13].tor, &mu_this->sendMes.motors[13].temp[0], &mu_this->sendMes.motors[13].error);
        
        publisher_->publish(sendMes); 

    }


    rclcpp::Subscription<devices_pkg::msg::YHandMsg>::SharedPtr subscription_;
    rclcpp::Publisher<devices_pkg::msg::YHandMsg>::SharedPtr publisher_;
    devices_pkg::msg::YHandMsg sendMes;


};

#endif // Y_HAND_NODE_H_
