#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "Motor_TaiHu.hpp"
//#include "syst.hpp"
#include "UDP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Custom_TOP.hpp"
//#include "Motor_TOP.hpp"
#include "Switch_Board.hpp"

#include <filesystem>
#include <iostream>

using namespace std;

class MinimalPublisher : public rclcpp::Node
{

public:
    MinimalPublisher()

        : Node("minimal_publisher")

    {

        publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);

        timer_ = this->create_wall_timer(

            std::chrono::milliseconds(500),

            std::bind(&MinimalPublisher::timer_callback, this));
    }

private:
    void timer_callback()

    {

        auto message = std_msgs::msg::String();

        message.data = "Hello, world! " + std::to_string(count_++);

        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());

        publisher_->publish(message);
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

    rclcpp::TimerBase::SharedPtr timer_;

    size_t count_ = 0;

};

int main(int argc, char *argv[])

{

    UDP_Def UDP_Def_one;
    Init_One_UDP_Connect(&UDP_Def_one, inet_addr("127.0.0.1"), htons(16201));

    Robot_Hardware *Test_Robot = new Robot_Hardware();
    Test_Robot->Add_Device_Type("Switch_Board", Switch_Board_Device_Init, Switch_Board_Device_CallBack_F, Switch_Board_Device_Delete_F);
    Test_Robot->Add_Device_Type("TaiHu_Custom_Motor", Motor_Device_Init_TaiHu, Motor_Device_CallBack_TaiHu, Motor_Device_Delete_TaiHu);

    filesystem::path exe_path = filesystem::canonical("/proc/self/exe");  
    filesystem::path dir_path = exe_path.parent_path();
    std::cout << "程序所在目录: " << dir_path << std::endl;

    string ADDR = dir_path.string() + "/../config/YAML/W_Bot/zhang_RuiMing/Motor/TaiHu_TOP.yaml";
    if(Test_Robot->Init_TOP(ADDR) != 0)
    {
        cout << "Init_ERR" << endl;
        return -1;
    }

    const shared_ptr<Device_Struct> Main_Switch_Board = Test_Robot->Get_Device_For_Name("Main_Switch_Board");
    Main_B *Main_Switch_Board_Control = static_cast<Main_B *>(Test_Robot->Get_Control_Class(Main_Switch_Board));

    const shared_ptr<Device_Struct> Motor = Test_Robot->Get_Device_For_Name("TiaHu_Motor");
    Motor_TaiHu *Motor_TaiHu_c = static_cast<Motor_TaiHu *>(Test_Robot->Get_Control_Class(Motor));

    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<MinimalPublisher>());

    rclcpp::shutdown();

    return 0;
}