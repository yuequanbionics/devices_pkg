#include "Test_Node.hpp"

template <typename MsgT>
Test_Node<MsgT>::Test_Node (
        const std::string& node_name,
        const std::string& pub_topic,
        const std::string& sub_topic)
: Node(node_name)
{
    curr_node_name = node_name;
    std::string topic_sub_name = sub_topic;
    std::string topic_pub_name = pub_topic;
    
    //RCLCPP_INFO(this->get_logger(), "sub %s ,pub %s", topic_sub_name.c_str(), topic_pub_name.c_str());

    publisher_ = this->create_publisher<MsgT>(topic_pub_name, 10);

    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(10),
        std::bind(cb_map[curr_node_name], this));
    subscription_ = this->create_subscription<MsgT>(topic_sub_name, 10, \
        std::bind(topic_cb_map[curr_node_name], this, std::placeholders::_1));
}

template <typename MsgT>
void Test_Node<MsgT>::y_hand_timer_callback()
{
    auto message = MsgT();

    for(int i = 0; i < 28; i++)
    {
        message.motors[i].pos = 0.0;
        message.motors[i].vel = 0.0;
        message.motors[i].tor = -0.03;
        message.motors[i].kp = 0.0;
        message.motors[i].kd = 0.0;
    }
    publisher_->publish(message);
}

template <typename MsgT>
void Test_Node<MsgT>::x_hand_timer_callback()
{
    auto message = MsgT();

    for(int i = 0; i < 28; i++)
    {
        message.motors[i].pos = 0.0;
        message.motors[i].vel = 0.0;
        message.motors[i].tor = -0.03;
        message.motors[i].kp = 0.0;
        message.motors[i].kd = 0.0;
    }
    publisher_->publish(message);
}


template <typename MsgT>
void Test_Node<MsgT>::y_hand_topic_callback(const typename MsgT::SharedPtr msg) const
{
    for (size_t i = 0; i < 28; i++)
    {
        RCLCPP_INFO(this->get_logger(), "Received motor %d  pos=%f, vel=%f, tor=%f, kp=%f, kd=%f",
                    i, msg->motors[i].pos, msg->motors[i].vel, msg->motors[i].tor, msg->motors[i].kp, msg->motors[i].kd);
    }

     RCLCPP_INFO(this->get_logger(), " -------------------------------------------------");
}
template <typename MsgT>
void Test_Node<MsgT>::x_hand_topic_callback(const typename MsgT::SharedPtr msg) const
{
    // for (size_t i = 0; i < 28; i++)
    // {
    //     RCLCPP_INFO(this->get_logger(), "Received motor %d  pos=%f, vel=%f, tor=%f, kp=%f, kd=%f",
    //                 i, msg->motors[i].pos, msg->motors[i].vel, msg->motors[i].tor, msg->motors[i].kp, msg->motors[i].kd);
    // }
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    std::string string_arg1 = argv[1];
    std::string string_arg2 = argv[2];
    std::string string_arg3 = argv[3];
    // RCLCPP_INFO(rclcpp::get_logger("main"), "sssssssssssssssssssssssssssssstring :  %s", string_arg1.c_str());
    // RCLCPP_INFO(rclcpp::get_logger("main"), "sssssssssssssssssssssssssssssstring :  %s", string_arg2.c_str());
    // RCLCPP_INFO(rclcpp::get_logger("main"), "sssssssssssssssssssssssssssssstring :  %s", string_arg3.c_str());

    rclcpp::executors::MultiThreadedExecutor executor;

    if(string_arg1 == "test_yhand_node")
    {
        using MsgT = devices_pkg::msg::YHandMsg;
        const auto TestNode = std::make_shared<Test_Node<MsgT>>(argv[1],argv[2],argv[3]);
        executor.add_node(TestNode);
        executor.spin();
    }
    else if(string_arg1 == "test_xhand_node")
    {
        using MsgT = devices_pkg::msg::XHandMsg;
        const auto TestNode = std::make_shared<Test_Node<MsgT>>(argv[1],argv[2],argv[3]);
        executor.add_node(TestNode);
        executor.spin();
    }
    else
    {
        rclcpp::shutdown();
        return -1;
    }
    

    // 关闭 ROS 2
    rclcpp::shutdown();
    return 0;
}