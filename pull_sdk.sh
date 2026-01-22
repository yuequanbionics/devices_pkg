#!/bin/bash

GITEE_REPO="git@gitee.com:beijing-daqi-yuequan-bionics/sdk_2.git"
GITHUB_REPO="https://github.com/yuequanbionics/YQ_SDK.git"

# 定义Gitee需要切换的分支
GITEE_TARGET_BRANCH="develop"

# 检查是否传入了平台参数
if [ $# -ne 1 ]; then
    echo "用法错误！正确用法：$0 [gitee|github]"
    echo "示例："
    echo "  $0 gitee    # 从Gitee拉取源代码"
    echo "  $0 github   # 从GitHub拉取输出代码"
    exit 1
fi

# 定义要拉取的目标平台
PLATFORM=$1

# 清理已存在的sdk目录（避免冲突）
if [ -d "sdk" ]; then
    echo "检测到已存在sdk目录，正在清理..."
    rm -rf sdk
    if [ $? -ne 0 ]; then
        echo "错误：清理旧的sdk目录失败，请检查权限！"
        exit 1
    fi
fi

# 根据平台选择对应的仓库地址并拉取代码
case $PLATFORM in
    gitee)
        echo "开始从Gitee拉取代码..."
        git clone $GITEE_REPO src/devices_pkg/sdk
        cd src/devices_pkg/sdk || exit 1
        git checkout $GITEE_TARGET_BRANCH
        if [ $? -ne 0 ]; then
                echo "警告：切换${GITEE_TARGET_BRANCH}分支失败，可能该分支不存在！"
                cd ../../..
                exit 1
            fi
            cd ../../..
        ;;
    github)
        echo "开始从GitHub拉取代码..."
        git clone $GITHUB_REPO src/devices_pkg/sdk
        ;;
    *)
        echo "错误：不支持的平台 '$PLATFORM'，仅支持 gitee 或 github！"
        exit 1
        ;;
esac

# 检查拉取是否成功
if [ $? -eq 0 ] && [ -d "src/devices_pkg/sdk" ]; then
    echo "✅ 代码拉取成功！已将代码保存到 ./src/devices_pkg/sdk 目录"
else
    echo "❌ 代码拉取失败！请检查仓库地址或网络连接"
    exit 1
fi