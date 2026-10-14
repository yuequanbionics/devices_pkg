# X-Hand ROS 2 节点

`X_Hand_Node` 是 ROS 2 与 X-Hand 硬件之间的桥接节点：订阅电机指令，经 SDK 下发到 6 个电机，并发布电机反馈；另以 100 ms 定时器采集并发布 6 路触觉数据。

数据流：`x_hand_cmd` → `Send()` 下发 → 等待 2 ms → `Get_FB()` 读取 → `x_hand_data` 发布；触觉由独立定时器周期发布到 `sensor_data`。

## 1. 构建

环境要求：已安装 ROS 2 与 colcon，并已 source ROS 2 环境。

```bash
cd devices_pkg_ws
colcon build --packages-select devices_pkg
source install/setup.bash
```

CMake 通过 `add_device_node(X_Hand_Node)` 将 `X_Hand_Out.cpp` 与 `X_Hand_Node.cpp` 编译为同一可执行文件，并定义 `HAVE_ROS` 宏（使 SDK 侧的 `main` 变为 `hardware_init`，避免符号冲突）。构建时按 CPU 架构（x86_64 / aarch64）选择 SDK 库，并将 `sdk/config/YAML`、`sdk/script/python/Tool`、`sdk/bin` 安装到 `share/devices_pkg/`。

## 2. 运行

必须从 `devices_pkg_ws` 根目录启动：节点以相对路径 `install/devices_pkg/share/devices_pkg/YAML/X_Hand/out/<dev_type>/TOP.yaml` 读取配置。修改源码树中的 YAML 后需重新 `colcon build` 才会同步到安装目录。

```bash
# 手动配置：SN、PC/主控板 IP 写死在 xhand_launch.py 的 config_dev1 中，按实际修改
ros2 launch devices_pkg xhand_launch.py

# 自动配置：调用 Get_SN 工具读取设备 SN，按 SN 第 12 位奇偶分配左/右手
ros2 launch devices_pkg xhand_Auto.py
```

两种 launch 都会同时启动硬件节点和测试节点。测试节点以 10 ms 周期发送预设指令驱动硬件，运行前确认运动范围与安全条件。自动 launch 未检测到设备时只启动两个测试节点。

也可以直接运行硬件节点（需 5 个参数：节点名、发布话题、订阅话题、dev_type、dev_config）：

```bash
ros2 run devices_pkg X_Hand_Node \
    x_hand_node x_hand_data x_hand_cmd self_g_1.0 \
    "PC_IP: 192.168.3.245
SN: <your_SN>SG
Boards:
  - Id: 101
    IP: 192.168.3.106"
```

## 3. 话题接口与消息定义

| 话题 | 消息类型 | 方向 | 说明 |
| --------------- | ---------------------------------- | ---- | -------------------------------------------------------- |
| `x_hand_cmd` | `devices_pkg/msg/XHandMsg` | 订阅 | 电机指令，`motors[6]`，使用 `pos/vel/tor/kp/kd` |
| `x_hand_data` | `devices_pkg/msg/XHandMsg` | 发布 | 电机反馈，`motors[6]`，使用 `pos/vel/tor/temp/error` |
| `sensor_data` | `devices_pkg/msg/XHandSensorMsg` | 发布 | 触觉数据，`pressure[6]`，传感器离线时元素为空数组 |

同一 `XHandMsg` 类型复用于指令与反馈两个方向，字段含义随方向不同。自动 launch 下电机话题加 `_L`/`_R` 后缀；`sensor_data` 在源码中硬编码，不加后缀、不可通过参数修改（双手共用同一话题）。

```bash
ros2 topic echo /x_hand_data
ros2 topic echo /sensor_data
```

### 3.1 XHandMsg / Motor.msg

`XHandMsg.msg` 为 `devices_pkg/Motor[6] motors`；`types/Motor.msg` 字段：

| 字段      | 类型       | 指令侧                                       | 反馈侧              |
| --------- | ---------- | -------------------------------------------- | ------------------- |
| `pos`   | float32    | 期望位置，归一化 [0,1]（SDK 端超限自动截断） | 当前位置            |
| `vel`   | float32    | 期望速度                                     | 当前速度            |
| `tor`   | float32    | 前馈力矩                                     | 实际力矩            |
| `kp`    | float32    | 位置增益（示例值 500）                       | -                   |
| `kd`    | float32    | 速度增益（示例值 5）                         | -                   |
| `temp`  | float32[2] | -                                            | 电机温度            |
| `error` | uint16     | -                                            | 0=正常，非零=故障码 |

SDK 下发为 MIT-PD 控制，常见用法：纯位置控制设 `tor=0, vel=0` 及 `pos/kp/kd`；纯力矩控制设 `kp=0, kd=0` 及 `tor`。

### 3.2 XHandSensorMsg / Pressure.msg

`XHandSensorMsg.msg` 为 `devices_pkg/Pressure[6] pressure`；`types/Pressure.msg` 为 `uint16[] finger`，即触觉阵列原始 ADC 值，范围 [0, 4096)。

