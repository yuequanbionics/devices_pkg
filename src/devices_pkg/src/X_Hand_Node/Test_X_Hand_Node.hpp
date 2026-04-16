#ifndef TEST_NODE_H_
#define TEST_NODE_H_

#include <filesystem>
#include <iostream>
#include <string>

#include "Custom_TOP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Switch_Board.hpp"
#include "UDP.hpp"
#include "devices_pkg/msg/x_hand_msg.hpp"
#include "devices_pkg/msg/x_hand_sensor_msg.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std;

template <typename MsgT>

class Test_X_Hand_Node : public rclcpp::Node {
   public:
    Test_X_Hand_Node(
        const std::string& node_name);


    int create_objects(
        const std::string& pub_topic,
        const std::string& sub_topic);

    void x_hand_timer_callback();
    void timer_callback();
    void x_hand_topic_callback(const typename MsgT::SharedPtr msg) const;
    void x_hand_sensor_callback(const devices_pkg::msg::XHandSensorMsg::SharedPtr msg) const;

   private:
    typename rclcpp::Publisher<MsgT>::SharedPtr publisher_;
    typename rclcpp::Subscription<MsgT>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<devices_pkg::msg::XHandSensorMsg>::SharedPtr sensor_subscription_;
};

#endif  // TEST_NODE_H_