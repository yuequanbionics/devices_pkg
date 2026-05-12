#!/bin/bash
set -euo pipefail

# ====================== 配置 ======================
# 工作空间名称（ROS2 Jazzy 工作空间）
WORKSPACE_NAME="devices_pkg_ws"
# ==================================================

# 1. 获取脚本自身所在目录（绝对路径）
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
# 2. 获取脚本所在目录的 **上一级目录**（工作空间将创建在这里）
PARENT_DIR=$(dirname "${SCRIPT_DIR}")
# 3. 工作空间完整路径
WORKSPACE_PATH="${PARENT_DIR}/${WORKSPACE_NAME}"
# 4. 工作空间 src 目录路径
SRC_PATH="${WORKSPACE_PATH}/src"

echo "================================================"
echo "📌 脚本所在目录：${SCRIPT_DIR}"
echo "📌 工作空间将创建在：${PARENT_DIR}"
echo "📌 工作空间路径：${WORKSPACE_PATH}"
echo "📌 目标 src 目录：${SRC_PATH}"
echo "================================================"

# 5. 创建工作空间 & src 目录
echo -e "\n🚀 正在创建 ROS2 Jazzy 工作空间..."
mkdir -p "${SRC_PATH}"

# 6. 复制【脚本所在的整个文件夹】到 src 下
echo -e "\n📂 复制脚本所在文件夹到工作空间 src 目录..."
cp -r "${SCRIPT_DIR}" "${SRC_PATH}/"

# 完成提示
echo -e "\n✅ ROS2 Jazzy 工作空间创建完成！"
echo "📍 工作空间：${WORKSPACE_PATH}"
echo "📍 源码路径：${SRC_PATH}/$(basename "${SCRIPT_DIR}")"
echo -e "\n🎉 操作成功！"
