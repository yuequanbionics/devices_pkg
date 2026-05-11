//
// Created by lwz on 2026/3/12.
//

#ifndef DEVICES_PKG_TEST_IO_BOARD_H
#define DEVICES_PKG_TEST_IO_BOARD_H

#include "rclcpp/rclcpp.hpp"
#include "HARDWARE_TOP.hpp"
#include "devices_pkg/msg/io_board_msg.hpp"

// 声明外部变量
extern u8 R_IO[32];

//声明外部函数
int hardware_init(const string& ADDR, const string& Config);
void ROS_Get_GPIOx_Status();


class IO_Board_Node : public rclcpp::Node
{
public:
    IO_Board_Node(
        const std::string& node_name,
        const std::string& pub_topic,
        const std::string& sub_topic,
        const std::string& dev_config

        ) : Node(node_name)
    {
        hardware_init("src/devices_pkg/sdk/config/YAML/IO_Board/out/TOP.yaml", dev_config);
        publisher_ = this->create_publisher<devices_pkg::msg::IOBoardMsg>(pub_topic, 10);
        subscription_ = this->create_subscription<devices_pkg::msg::IOBoardMsg>(sub_topic, 10, \
            std::bind(&IO_Board_Node::topic_callback, this, std::placeholders::_1));

        timer_io_board = this->create_wall_timer(std::chrono::microseconds(20),
            std::bind(&IO_Board_Node::io_board_timer_callback, this));
    }


private:
    void topic_callback(const devices_pkg::msg::IOBoardMsg::SharedPtr msg) const
    {
        &msg;
    }

    void io_board_timer_callback()
    {
        auto io_data_msg = devices_pkg::msg::IOBoardMsg();

        ROS_Get_GPIOx_Status();

        for (uint8_t i = 1; i <= 6; i++)
            io_data_msg.io_status[i] = R_IO[i];

        publisher_->publish(io_data_msg);
    }

    rclcpp::TimerBase::SharedPtr timer_io_board;

    rclcpp::Subscription<devices_pkg::msg::IOBoardMsg>::SharedPtr subscription_;
    rclcpp::Publisher<devices_pkg::msg::IOBoardMsg>::SharedPtr publisher_;
    devices_pkg::msg::IOBoardMsg sendMes;


};


#endif //DEVICES_PKG_TEST_IO_BOARD_H