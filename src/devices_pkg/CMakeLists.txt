cmake_minimum_required(VERSION 3.8)
project(devices_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_subdirectory(src/sdk/)

add_executable(devices_node_publisher

src/out/Publisher.cpp
)

add_executable(devices_node_subscriber

src/out/Subscriber.cpp
)

set(SDK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/inc)

set(SDK_INCLUDE_DIRS
    ${SDK_ROOT}/
    ${SDK_ROOT}/HARDWARE_TOP
    ${SDK_ROOT}/Z_SYS_SRC/CAN
    ${SDK_ROOT}/Custom
    ${SDK_ROOT}/Z_SYS_SRC/SYS
    ${SDK_ROOT}/Z_SYS_SRC/UDP
    ${SDK_ROOT}/../third_party/lib/install-x86_64/include
    ${SDK_ROOT}/../third_party/lib/install-x86_64/include/yaml-cpp
)

target_include_directories(devices_node_publisher PRIVATE

    ${YAML_CPP_INCLUDE_DIR}
    ${SDK_INCLUDE_DIRS}
    ${SDK_ROOT}/Custom_Device/Motor_TaiHu
    ${SDK_ROOT}/Switch_Board

)

target_include_directories(devices_node_subscriber PRIVATE

    ${YAML_CPP_INCLUDE_DIR}
    ${SYS_INC_DIR}
    ${INC_DIR}/Custom_Device/Motor_TaiHu
    ${INC_DIR}/Switch_Board

)

#message("主项目源码目录 (CMAKE_CURRENT_SOURCE_DIR): ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/lib/x86_64/")
#/home/yq-z/code/SDK_2_ROS/src/devices_pkg/src/sdk/third_party/lib/install-x86_64/lib
set(SDK_LIB_ABS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/lib/x86_64)
set(HARDWARE_TOP_LIB ${SDK_LIB_ABS_DIR}/libHARDWARE_TOP.so)
set(SWITCH_BOARD_LIB ${SDK_LIB_ABS_DIR}/libSwitch_Board.so)
set(MOTOR_TAIHU_LIB ${SDK_LIB_ABS_DIR}/libMotor_Device_TaiHu.so)
set(SPDLOG_LIB ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/third_party/lib/install-x86_64/lib/libspdlog.so)


target_link_libraries(devices_node_publisher

    ${HARDWARE_TOP_LIB}
    ${SWITCH_BOARD_LIB}
    ${MOTOR_TAIHU_LIB}
    ${SPDLOG_LIB}
    pthread
)


target_link_libraries(devices_node_subscriber

    ${HARDWARE_TOP_LIB}
    ${SWITCH_BOARD_LIB}
    ${MOTOR_TAIHU_LIB}
    ${SPDLOG_LIB}
    pthread
)

# 链接 ROS 2 依赖
 ament_target_dependencies(devices_node_publisher rclcpp std_msgs)

 ament_target_dependencies(devices_node_subscriber rclcpp std_msgs)


# 安装可执行文件

install(TARGETS 

  devices_node_publisher
  devices_node_subscriber
  DESTINATION lib/${PROJECT_NAME}

)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