`pressure` 下标与部位：0=拇指、1=食指、2=中指、3=无名指、4=小指、5=手掌中心（对应传感器 ID 0x01–0x06，下标 = ID-1）。

### 3.3 消息字段与 C++ 变量对照

| msg 字段 | 指令侧 | 反馈侧 |
| ------------------------- | ----------------------- | -------------------------- |
| `motors[i].pos/vel/tor` | `Send_Datas[i].P/V/F` | `FB_Datas[i].P/V/F` |
| `motors[i].kp/kd` | `Send_Datas[i].KP/KD` | - |
| `motors[i].temp/error` | - | `FB_Datas[i].temp/error` |

## 4. 启动文件

### 4.1 xhand_launch.py（手动配置）

- `config_dev1` 内联 YAML 定义 `PC_IP` / `SN` / `Boards(Id, IP)`，按实际网络修改。
- launch 对所有节点统一传 5 个命令行参数（节点名、发布话题、订阅话题、dev_type、dev_config）；测试节点只使用前 3 个。
- `check_sn_suffix(config)` 按 SN 后缀推断 dev_type：

| 条件                      | dev_type                              |
| ------------------------- | ------------------------------------- |
| config 为`'None'`       | `mz_g_orin_fdcan_1.0`（FDCAN 路径） |
| SN 末尾非数字后缀为`SG` | `self_g_1.0`                        |
| 后缀为`MG`              | `mz_g_1.0`                          |
| 后缀为`M`               | `mz_1.0`                            |
| 其他（含未解析到 SN）     | `self_1.0`                          |

### 4.2 xhand_Auto.py（自动读取 SN）

适用于左右双手同时部署。流程：`Get_SN.DEC(pkg_path)` 读取并解码设备 SN 列表 → 只处理 `1X` 开头的 SN → 按 `sn[11]` 奇偶分配：

| SN 第 12 位 | 节点              | 主控板 IP     | 话题后缀 |
| ----------- | ----------------- | ------------- | -------- |
| 奇数        | `x_hand_node_L` | 192.168.3.106 | `_L`   |
| 偶数        | `x_hand_node_R` | 192.168.3.107 | `_R`   |

随后复用 `check_sn_suffix()` 推断 dev_type 并创建节点。

`Get_SN.py`（`sdk/script/python/Tool/`，构建后安装到 `share/devices_pkg/Tool/`）：

- 按架构选择原生可执行文件：优先 `bin/Tool/{x86|arm64}/Get_SN`，不存在时回退 `bin/Tool/ubuntu20.04/{x86|arm64}/Get_SN`，经 `subprocess` 执行获取原始数字 SN。
- 用模板/掩码（`SN_MOD`/`SN_MSK`）将数字 SN 解码为可读字符串（如 `1X1T2605061`）。
- 通过 `/tmp/dec_sn.lock` 排他文件锁阻塞等待，串行化并发调用。

## 5. 整体架构

| 层级         | 组件                                               | 职责                                                                     |
| ------------ | -------------------------------------------------- | ------------------------------------------------------------------------ |
| ROS 2 话题层 | `x_hand_cmd` / `x_hand_data` / `sensor_data` | 消息传输通道                                                             |
| 上层节点     | `Test_X_Hand_Node`                               | 10 ms 周期发送预设指令，订阅反馈与触觉                                   |
| 硬件节点     | `X_Hand_Node`                                    | 收指令 →`Send()` 下发 → `Get_FB()` 读反馈 → 发布；100 ms 采集触觉 |
| SDK 桥接层   | `X_Hand_Out.cpp`                                 | `hardware_init()` 初始化；`Send()` 下发 MIT-PD；`Get_FB()` 读反馈  |
| 物理硬件     | 主控板、电机 ×6、触觉传感器 ×6                   | UDP 通信                                                                 |

文件依赖关系：

| 源文件                 | 依赖目标                               | 方式                                                       |
| ---------------------- | -------------------------------------- | ---------------------------------------------------------- |
| `xhand_launch.py`    | `X_Hand_Node` / `Test_X_Hand_Node` | 启动并传命令行参数                                         |
| `xhand_Auto.py`      | `xhand_launch.py`、`Get_SN.py`     | import`check_sn_suffix()`、`DEC()`                     |
| `X_Hand_Node.hpp`    | `X_Hand_Out.cpp`                     | extern`hardware_init` / `Get_FB` / `Send` 及全局变量 |
| `XHandMsg.msg`       | `types/Motor.msg`                    | 引用                                                       |
| `XHandSensorMsg.msg` | `types/Pressure.msg`                 | 引用                                                       |

关键关联：

- `Send_Datas[6]` / `FB_Datas[6]` / `Tactile_Sensor_Control` 是节点与 SDK 之间的共享全局变量（节点写 `Send_Datas` → SDK 读；SDK 写 `FB_Datas` → 节点读），设计上假设单实例运行。
- 测试节点发布 `x_hand_cmd`、订阅 `x_hand_data`，与硬件节点构成闭环验证。

