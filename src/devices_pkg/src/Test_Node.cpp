#include "Test_Node.hpp"

template <typename MsgT>
Test_Node<MsgT>::Test_Node(const std::string& node_name)
: Node(node_name+"_test_node")
{
    curr_node_name = node_name;
    std::string topic_sub_name = node_name + "_subscriber";
    std::string topic_pub_name = node_name + "_publisher";
    
    

    publisher_ = this->create_publisher<MsgT>(topic_sub_name, 10);
    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(2),
        std::bind(cb_map[curr_node_name], this));
    subscription_ = this->create_subscription<MsgT>(topic_pub_name, 10, \
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
void Test_Node<MsgT>::e_bot_timer_callback()
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
void Test_Node<MsgT>::w_bot_timer_callback()
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
}
template <typename MsgT>
void Test_Node<MsgT>::x_hand_topic_callback(const typename MsgT::SharedPtr msg) const
{
    for (size_t i = 0; i < 28; i++)
    {
        RCLCPP_INFO(this->get_logger(), "Received motor %d  pos=%f, vel=%f, tor=%f, kp=%f, kd=%f",
                    i, msg->motors[i].pos, msg->motors[i].vel, msg->motors[i].tor, msg->motors[i].kp, msg->motors[i].kd);
    }
}
template <typename MsgT>
void Test_Node<MsgT>::e_bot_topic_callback(const typename MsgT::SharedPtr msg) const
{
    for (size_t i = 0; i < 28; i++)
    {
        RCLCPP_INFO(this->get_logger(), "Received motor %d  pos=%f, vel=%f, tor=%f, kp=%f, kd=%f",
                    i, msg->motors[i].pos, msg->motors[i].vel, msg->motors[i].tor, msg->motors[i].kp, msg->motors[i].kd);
    }
}
template <typename MsgT>
void Test_Node<MsgT>::w_bot_topic_callback(const typename MsgT::SharedPtr msg) const
{
    for (size_t i = 0; i < 28; i++)
    {
        RCLCPP_INFO(this->get_logger(), "Received motor %d  pos=%f, vel=%f, tor=%f, kp=%f, kd=%f",
                    i, msg->motors[i].pos, msg->motors[i].vel, msg->motors[i].tor, msg->motors[i].kp, msg->motors[i].kd);
    }
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    std::string string_arg = argv[1];

    rclcpp::executors::MultiThreadedExecutor executor;

    if(string_arg == "y_hand")
    {
        using MsgT = devices_pkg::msg::YHandMsg;
        const auto TestNode = std::make_shared<Test_Node<MsgT>>(string_arg);
        executor.add_node(TestNode);
        executor.spin();
    }
    else if(string_arg == "x_hand")
    {
        using MsgT = devices_pkg::msg::XHandMsg;
        const auto TestNode = std::make_shared<Test_Node<MsgT>>(string_arg);
        executor.add_node(TestNode);
        executor.spin();
    }
    // else if(string_arg == "E_Bot")
    // {
    //     using MsgT = devices_pkg::msg::EBotMsg;
    //     const auto TestNode = std::make_shared<Test_Node<MsgT>>(string_arg);
    // }
    // else if(string_arg == "W_Bot")
    // {
    //     using MsgT = devices_pkg::msg::WBotMsg;
    //     const auto TestNode = std::make_shared<Test_Node<MsgT>>(string_arg);
    // }
    else
    {
        rclcpp::shutdown();
        return -1;
    }
    

    // 关闭 ROS 2
    rclcpp::shutdown();
    return 0;
}