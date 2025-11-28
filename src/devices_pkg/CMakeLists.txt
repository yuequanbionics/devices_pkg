cmake_minimum_required(VERSION 3.8)
project(devices_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(devices_node_publisher

src/out/Publisher.cpp

)

add_executable(devices_node_subscriber

src/out/Subscriber.cpp

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

add_subdirectory(src/sdk/)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
