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

//重复定义结构体
typedef struct X_hand_FB {
    float P;
    float V;
    float F;
    float temp[2];
    u16 error;
} X_hand_FB;

typedef struct X_hand_Send_Data {
    float P;
    float V;
    float F;
    float KP;
    float KD;
} X_hand_Send_Data;

//声明外部变量
extern Robot_Hardware *X_Hand;
extern X_hand_Send_Data Send_Datas[6];
extern X_hand_FB FB_Datas[6];
extern map<u8, vector<u16>> g_sensor_data;

//声明外部函数
int hardware_init(string ADDR);
void Get_FB(void);
void Send(void);

const int MOTOR_NUM = 6;

class X_Hand_Node : public rclcpp::Node
{
public:
     X_Hand_Node()
     : Node("x_hand_node")
     {
        hardware_init("src/devices_pkg/src/sdk/config/YAML/X_Hand/out/TOP.yaml");
        publisher_ = this->create_publisher<devices_pkg::msg::XHandMsg>("x_hand_publisher", 10);
        subscription_ = this->create_subscription<devices_pkg::msg::XHandMsg>("x_hand_subscriber", 10, \
            std::bind(&X_Hand_Node::topic_callback, this, std::placeholders::_1));
     }



private:
    void topic_callback(const devices_pkg::msg::XHandMsg::SharedPtr msg) const
    {
        for(size_t i = 0; i < MOTOR_NUM; i++)
        {
            Send_Datas[i].P = msg->motors[i].pos;
            Send_Datas[i].V = msg->motors[i].vel;
            Send_Datas[i].F = msg->motors[i].tor;
            Send_Datas[i].KP = msg->motors[i].kp;
            Send_Datas[i].KD = msg->motors[i].kd;

            
        }
        Send();

        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        X_Hand_Node* mutable_this = const_cast<X_Hand_Node*>(this);

        Get_FB();

        if (g_sensor_data.size() == MOTOR_NUM)
        {
            for (size_t i = 0; i < MOTOR_NUM; i++)
            {
                mutable_this->sendMes.motors[i].pos = FB_Datas[i].P;
                mutable_this->sendMes.motors[i].vel = FB_Datas[i].V;
                mutable_this->sendMes.motors[i].tor = FB_Datas[i].F;
                mutable_this->sendMes.motors[i].temp[0] = FB_Datas[i].temp[0];
                mutable_this->sendMes.motors[i].temp[1] = FB_Datas[i].temp[1];
                mutable_this->sendMes.motors[i].error = FB_Datas[i].error;

                mutable_this->sendMes.pressure[i].finger = g_sensor_data.at((u8)i + 1);
            }
        }

        publisher_->publish(sendMes);

        
        

    }


    rclcpp::Subscription<devices_pkg::msg::XHandMsg>::SharedPtr subscription_;
    rclcpp::Publisher<devices_pkg::msg::XHandMsg>::SharedPtr publisher_;
    devices_pkg::msg::XHandMsg sendMes;


};