## 6. 节点实现要点

### 6.1 X_Hand_Node

- `main()` 要求 5 个参数（argv[1..5]）：节点名、发布话题、订阅话题、dev_type、dev_config；使用 `MultiThreadedExecutor`（电机回调与触觉定时器并发）。
- `create_objects()` 拼装 YAML 路径并调用 `hardware_init()`，随后创建反馈/触觉发布者、指令订阅者和 100 ms 触觉定时器。
- `topic_callback()`：`motors[i]` → `Send_Datas[i]` → `Send()` → `sleep(2ms)` → `Get_FB()` → `FB_Datas[i]` → 填充成员 `sendMes` 并发布（`const_cast` 用于在 const 回调中写成员）。
- `sensor_timer_callback()`：`Tactile_Sensor_Control == nullptr` 时打印一次告警后跳过；否则逐路调用 `Get_Stored_Sensor_Data()`（非阻塞读缓存）组装消息发布。

### 6.2 Test_X_Hand_Node

不接触硬件，`main()` 只使用前 3 个参数。10 ms 定时器发送预设指令：`motors[2..5].pos = 0.5 ± 0.5`（即在 0/1 间切换），拇指两个电机幅度较小（`motors[0]=0.25±0.25`，`motors[1]=0.1±0.1`），`kp=500, kd=5`；每 500 个周期（5 s）翻转一次方向。同时订阅 `sensor_data`，但日志打印代码已注释，终端不输出传感器值。

## 7. SDK 桥接（X_Hand_Out.cpp）

`hardware_init(ADDR, Config)`：

1. 创建 `Robot_Hardware` 并注册 5 类设备（Switch_Board / Switch_Board_Orin / Motor / Auto_Set_Id / Tactile_Sensor_Custom）；
2. `Init_TOP()` 解析 `TOP.yaml` + 内联 dev_config，构建设备拓扑；
3. 按名字获取 6 个电机及触觉设备句柄并转为控制类指针；
4. 触觉设备存在时 `Tactile_Sensor_Init()` 启动 40 ms 周期采集；
5. `X_hand_Init()` 执行机械零位标定，写入 `Pos_Offest`。

反馈归一化（`Get_FB()`）与指令反变换（`Send()`）：

```text
FB:  P = (P_raw - Pos_Offest) * Motor_Mirror / Motor_K
     V = V_raw * Motor_Mirror / Motor_K
     F = F_raw * Motor_Mirror
Send: 逐电机下发 MIT-PD，P 先做反变换 P*Mirror*K + Offset 并截断到 [0,1]，
     最后 Send_Buff_Data() 批量推送
```

| 系数                | 作用                                 |
| ------------------- | ------------------------------------ |
| `Pos_Offest[6]`   | 零位偏移，`X_hand_Init()` 标定写入 |
| `Motor_Mirror[6]` | 方向镜像（±1）                      |
| `Motor_K[6]`      | 量程换算（各电机不同）               |

dev_type 对应 `sdk/config/YAML/X_Hand/out/<dev_type>/` 目录；其中 `self_1.0`、`self_g_1.0`、`mz_1.0`、`mz_g_1.0` 的 `Devices.yaml` 配置了触觉传感器，两种 FDCAN 配置（`mz_g_fdcan_1.0`、`mz_g_orin_fdcan_1.0`）不含触觉设备。

## 8. 文件结构

工作空间 `devices_pkg_ws/src/devices_pkg/`：

```text
devices_pkg/
├── src/X_Hand_Node/
│   ├── X_Hand_Node.hpp/.cpp          # 硬件节点（main 在 .cpp）
│   ├── Test_X_Hand_Node.hpp/.cpp     # 示例测试节点
│   └── readme.md
├── sdk/
│   ├── src/Project/X_Hand/out/X_Hand_Out.cpp   # SDK 桥接（hardware_init/Send/Get_FB）
│   ├── inc/                            # SDK 头文件（HARDWARE_TOP / Hw_Pressure_Sensor 等）
│   ├── config/YAML/X_Hand/out/<dev_type>/      # 拓扑配置，安装到 share/devices_pkg/YAML/
│   ├── script/python/Tool/Get_SN.py            # SN 读取脚本，安装到 share/devices_pkg/Tool/
│   └── bin/Tool/{x86,arm64}/Get_SN             # SN 原生可执行文件（含 ubuntu20.04/ 回退目录），安装到 share/devices_pkg/bin/
├── msg/
│   ├── XHandMsg.msg                  # Motor[6] motors
│   ├── XHandSensorMsg.msg            # Pressure[6] pressure
│   └── types/Motor.msg、Pressure.msg
└── launch/
    ├── xhand_launch.py               # 手动 SN 配置启动
    └── xhand_Auto.py                 # 自动读取 SN，区分左/右手
```
