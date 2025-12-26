cmake_minimum_required(VERSION 3.15)
project(devices_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

set(SDK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/inc)
set(SDK_LIB_ABS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/lib/x86_64)
set(SPDLOG_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/third_party/lib/install-x86_64/lib)
file(GLOB SPDLOG_ALL_FILES "${SPDLOG_LIB_DIR}/libspdlog.so*")

set(SDK_INCLUDE_DIRS
    ${SDK_ROOT}/
    ${SDK_ROOT}/HARDWARE_TOP
    ${SDK_ROOT}/Z_SYS_SRC/CAN
    ${SDK_ROOT}/Custom
    ${SDK_ROOT}/Z_SYS_SRC/SYS
    ${SDK_ROOT}/Z_SYS_SRC/UDP
    ${SDK_ROOT}/../third_party/lib/install-x86_64/include
    ${SDK_ROOT}/../third_party/lib/install-x86_64/include/yaml-cpp
    ${SDK_ROOT}/Switch_Board
    ${SDK_ROOT}/Switch_Board/mcu/stm32h743
    ${SDK_ROOT}/Custom_Device/Servo_Feetech
    ${CMAKE_INSTALL_PREFIX}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/project/E_Bot_Node
    ${CMAKE_CURRENT_SOURCE_DIR}/src/project/W_Bot_Node
    ${CMAKE_CURRENT_SOURCE_DIR}/src/project/X_Hand_Node
    ${CMAKE_CURRENT_SOURCE_DIR}/src/project/Y_Hand_Node

)

set(SDK_ALL_LIBS
  ${SPDLOG_ALL_FILES}
  ${SDK_LIB_ABS_DIR}/libHARDWARE_TOP.so
  ${SDK_LIB_ABS_DIR}/libSwitch_Board.so
  ${SDK_LIB_ABS_DIR}/libMotor_Device_TaiHu.so
  ${SDK_LIB_ABS_DIR}/libServo_Feetech_SCS0009.so
)

set(MSG_FILES
    "msg/types/IMU.msg"
    "msg/types/Motor.msg"
    "msg/types/Pressure.msg"
    "msg/EBotMsg.msg"
    "msg/WBotMsg.msg"
    "msg/XHandMsg.msg"
    "msg/YHandMsg.msg"
)

rosidl_generate_interfaces(${PROJECT_NAME}
    ${MSG_FILES}
    DEPENDENCIES std_msgs 
)

install(FILES
  ${SDK_ALL_LIBS}
  DESTINATION lib/
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")

function(add_device_node TARGET_NAME)

  add_executable(${TARGET_NAME}
      src/project/${TARGET_NAME}/${TARGET_NAME}.cpp
  )

   # 添加 Debug 编译定义
  target_compile_definitions(${TARGET_NAME} PRIVATE
        HAVE_ROS
  )

  # 添加公共头文件路径
  target_include_directories(${TARGET_NAME} PRIVATE ${SDK_INCLUDE_DIRS})
    
  # 链接 ROS2 依赖
  ament_target_dependencies(${TARGET_NAME} rclcpp std_msgs)

  # 解决「ROS2自动生成的消息接口代码未提前编译」导致的编译错误
  add_dependencies(${TARGET_NAME} ${cpp_typesupport_target})

  # 添加公共链接库
  target_link_libraries(${TARGET_NAME}
      -Wl,--no-as-needed
      ${SDK_ALL_LIBS}
      ${cpp_typesupport_target}
      pthread
      -Wl,--as-needed
  )

  # 安装目标
  install(TARGETS ${TARGET_NAME}
      RUNTIME DESTINATION lib/${PROJECT_NAME}
  )
endfunction()

# 添加设备节点
#add_device_node(X_Hand_Node)
#add_device_node(Y_Hand_Node)
add_device_node(E_Bot_Node)
#add_device_node(Y_Bot_Node)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
