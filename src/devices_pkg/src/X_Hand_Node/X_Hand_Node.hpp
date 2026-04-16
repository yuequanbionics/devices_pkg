#ifndef X_HAND_NODE_H_
#define X_HAND_NODE_H_

#include <filesystem>
#include <iostream>

#include "Custom_TOP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Hw_Pressure_Sensor.hpp"
#include "Switch_Board.hpp"
#include "UDP.hpp"
#include "devices_pkg/msg/x_hand_msg.hpp"
#include "devices_pkg/msg/x_hand_sensor_msg.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std;

// 重复定义结构体
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

// 声明外部变量
extern Robot_Hardware* X_Hand;
extern X_hand_Send_Data Send_Datas[6];
extern X_hand_FB FB_Datas[6];
extern Hw_Pressure_Sensor* Tactile_Sensor_Control;  // 触摸传感器

// 声明外部函数
int hardware_init(const string& ADDR, const string& Config);
void Get_FB(void);
void Send(void);

const int MOTOR_NUM = 6;

class X_Hand_Node : public rclcpp::Node {
   public:
    X_Hand_Node(
        const std::string& node_name)
        : Node(node_name) {
        
    }

     int create_objects(
        const std::string& pub_topic,
        const std::string& sub_topic,
        const std::string& dev_type,
        const std::string& dev_config) {

        string yaml_path = "src/devices_pkg/sdk/config/YAML/X_Hand/out/" + dev_type + "/TOP.yaml";
        if(hardware_init(yaml_path, dev_config) != 0)
        {
            cout << "Hardware initialization failed" << endl;
            return -1;
        }
        publisher_ = this->create_publisher<devices_pkg::msg::XHandMsg>(pub_topic, 10);
        sensor_publisher_ = this->create_publisher<devices_pkg::msg::XHandSensorMsg>("sensor_data", 10);
        subscription_ = this->create_subscription<devices_pkg::msg::XHandMsg>(sub_topic, 10,
                                                                              std::bind(&X_Hand_Node::topic_callback, this, std::placeholders::_1));
        timer_Sensor = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&X_Hand_Node::sensor_timer_callback, this));

        return 0;
     }


   private:
    void sensor_timer_callback() {
        auto sensor_message = devices_pkg::msg::XHandSensorMsg();
        static const vector<uint8_t> sensor_ids = {
            HW_THUMB_ID,          // 拇指
            HW_INDEX_FINGER_ID,   // 食指
            HW_MIDDLE_FINGER_ID,  // 中指
            HW_RING_FINGER_ID,    // 无名指
            HW_LITTLE_FINGER_ID,  // 小指
            HW_PALM_CENTER_ID     // 手掌中心
        };

        for (uint8_t sensor_id : sensor_ids) {
            vector<uint16_t> raw_sensor_data = Tactile_Sensor_Control->Get_Stored_Sensor_Data(sensor_id);

            sensor_message.pressure[sensor_id - 1].finger = raw_sensor_data;
        }
        // 发布采集完整的传感器消息
        sensor_publisher_->publish(sensor_message);
    }

    void topic_callback(const devices_pkg::msg::XHandMsg::SharedPtr msg) const {
        for (size_t i = 0; i < MOTOR_NUM; i++) {
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

        for (size_t i = 0; i < MOTOR_NUM; i++) {
            mutable_this->sendMes.motors[i].pos = FB_Datas[i].P;
            mutable_this->sendMes.motors[i].vel = FB_Datas[i].V;
            mutable_this->sendMes.motors[i].tor = FB_Datas[i].F;
            mutable_this->sendMes.motors[i].temp[0] = FB_Datas[i].temp[0];
            mutable_this->sendMes.motors[i].temp[1] = FB_Datas[i].temp[1];
            mutable_this->sendMes.motors[i].error = FB_Datas[i].error;
        }

        publisher_->publish(sendMes);
    }

    rclcpp::Subscription<devices_pkg::msg::XHandMsg>::SharedPtr subscription_;
    rclcpp::Publisher<devices_pkg::msg::XHandMsg>::SharedPtr publisher_;
    rclcpp::Publisher<devices_pkg::msg::XHandSensorMsg>::SharedPtr sensor_publisher_;
    rclcpp::TimerBase::SharedPtr timer_Sensor;
    devices_pkg::msg::XHandMsg sendMes;
};

#endif  // X_HAND_NODE_H_
