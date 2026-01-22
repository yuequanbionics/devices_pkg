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

class Test_Node : public rclcpp::Node
{
public:
    Test_Node()
    : Node("test_node")
    {
        publisher_ = this->create_publisher<devices_pkg::msg::XHandMsg>("x_hand_subscriber", 10);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(2),
            std::bind(&Test_Node::timer_callback, this));

        subscription_ = this->create_subscription<devices_pkg::msg::XHandMsg>("x_hand_publisher", 10, \
            std::bind(&Test_Node::topic_callback, this, std::placeholders::_1));
 }
private:

    void timer_callback()
    {
        auto message = devices_pkg::msg::XHandMsg();

        if (times % 500 * 2 == 0) {
            test = -test;
        }
        times++;

        for (int i = 0; i < 6; i++) {
            message.motors[i].pos = 0.5 + test;
            message.motors[i].vel = 0;
            message.motors[i].tor = 0;
            message.motors[i].kp = 500;
            message.motors[i].kd = 5;
        }
        message.motors[0].pos = 0.25 + test / 2;
        message.motors[1].pos = 0.1 + test / 5;


        publisher_->publish(message);
        RCLCPP_INFO(this->get_logger(), "Published command message");
    
    }

     void topic_callback(const devices_pkg::msg::XHandMsg::SharedPtr msg) const
    {
        // for(size_t i = 0; i < 6; i++)
        // {
        //     RCLCPP_INFO(this->get_logger(), "Received motor %d  pos=%f, vel=%f, tor=%f, kp=%f, kd=%f", 
        //         i+1, msg->motors[i].pos, msg->motors[i].vel, msg->motors[i].tor, msg->motors[i].kp, msg->motors[i].kd);

        //     // for(size_t j = 0; j < msg->pressure[i].finger.size(); j++)
        //     // {
        //     //     RCLCPP_INFO(this->get_logger(), "Received  Pressure finger %d size %d data: %d", 
        //     //     i, j, msg->pressure[i].finger[j]);
        //     // }

        // }

    }
    rclcpp::Publisher<devices_pkg::msg::XHandMsg>::SharedPtr publisher_;
    rclcpp::Subscription<devices_pkg::msg::XHandMsg>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;

    //float loop_time_step = 1000 * 1000 / 500;
    int times = 0;
    float test = 0.5;
};

int main(int argc, char *argv[])

{

    rclcpp::init(argc, argv);

    const auto TestNode = std::make_shared<Test_Node>();

    // 创建多线程执行器
    rclcpp::executors::MultiThreadedExecutor executor;

    executor.add_node(TestNode);
    executor.spin();

    // 关闭 ROS 2
    rclcpp::shutdown();

    return 0;
}