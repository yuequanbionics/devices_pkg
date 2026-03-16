#include "Test_Y_Hand_Node.hpp"

template <typename MsgT>
Test_Y_Hand_Node<MsgT>::Test_Y_Hand_Node (
        const std::string& node_name,
        const std::string& pub_topic,
        const std::string& sub_topic)
: Node(node_name)
{

    publisher_ = this->create_publisher<MsgT>(pub_topic, 10);
    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(10),
        std::bind(&Test_Y_Hand_Node<MsgT>::y_hand_timer_callback, this));
    subscription_ = this->create_subscription<MsgT>(sub_topic, 10, \
        std::bind(&Test_Y_Hand_Node<MsgT>::y_hand_topic_callback, this, std::placeholders::_1));
}

template <typename MsgT>
void Test_Y_Hand_Node<MsgT>::y_hand_timer_callback()
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
void Test_Y_Hand_Node<MsgT>::y_hand_topic_callback(const typename MsgT::SharedPtr msg) const
{
    // for (size_t i = 0; i < 28; i++)
    // {
    //     RCLCPP_INFO(this->get_logger(), "Received motor %d  pos=%f, vel=%f, tor=%f, kp=%f, kd=%f",
    //                 i, msg->motors[i].pos, msg->motors[i].vel, msg->motors[i].tor, msg->motors[i].kp, msg->motors[i].kd);
    // }

    //  RCLCPP_INFO(this->get_logger(), " -------------------------------------------------");
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::executors::MultiThreadedExecutor executor;

    using MsgT = devices_pkg::msg::YHandMsg;
    const auto TestNode = std::make_shared<Test_Y_Hand_Node<MsgT>>(argv[1], argv[2], argv[3]);
    executor.add_node(TestNode);
    executor.spin();

    // 关闭 ROS 2
    rclcpp::shutdown();
    return 0;
}