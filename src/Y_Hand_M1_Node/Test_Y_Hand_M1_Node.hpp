#ifndef TEST_NODE_H_
#define TEST_NODE_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "UDP.hpp"
#include "HARDWARE_TOP.hpp"
#include "Custom_TOP.hpp"
#include "Switch_Board.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include "devices_pkg/msg/y_hand_msg.hpp"


using namespace std;

template <typename MsgT>

class Test_Y_Hand_M1_Node : public rclcpp::Node
{
public:
    Test_Y_Hand_M1_Node(const std::string& node_name);

    int create_objects(
        const std::string& pub_topic,
        const std::string& sub_topic);
    void y_hand_timer_callback();
    void y_hand_topic_callback(const typename MsgT::SharedPtr msg) const;


private:
    
    typename rclcpp::Publisher<MsgT>::SharedPtr publisher_;
    typename rclcpp::Subscription<MsgT>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;


};


#endif // TEST_NODE_H_