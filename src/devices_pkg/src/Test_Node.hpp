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
#include "devices_pkg/msg/x_hand_msg.hpp"
#include "devices_pkg/msg/y_hand_msg.hpp"


using namespace std;

template <typename MsgT>

class Test_Node : public rclcpp::Node
{
public:
    Test_Node( 
        const std::string& node_name,
        const std::string& pub_topic,
        const std::string& sub_topic);

    void y_hand_timer_callback();
    void x_hand_timer_callback();

    void y_hand_topic_callback(const typename MsgT::SharedPtr msg) const;
    void x_hand_topic_callback(const typename MsgT::SharedPtr msg) const;


private:
    
    typename rclcpp::Publisher<MsgT>::SharedPtr publisher_;
    typename rclcpp::Subscription<MsgT>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;

    int times = 0;
    float test = 0.5;
    std::string curr_node_name;

    using TimerCallbackFunc = void (Test_Node<MsgT>::*)(); // 定义回调函数指针类型
    std::unordered_map<std::string, TimerCallbackFunc> cb_map = {
        {"test_yhand_node", &Test_Node<MsgT>::y_hand_timer_callback},
        {"test_xhand_node", &Test_Node<MsgT>::x_hand_timer_callback}};

    using TopicCallbackFunc = void (Test_Node<MsgT>::*)(const typename MsgT::SharedPtr) const;
    std::unordered_map<std::string, TopicCallbackFunc> topic_cb_map = {
        {"test_yhand_node", &Test_Node<MsgT>::y_hand_topic_callback},
        {"test_xhand_node", &Test_Node<MsgT>::x_hand_topic_callback}};
};


#endif // TEST_NODE_H_