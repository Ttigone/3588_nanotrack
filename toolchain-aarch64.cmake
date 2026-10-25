set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# 交叉编译器路径
if(NOT DEFINED ENV{CROSS_COMPILE} OR "$ENV{CROSS_COMPILE}" STREQUAL "")
    message(FATAL_ERROR
        "未设置环境变量 CROSS_COMPILE。\n"
        "请先在终端执行，例如:\n"
        "  export CROSS_COMPILE=$SDK/prebuilts/gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-\n"
        "  cmake -B build -DCMAKE_TOOLCHAIN_FILE=toolchain-aarch64.cmake")
endif()

set(CROSS_COMPILE "$ENV{CROSS_COMPILE}")
set(CMAKE_C_COMPILER   "${CROSS_COMPILE}gcc")
set(CMAKE_CXX_COMPILER "${CROSS_COMPILE}g++")

# 依赖库已放在项目 3rdparty 内，一般不需要 sysroot。
# 如果确实要从 SDK rootfs 里取头文件/库，取消注释并改成实际路径：
# set(CMAKE_SYSROOT /home/alientek/Desktop/atk_dlrk3588_linux6.1/buildroot/output/alientek_rk3588/host/aarch64-buildroot-linux-gnu/sysroot)

# 交叉编译时显式绕过 FindThreads 模块的探测盲区
set(THREADS_HAVE_PTHREAD_ARG TRUE CACHE BOOL "Have pthread arg" FORCE)
set(CMAKE_USE_PTHREADS_INIT 1 CACHE BOOL "Use pthreads" FORCE)
set(CMAKE_THREAD_LIBS_INIT "-pthread" CACHE STRING "Thread libraries" FORCE)

# 只从目标系统查找头文件和库，避免链到主机的 x86 版本
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
