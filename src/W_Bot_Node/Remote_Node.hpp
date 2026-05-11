#include "rclcpp/rclcpp.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include "devices_pkg/msg/remote.hpp"

devices_pkg::msg::Remote re_msg;

// ROS2 UDP客户端：4Hz发送固定16进制 + 同步接收服务端回复 + 16进制显示
class Remote_Node : public rclcpp::Node
{
public:
    Remote_Node() : Node("Remote_Node")
    {
        // 1. 声明参数：服务端（目标）IP/端口、发送频率、预期回复字节数
        this->declare_parameter<std::string>("server_ip", "192.168.144.12"); // 服务端IP
        this->declare_parameter<int>("server_port", 19856);              // 服务端端口
        this->declare_parameter<double>("send_hz", 4.0);                // 发送频率4Hz
        this->declare_parameter<int>("expect_resp_len", 42);             // 预期服务端回复字节数（示例7字节）
        this->get_parameter("server_ip", server_ip_);
        this->get_parameter("server_port", server_port_);
        this->get_parameter("send_hz", send_hz_);
        this->get_parameter("expect_resp_len", expect_resp_len_);

        // 2. 初始化UDP套接字（客户端，系统自动分配本地端口）
        if (!init_udp_socket())
        {
            RCLCPP_FATAL(this->get_logger(), "UDP套接字初始化失败，节点退出！");
            rclcpp::shutdown();
            return;
        }

        // 3. 初始化服务端地址（发送目标）
        memset(&server_addr_, 0, sizeof(server_addr_));
        server_addr_.sin_family = AF_INET;
        server_addr_.sin_addr.s_addr = inet_addr(server_ip_.c_str());
        server_addr_.sin_port = htons(server_port_);



        // 5. 启动**独立接收线程**：接收服务端回复，不阻塞发送定时器
        recv_thread_ = std::thread(&Remote_Node::udp_recv_resp_loop, this);

        // 6. 创建ROS2定时器：4Hz定频发送（250ms周期）
        double period_ms = 1000.0 / send_hz_;
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds((int)period_ms),
            std::bind(&Remote_Node::udp_send_callback, this)
        );

        remote_pub_ = this->create_publisher<devices_pkg::msg::Remote>("/remote_data", 10);

        RCLCPP_INFO(this->get_logger(), "UDP 16进制客户端（收发双向）启动成功！");
        RCLCPP_INFO(this->get_logger(), "发送目标：%s:%d | 频率：%.1fHz | 发送字节数：%lu",
                    server_ip_.c_str(), server_port_, send_hz_, send_data_.size());
        RCLCPP_INFO(this->get_logger(), "预期服务端回复字节数：%d\n", expect_resp_len_);
        
    }

    ~Remote_Node()
    {
        close(sock_fd_);
        if (recv_thread_.joinable()) recv_thread_.join();
        RCLCPP_INFO(this->get_logger(), "UDP双向客户端已关闭");
    }

private:
    // 发送参数
    std::string server_ip_;
    int server_port_;
    double send_hz_;
    int expect_resp_len_; // 预期服务端回复字节数
    // ROS2相关
    rclcpp::TimerBase::SharedPtr timer_;
    // UDP相关
    int sock_fd_;
    struct sockaddr_in server_addr_;
    std::thread recv_thread_; // 独立接收线程（处理服务端回复）
    const int BUF_MAX_SIZE = 128;
    // 数据缓存
    std::vector<uint8_t> send_data_ = {0x55, 0x66, 0x01, 0x01, 0x00, 0x00, 0x00, 0x42, 0x02, 0xB5, 0xC0}; // 要发送的固定16进制数据
    using TimePoint = std::chrono::system_clock::time_point;


    rclcpp::Publisher<devices_pkg::msg::Remote>::SharedPtr remote_pub_;
    // 初始化UDP套接字（客户端无需绑定）
    bool init_udp_socket()
    {
        sock_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_fd_ < 0)
        {
            RCLCPP_ERROR(this->get_logger(), "创建套接字失败：%s", strerror(errno));
            return false;
        }
        return true;
    }

    // 定时器回调：4Hz定频发送数据
    void udp_send_callback()
    {
        ssize_t send_len = sendto(sock_fd_, send_data_.data(), send_data_.size(), 0,
                                  (struct sockaddr*)&server_addr_, sizeof(server_addr_));
        if (send_len < 0 || (size_t)send_len != send_data_.size())
        {
            RCLCPP_ERROR(this->get_logger(), "向%s:%d 发送失败！", server_ip_.c_str(), server_port_);
            return;
        }

    }
        void sbus_parse(uint8_t ch[])
    {
            // 从索引7开始，每次取两个字节（低位在前）
            for (int i = 8; i < 39; i += 2)
            {
                // 提取两个字节：ch[i] 是低字节，ch[i+1] 是高字节
                double value = (static_cast<uint16_t>(((ch[i + 1]) << 8) | ch[i])-1500)/4.5;
                
                // 计算通道编号（从1开始）
                int channel = (i - 8) / 2;
                re_msg.ch[channel]=value;
                remote_pub_->publish(re_msg);
                RCLCPP_INFO(this->get_logger(), "【msg发布成功】话题：/remote_data");

            }
        }
    // 独立接收线程：持续接收服务端的回复数据，同步显示// 独立接收线程：持续接收服务端的回复数据，同步显示
    void udp_recv_resp_loop()
    {
        std::vector<uint8_t> recv_buf(BUF_MAX_SIZE); // 使用 vector 替代 VLA
        struct sockaddr_in resp_addr; // 回复端（服务端）地址
        socklen_t resp_addr_len = sizeof(resp_addr);

        while (rclcpp::ok())
        {
            // 阻塞接收服务端回复（收到即处理，和发送4Hz同步）
            ssize_t recv_len = recvfrom(sock_fd_, recv_buf.data(), BUF_MAX_SIZE, 0,
                                        (struct sockaddr*)&resp_addr, &resp_addr_len);
            if (recv_len <= 0) { continue; }

            // 解析服务端信息
            std::string server_ip = inet_ntoa(resp_addr.sin_addr);
            // uint16_t server_port = ntohs(resp_addr.sin_port);
            std::string ts = get_millisecond_timestamp();

            // 回复数据长度校验
            if ((size_t)recv_len != (size_t)expect_resp_len_)
            {
                // RCLCPP_WARN(this->get_logger(), "[%s] 服务端%s:%d 回复不完整：收%ld字节，预期%d",
                //             ts.c_str(), server_ip.c_str(), server_port, recv_len, expect_resp_len_);
                continue;
            }

            sbus_parse(recv_buf.data()); // 注意：传递 vector 的 .data()
        }
    }

    // 工具：毫秒级时间戳
    std::string get_millisecond_timestamp()
    {
        TimePoint now = std::chrono::system_clock::now();
        std::time_t now_sec = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_sec);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "."
            << std::setw(3) << std::setfill('0') << ms.count();
        return oss.str();
    }

};