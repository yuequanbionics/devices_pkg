#include "E_Bot_Node.hpp"
#include <filesystem>
#include <iostream>

using namespace std;

int hardware_init(string ADDR);

int main(int argc, char *argv[])

{

    UDP_Def UDP_Def_one;
    Init_One_UDP_Connect(&UDP_Def_one, inet_addr("127.0.0.1"), htons(16201));

     Robot_Hardware *Test_Robot = new Robot_Hardware();
    Test_Robot->Add_Device_Type("Switch_Board", Switch_Board_Device_Init, Switch_Board_Device_CallBack_F, Switch_Board_Device_Delete_F);
    // Test_Robot->Add_Device_Type("TaiHu_Custom_Motor", Motor_Device_Init_TaiHu, Motor_Device_CallBack_TaiHu, Motor_Device_Delete_TaiHu);

    // filesystem::path exe_path = filesystem::canonical("/proc/self/exe");  
    // filesystem::path dir_path = exe_path.parent_path();
    // std::cout << "程序所在目录: " << dir_path << std::endl;

    // string ADDR = dir_path.string() + "/../config/YAML/W_Bot/zhang_RuiMing/Motor/TaiHu_TOP.yaml";
    // if(Test_Robot->Init_TOP(ADDR) != 0)
    // {
    //     cout << "Init_ERR" << endl;
    //     return -1;
    // }

    // const shared_ptr<Device_Struct> Main_Switch_Board = Test_Robot->Get_Device_For_Name("Main_Switch_Board");
    // Main_B *Main_Switch_Board_Control = static_cast<Main_B *>(Test_Robot->Get_Control_Class(Main_Switch_Board));

    // const shared_ptr<Device_Struct> Motor = Test_Robot->Get_Device_For_Name("TiaHu_Motor");
    // Motor_TaiHu *Motor_TaiHu_c = static_cast<Motor_TaiHu *>(Test_Robot->Get_Control_Class(Motor));

    rclcpp::init(argc, argv);

    hardware_init("../../../sdk/config//YAML/E_Bot/li_wenzhi/Servo_Config/TOP.yaml");

    // auto pub_node = std::make_shared<E_Bot_Publisher>(Test_Robot);
    // auto sub_node = std::make_shared<E_Bot_Subscriber>(Test_Robot);

    // // 创建多线程执行器
    // rclcpp::executors::MultiThreadedExecutor executor;
    // executor.add_node(pub_node);
    // executor.add_node(sub_node);
    // executor.spin();

    // rclcpp::shutdown();

    delete Test_Robot;

    return 0;
}