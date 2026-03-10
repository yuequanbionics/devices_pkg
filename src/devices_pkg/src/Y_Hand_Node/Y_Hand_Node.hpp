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
extern shared_ptr<Device_class> Motor15_Device;
extern shared_ptr<Device_class> Motor16_Device;
extern shared_ptr<Device_class> Motor17_Device;
extern shared_ptr<Device_class> Motor18_Device;
extern shared_ptr<Device_class> Motor19_Device;
extern shared_ptr<Device_class> Motor20_Device;
extern shared_ptr<Device_class> Motor21_Device;
extern shared_ptr<Device_class> Motor22_Device;
extern shared_ptr<Device_class> Motor23_Device;
extern shared_ptr<Device_class> Motor24_Device;
extern shared_ptr<Device_class> Motor25_Device;
extern shared_ptr<Device_class> Motor26_Device;
extern shared_ptr<Device_class> Motor27_Device;
extern shared_ptr<Device_class> Motor28_Device;

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
extern Motor *Motor15_Control;
extern Motor *Motor16_Control;
extern Motor *Motor17_Control;
extern Motor *Motor18_Control;
extern Motor *Motor19_Control;
extern Motor *Motor20_Control;
extern Motor *Motor21_Control;
extern Motor *Motor22_Control;
extern Motor *Motor23_Control;
extern Motor *Motor24_Control;
extern Motor *Motor25_Control;
extern Motor *Motor26_Control;
extern Motor *Motor27_Control;
extern Motor *Motor28_Control;

//声明外部函数
int hardware_init(string ADDR, string Config);
int hand_pretighten(float F, uint32_t time_ms);

