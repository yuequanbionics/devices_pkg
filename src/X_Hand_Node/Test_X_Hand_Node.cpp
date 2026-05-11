#include "Test_X_Hand_Node.hpp"

template <typename MsgT>
Test_X_Hand_Node<MsgT>::Test_X_Hand_Node(const std::string& node_name)
    : Node(node_name) {

}

template <typename MsgT>
int Test_X_Hand_Node<MsgT>::create_objects(
    const std::string& pub_topic,
    const std::string& sub_topic) {
    publisher_ = this->create_publisher<MsgT>(pub_topic, 10);
    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(10),
        std::bind(&Test_X_Hand_Node<MsgT>::timer_callback, this));

    sensor_subscription_ = this->create_subscription<devices_pkg::msg::XHandSensorMsg>(
        "sensor_data", 10,
        std::bind(&Test_X_Hand_Node<MsgT>::x_hand_sensor_callback, this, std::placeholders::_1));

    subscription_ = this->create_subscription<MsgT>(sub_topic, 10,
                                                    std::bind(&Test_X_Hand_Node<MsgT>::x_hand_topic_callback, this, std::placeholders::_1));
    return 0;
}

template <typename MsgT>
void Test_X_Hand_Node<MsgT>::x_hand_timer_callback() {
    auto message = MsgT();
    publisher_->publish(message);
}

static int times = 0;
static float test = 0.5;

template <typename MsgT>
void Test_X_Hand_Node<MsgT>::timer_callback() {
    auto message = MsgT();

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
}

template <typename MsgT>
void Test_X_Hand_Node<MsgT>::x_hand_sensor_callback(const devices_pkg::msg::XHandSensorMsg::SharedPtr msg) const {
    // 定义传感器ID和对应的名称映射
    static const std::map<uint8_t, std::string> sensor_names = {
        {1, "拇指(Thumb)"},
        {2, "食指(Index Finger)"},
        {3, "中指(Middle Finger)"},
        {4, "无名指(Ring Finger)"},
        {5, "小指(Little Finger)"},
        {6, "手掌中心(Palm Center)"}};

    // RCLCPP_INFO(this->get_logger(), "====== 接收到传感器数据 ======");

    // 遍历所有传感器（1-6，对应文档4中定义的sensor_ids）
    for (uint8_t sensor_id = 1; sensor_id <= 6; ++sensor_id) {
        // 获取传感器名称
        std::string sensor_name = "未知传感器";
        auto it = sensor_names.find(sensor_id);
        if (it != sensor_names.end()) {
            sensor_name = it->second;
        }

        // 获取该传感器的数据
        auto pressure_data = msg->pressure[sensor_id - 1];  // 注意：数组索引从0开始，传感器ID从1开始

        // 检查数据是否有效
        if (pressure_data.finger.empty()) {
            // RCLCPP_INFO(this->get_logger(), "%s (ID: %d): 无数据",
            //             sensor_name.c_str(), sensor_id);
        } else {
            // 创建数据字符串
            std::ostringstream data_stream;
            data_stream << sensor_name << " (ID: " << static_cast<int>(sensor_id) << "): [";

            for (size_t i = 0; i < pressure_data.finger.size(); ++i) {
                data_stream << pressure_data.finger[i];
                if (i < pressure_data.finger.size() - 1) {
                    data_stream << ", ";
                }
            }
            data_stream << "]";

            // 打印传感器数据
            // RCLCPP_INFO(this->get_logger(), "%s", data_stream.str().c_str());

            // // 可选：打印数据统计信息
            // if (!pressure_data.finger.empty()) {
            //     uint16_t min_val = *std::min_element(pressure_data.finger.begin(), pressure_data.finger.end());
            //     uint16_t max_val = *std::max_element(pressure_data.finger.begin(), pressure_data.finger.end());
            //     uint16_t avg_val = std::accumulate(pressure_data.finger.begin(), pressure_data.finger.end(), 0) / pressure_data.finger.size();

            //     RCLCPP_INFO(this->get_logger(), "      数据统计: 最小值=%u, 最大值=%u, 平均值=%u",
            //                 min_val, max_val, avg_val);
            // }
        }
    }

    // RCLCPP_INFO(this->get_logger(), "=============================\n");
}

template <typename MsgT>
void Test_X_Hand_Node<MsgT>::x_hand_topic_callback(const typename MsgT::SharedPtr msg) const {
    // for (size_t i = 0; i < 28; i++)
    // {
    //     RCLCPP_INFO(this->get_logger(), "Received motor %d  pos=%f, vel=%f, tor=%f, kp=%f, kd=%f",
    //                 i, msg->motors[i].pos, msg->motors[i].vel, msg->motors[i].tor, msg->motors[i].kp, msg->motors[i].kd);
    // }

    //  RCLCPP_INFO(this->get_logger(), " -------------------------------------------------");
}

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);

    if (argc < 4)
    {
        cout << "Invalid or incomplete parameters" << endl;
        return -1;
    }

    rclcpp::executors::MultiThreadedExecutor executor;

    using MsgT = devices_pkg::msg::XHandMsg;
    const auto TestNode = std::make_shared<Test_X_Hand_Node<MsgT>>(argv[1]);
    TestNode->create_objects(argv[2], argv[3]);
    executor.add_node(TestNode);
    executor.spin();

    // 关闭 ROS 2
    rclcpp::shutdown();
    return 0;
}