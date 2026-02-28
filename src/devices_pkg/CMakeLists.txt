cmake_minimum_required(VERSION 3.15)
project(devices_pkg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 关闭扩展，保证跨平台兼容性
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找ros2库
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)


# 统一架构判断，获取安装目录后缀
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  set(ARCH_SUFFIX "x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
  set(ARCH_SUFFIX "arm64")
else()
  message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()


# 设置SDK相关路径
set(SDK_INC ${CMAKE_CURRENT_SOURCE_DIR}/sdk/inc)

if(ARCH_SUFFIX MATCHES "x86_64")
  set(SDK_LIB_ABS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdk/lib/x86_64)
elseif (ARCH_SUFFIX MATCHES "arm64")
  set(SDK_LIB_ABS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdk/lib/arch64)
endif()

set(TH_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdk/third_party/lib/install-${ARCH_SUFFIX}/lib) 
set(SOK_PROJECT_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdk/Project)
file(GLOB SPDLOG_ALL_FILES "${TH_LIB_DIR}/libspdlog.so*")
file(GLOB YAML_ALL_FILES "${TH_LIB_DIR}/libyaml-cpp.so*")

# 汇总SDK头文件路径
set(SDK_INCLUDE_DIRS
        ${SDK_INC}/HARDWARE_TOP
        ${SDK_INC}/Z_SYS_SRC/CAN
        ${SDK_INC}/Motors/Motor
        ${SDK_INC}/Misc/Auto_Set_Id
        ${SDK_INC}/Uart_Pressure_Sensor
        ${SDK_INC}/Z_SYS_SRC/SYS
        ${SDK_INC}/Z_SYS_SRC/UDP
        ${SDK_INC}/../third_party/lib/install-${ARCH_SUFFIX}/include
        ${SDK_INC}/../third_party/lib/install-${ARCH_SUFFIX}/include/yaml-cpp
        ${SDK_INC}/Switch_Boards/Switch_Board
        ${SDK_INC}/Switch_Boards/Switch_Board/mcu/stm32h743
        ${SDK_INC}/Custom_Device/Servo_Feetech
        ${SDK_INC}/Custom_Device/Uart_Pressure_Sensor
        ${SDK_INC}/Custom_Device/Eyou_Motor
        ${SDK_INC}/Custom_Device/Motor_TaiHu
        ${SDK_INC}/Custom_Device/Motor_BenMo
        ${SDK_INC}/Custom_Device/IMU_YuanJi_Uart
        ${SDK_INC}/Misc/Led_Device
        ${CMAKE_INSTALL_PREFIX}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/E_Bot_Node
        ${CMAKE_CURRENT_SOURCE_DIR}/src/W_Bot_Node
        ${CMAKE_CURRENT_SOURCE_DIR}/src/X_Hand_Node
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Y_Hand_Node

)

# 汇总SDK链接库
set(SDK_ALL_LIBS
        ${SPDLOG_ALL_FILES}
        ${YAML_ALL_FILES}
        ${SDK_LIB_ABS_DIR}/libHARDWARE_TOP.so
        ${SDK_LIB_ABS_DIR}/libSwitch_Board.so
        ${SDK_LIB_ABS_DIR}/libAuto_Set_Id.so
        ${SDK_LIB_ABS_DIR}/libMotor_Device.so
        ${SDK_LIB_ABS_DIR}/libMotor_Interpolation.so
        ${SDK_LIB_ABS_DIR}/libMotor_Device_TaiHu.so
        ${SDK_LIB_ABS_DIR}/libServo_Feetech_SCS0009.so
        ${SDK_LIB_ABS_DIR}/libPressure_Sensor_Custom_Device.so
        ${SDK_LIB_ABS_DIR}/libLed_Device.so
        ${SDK_LIB_ABS_DIR}/libMotor_Device_BenMo.so
        ${SDK_LIB_ABS_DIR}/libEyou_Custom_Motor_Device.so
        ${SDK_LIB_ABS_DIR}/libIMU_Device_YuanJi.so
)

# 定义消息文件
set(MSG_FILES
        "msg/types/IMU.msg"
        "msg/types/Motor.msg"
        "msg/types/Pressure.msg"
        "msg/types/Pretighten.msg"
        "msg/types/LED.msg"
        "msg/EBotMsg.msg"
        "msg/WBotMsg.msg"
        "msg/XHandMsg.msg"
        "msg/YHandMsg.msg"
        "msg/Remote.msg"
        "msg/WBotLED.msg"
        
)

# 生成消息接口代码
rosidl_generate_interfaces(${PROJECT_NAME}
        ${MSG_FILES}
        DEPENDENCIES std_msgs
)

# 自动处理 setup.py, package.xml 和 launch 目录的路径问题
#ament_python_install_package(${PROJECT_NAME})

# 安装SDK库文件到lib目录
install(FILES
        ${SDK_ALL_LIBS}
        DESTINATION lib/
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

# 安装launch文件到share/<package_name>目录
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

# 解决「ROS2自动生成的消息接口代码未提前编译」导致的编译错误
rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")

set(TARGET_LIST "Y_Hand_Node;X_Hand_Node;E_Bot_Node;W_Bot_Node")

function(add_device_node TARGET_NAME)

  # 生成对应的输出文件名
  string(REPLACE "_Node" "" PROJECT_FILES_NAME ${TARGET_NAME})
  string(REPLACE "_Node" "_Out" PROJECT_OUT_NAME ${TARGET_NAME})
  # string(FIND ${TARGET_NAME} "Test_" IS_TEST_PROJECT)

  # if(NOT ${IS_TEST_PROJECT} EQUAL -1)
  #   set(SOURCES_FILES "src/Test_Node.cpp")
  # else()
  #   set(SOURCES_FILES
  #   "sdk/src/Project/${PROJECT_FILES_NAME}/out/${PROJECT_OUT_NAME}.cpp"
  #   "src/${TARGET_NAME}/${TARGET_NAME}.cpp"
  #  )   
  # endif()

  if(${TARGET_NAME} IN_LIST TARGET_LIST)
  set(SOURCES_FILES
    "sdk/src/Project/${PROJECT_FILES_NAME}/out/${PROJECT_OUT_NAME}.cpp"
    "src/${TARGET_NAME}/${TARGET_NAME}.cpp"
  )   
endif()


  if(${TARGET_NAME} STREQUAL "Remote_Node")
  set(SOURCES_FILES
  "src/W_Bot_Node/${TARGET_NAME}.cpp"
 )   
  endif()

  # 添加可执行文件
  add_executable(${TARGET_NAME}
  ${SOURCES_FILES}
  )

 
  # 添加Debug编译定义
  target_compile_definitions(${TARGET_NAME} PRIVATE
          HAVE_ROS
  )

  # 添加公共头文件路径
  target_include_directories(${TARGET_NAME} PRIVATE ${SDK_INCLUDE_DIRS})

  # 链接 ROS2 依赖
  ament_target_dependencies(${TARGET_NAME} rclcpp std_msgs)

  # 确保消息接口先编译完成
  add_dependencies(${TARGET_NAME} ${cpp_typesupport_target})

  # 链接所有依赖库
  target_link_libraries(${TARGET_NAME}

          -Wl,--no-as-needed
          ${SDK_ALL_LIBS}
          ${cpp_typesupport_target}
          pthread
          -Wl,--as-needed
  )

  # 安装可执行文件到lib/<package_name>目录
  install(TARGETS ${TARGET_NAME}
          RUNTIME DESTINATION lib/${PROJECT_NAME}
  )
endfunction()

# 添加设备节点
#add_device_node(X_Hand_Node)
add_device_node(Y_Hand_Node)
add_device_node(E_Bot_Node)
add_device_node(W_Bot_Node)
#add_device_node(Test_Node)
add_device_node(Remote_Node)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