const int MOTOR_NUM = 28;
class Y_Hand_Node : public rclcpp::Node
{
public:
     Y_Hand_Node(
        const std::string& node_name,
        const std::string& pub_topic,
        const std::string& sub_topic,
        const std::string& dev_config
     )
     : Node(node_name)
     {
        hardware_init("src/devices_pkg/sdk/config/YAML/Y_Hand/out/TOP.yaml", dev_config);

        publisher_ = this->create_publisher<devices_pkg::msg::YHandMsg>(pub_topic, 10);
        
        subscription_ = this->create_subscription<devices_pkg::msg::YHandMsg>(sub_topic, 10, \
            std::bind(&Y_Hand_Node::topic_callback, this, std::placeholders::_1));
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
            Motor10_Control->Send_MIT_PD_Control_Data(Motor10_Device, msg->motors[9].pos, msg->motors[9].vel, msg->motors[5].tor, msg->motors[9].kp, msg->motors[9].kd);
            Motor11_Control->Send_MIT_PD_Control_Data(Motor11_Device, msg->motors[10].pos, msg->motors[10].vel, msg->motors[5].tor, msg->motors[10].kp, msg->motors[10].kd);
            Motor12_Control->Send_MIT_PD_Control_Data(Motor12_Device, msg->motors[11].pos, msg->motors[11].vel, msg->motors[11].tor, msg->motors[11].kp, msg->motors[11].kd);
            Motor13_Control->Send_MIT_PD_Control_Data(Motor13_Device, msg->motors[12].pos, msg->motors[12].vel, msg->motors[12].tor, msg->motors[12].kp, msg->motors[12].kd);
            Motor14_Control->Send_MIT_PD_Control_Data(Motor14_Device, msg->motors[13].pos, msg->motors[13].vel, msg->motors[13].tor, msg->motors[13].kp, msg->motors[13].kd);
            Motor15_Control->Send_MIT_PD_Control_Data(Motor15_Device, msg->motors[14].pos, msg->motors[14].vel, msg->motors[14].tor, msg->motors[14].kp, msg->motors[14].kd);
            Motor16_Control->Send_MIT_PD_Control_Data(Motor16_Device, msg->motors[15].pos, msg->motors[15].vel, msg->motors[15].tor, msg->motors[15].kp, msg->motors[15].kd);
            Motor17_Control->Send_MIT_PD_Control_Data(Motor17_Device, msg->motors[16].pos, msg->motors[16].vel, msg->motors[16].tor, msg->motors[16].kp, msg->motors[16].kd);
            Motor18_Control->Send_MIT_PD_Control_Data(Motor18_Device, msg->motors[17].pos, msg->motors[17].vel, msg->motors[17].tor, msg->motors[17].kp, msg->motors[17].kd);
            Motor19_Control->Send_MIT_PD_Control_Data(Motor19_Device, msg->motors[18].pos, msg->motors[18].vel, msg->motors[18].tor, msg->motors[18].kp, msg->motors[18].kd);
            Motor20_Control->Send_MIT_PD_Control_Data(Motor20_Device, msg->motors[19].pos, msg->motors[19].vel, msg->motors[19].tor, msg->motors[19].kp, msg->motors[19].kd);
            Motor21_Control->Send_MIT_PD_Control_Data(Motor21_Device, msg->motors[20].pos, msg->motors[20].vel, msg->motors[20].tor, msg->motors[20].kp, msg->motors[20].kd);
            Motor22_Control->Send_MIT_PD_Control_Data(Motor22_Device, msg->motors[21].pos, msg->motors[21].vel, msg->motors[21].tor, msg->motors[21].kp, msg->motors[21].kd);
            Motor23_Control->Send_MIT_PD_Control_Data(Motor23_Device, msg->motors[22].pos, msg->motors[22].vel, msg->motors[22].tor, msg->motors[22].kp, msg->motors[22].kd);
            Motor24_Control->Send_MIT_PD_Control_Data(Motor24_Device, msg->motors[23].pos, msg->motors[23].vel, msg->motors[23].tor, msg->motors[23].kp, msg->motors[23].kd);
            Motor25_Control->Send_MIT_PD_Control_Data(Motor25_Device, msg->motors[24].pos, msg->motors[24].vel, msg->motors[24].tor, msg->motors[24].kp, msg->motors[24].kd);
            Motor26_Control->Send_MIT_PD_Control_Data(Motor26_Device, msg->motors[25].pos, msg->motors[25].vel, msg->motors[25].tor, msg->motors[25].kp, msg->motors[25].kd);
            Motor27_Control->Send_MIT_PD_Control_Data(Motor27_Device, msg->motors[26].pos, msg->motors[26].vel, msg->motors[26].tor, msg->motors[26].kp, msg->motors[26].kd);
            Motor28_Control->Send_MIT_PD_Control_Data(Motor28_Device, msg->motors[27].pos, msg->motors[27].vel, msg->motors[27].tor, msg->motors[27].kp, msg->motors[27].kd);

            // 发送控制缓冲区数据到硬件
            Test_Robot->Send_Buff_Data();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        Y_Hand_Node* mu_this = const_cast<Y_Hand_Node*>(this);

        Motor1_Control->Get_Motor_FB_Data( Motor1_Device,  &mu_this->sendMes.motors[0].pos,  &mu_this->sendMes.motors[0].vel,  &mu_this->sendMes.motors[0].tor);
        Motor2_Control->Get_Motor_FB_Data( Motor2_Device,  &mu_this->sendMes.motors[1].pos,  &mu_this->sendMes.motors[1].vel,  &mu_this->sendMes.motors[1].tor);
        Motor3_Control->Get_Motor_FB_Data( Motor3_Device,  &mu_this->sendMes.motors[2].pos,  &mu_this->sendMes.motors[2].vel,  &mu_this->sendMes.motors[2].tor);
        Motor4_Control->Get_Motor_FB_Data( Motor4_Device,  &mu_this->sendMes.motors[3].pos,  &mu_this->sendMes.motors[3].vel,  &mu_this->sendMes.motors[3].tor);
        Motor5_Control->Get_Motor_FB_Data( Motor5_Device,  &mu_this->sendMes.motors[4].pos,  &mu_this->sendMes.motors[4].vel,  &mu_this->sendMes.motors[4].tor);
        Motor6_Control->Get_Motor_FB_Data( Motor6_Device,  &mu_this->sendMes.motors[5].pos,  &mu_this->sendMes.motors[5].vel,  &mu_this->sendMes.motors[5].tor);
        Motor7_Control->Get_Motor_FB_Data( Motor7_Device,  &mu_this->sendMes.motors[6].pos,  &mu_this->sendMes.motors[6].vel,  &mu_this->sendMes.motors[6].tor);
        Motor8_Control->Get_Motor_FB_Data( Motor8_Device,  &mu_this->sendMes.motors[7].pos,  &mu_this->sendMes.motors[7].vel,  &mu_this->sendMes.motors[7].tor);
        Motor9_Control->Get_Motor_FB_Data( Motor9_Device,  &mu_this->sendMes.motors[8].pos,  &mu_this->sendMes.motors[8].vel,  &mu_this->sendMes.motors[8].tor);
        Motor10_Control->Get_Motor_FB_Data( Motor10_Device,  &mu_this->sendMes.motors[9].pos,  &mu_this->sendMes.motors[9].vel,  &mu_this->sendMes.motors[9].tor);
        Motor11_Control->Get_Motor_FB_Data( Motor11_Device,  &mu_this->sendMes.motors[10].pos,  &mu_this->sendMes.motors[10].vel,  &mu_this->sendMes.motors[10].tor);
        Motor12_Control->Get_Motor_FB_Data( Motor12_Device,  &mu_this->sendMes.motors[11].pos,  &mu_this->sendMes.motors[11].vel,  &mu_this->sendMes.motors[11].tor);
        Motor13_Control->Get_Motor_FB_Data( Motor13_Device,  &mu_this->sendMes.motors[12].pos,  &mu_this->sendMes.motors[12].vel,  &mu_this->sendMes.motors[12].tor);
        Motor14_Control->Get_Motor_FB_Data( Motor14_Device,  &mu_this->sendMes.motors[13].pos,  &mu_this->sendMes.motors[13].vel,  &mu_this->sendMes.motors[13].tor);
        Motor15_Control->Get_Motor_FB_Data( Motor15_Device,  &mu_this->sendMes.motors[14].pos,  &mu_this->sendMes.motors[14].vel,  &mu_this->sendMes.motors[14].tor);
        Motor16_Control->Get_Motor_FB_Data( Motor16_Device,  &mu_this->sendMes.motors[15].pos,  &mu_this->sendMes.motors[15].vel,  &mu_this->sendMes.motors[15].tor);
        Motor17_Control->Get_Motor_FB_Data( Motor17_Device,  &mu_this->sendMes.motors[16].pos,  &mu_this->sendMes.motors[16].vel,  &mu_this->sendMes.motors[16].tor);
        Motor18_Control->Get_Motor_FB_Data( Motor18_Device,  &mu_this->sendMes.motors[17].pos,  &mu_this->sendMes.motors[17].vel,  &mu_this->sendMes.motors[17].tor);
        Motor19_Control->Get_Motor_FB_Data( Motor19_Device,  &mu_this->sendMes.motors[18].pos,  &mu_this->sendMes.motors[18].vel,  &mu_this->sendMes.motors[18].tor);
        Motor20_Control->Get_Motor_FB_Data( Motor20_Device,  &mu_this->sendMes.motors[19].pos,  &mu_this->sendMes.motors[19].vel,  &mu_this->sendMes.motors[19].tor);
        Motor21_Control->Get_Motor_FB_Data( Motor21_Device,  &mu_this->sendMes.motors[20].pos,  &mu_this->sendMes.motors[20].vel,  &mu_this->sendMes.motors[20].tor);
        Motor22_Control->Get_Motor_FB_Data( Motor22_Device,  &mu_this->sendMes.motors[21].pos,  &mu_this->sendMes.motors[21].vel,  &mu_this->sendMes.motors[21].tor);
        Motor23_Control->Get_Motor_FB_Data( Motor23_Device,  &mu_this->sendMes.motors[22].pos,  &mu_this->sendMes.motors[22].vel,  &mu_this->sendMes.motors[22].tor);
        Motor24_Control->Get_Motor_FB_Data( Motor24_Device,  &mu_this->sendMes.motors[23].pos,  &mu_this->sendMes.motors[23].vel,  &mu_this->sendMes.motors[23].tor);
        Motor25_Control->Get_Motor_FB_Data( Motor25_Device,  &mu_this->sendMes.motors[24].pos,  &mu_this->sendMes.motors[24].vel,  &mu_this->sendMes.motors[24].tor);
        Motor26_Control->Get_Motor_FB_Data( Motor26_Device,  &mu_this->sendMes.motors[25].pos,  &mu_this->sendMes.motors[25].vel,  &mu_this->sendMes.motors[25].tor);
        Motor27_Control->Get_Motor_FB_Data( Motor27_Device,  &mu_this->sendMes.motors[26].pos,  &mu_this->sendMes.motors[26].vel,  &mu_this->sendMes.motors[26].tor);
        Motor28_Control->Get_Motor_FB_Data( Motor28_Device,  &mu_this->sendMes.motors[27].pos,  &mu_this->sendMes.motors[27].vel,  &mu_this->sendMes.motors[27].tor);


        publisher_->publish(sendMes); 

    }


    rclcpp::Subscription<devices_pkg::msg::YHandMsg>::SharedPtr subscription_;
    rclcpp::Publisher<devices_pkg::msg::YHandMsg>::SharedPtr publisher_;
    devices_pkg::msg::YHandMsg sendMes;


};

#endif // Y_HAND_NODE_H_
