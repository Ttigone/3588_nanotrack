cmake_minimum_required(VERSION 3.10)
project(nanotrack_cpp CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#################################################################
# 第三方库：全部指向项目内 3rdparty，不走系统查找，
# 避免交叉编译时 find_package 误链到主机 x86 的库。
# 均可用 -D<变量名>=<路径> 覆盖。
#################################################################
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/3rdparty" CACHE PATH "第三方库根目录")

# rknn runtime
set(RKNN_API_DIR "${THIRD_PARTY_DIR}/rknpu2"           CACHE PATH "rknn runtime 目录")
set(RKNN_LIB     "${RKNN_API_DIR}/Linux/aarch64/librknnrt.so")

# rga
set(RGA_DIR      "${THIRD_PARTY_DIR}/librga"           CACHE PATH "rga 目录")
set(RGA_LIB      "${RGA_DIR}/Linux/aarch64/librga.so")

# eigen（仅头文件）
set(EIGEN_DIR    "${THIRD_PARTY_DIR}/eigen"            CACHE PATH "Eigen 根目录（其下为 Eigen/）")

# opencv：支持三种常见预编译目录结构
#   1) <dir>/OpenCVConfig.cmake                        （cmake/opencv4 目录）
#   2) <dir>/lib/cmake/opencv4/OpenCVConfig.cmake      （sysroot/rootfs 安装布局）
#   3) <dir>/share/OpenCV/OpenCVConfig.cmake           （rknn model zoo 的 opencv-linux-aarch64）
set(OPENCV_ROOT  "${THIRD_PARTY_DIR}/opencv"           CACHE PATH "OpenCV 预编译根目录")

foreach(_cv_cfg "${OPENCV_ROOT}" "${OPENCV_ROOT}/lib/cmake/opencv4" "${OPENCV_ROOT}/share/OpenCV")
    if(EXISTS "${_cv_cfg}/OpenCVConfig.cmake")
        set(OpenCV_DIR "${_cv_cfg}" CACHE PATH "OpenCVConfig.cmake 所在目录" FORCE)
        break()
    endif()
endforeach()

foreach(_lib "${RKNN_LIB}" "${RGA_LIB}")
    if(NOT EXISTS "${_lib}")
        message(FATAL_ERROR "缺少依赖库: ${_lib}\n请把对应文件放入 3rdparty 后重试")
    endif()
endforeach()

if(NOT EXISTS "${EIGEN_DIR}/Eigen")
    message(FATAL_ERROR "缺少 Eigen: ${EIGEN_DIR}/Eigen 不存在")
endif()

find_package(OpenCV REQUIRED CONFIG PATHS "${OpenCV_DIR}" NO_DEFAULT_PATH)

find_package(Threads REQUIRED)

#################################################################
# 可执行文件
#################################################################
add_executable(${PROJECT_NAME}
    demo_rknn.cpp
    nanotrack.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${RKNN_API_DIR}/include
    ${RGA_DIR}/include
    ${EIGEN_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${RKNN_LIB}
    ${RGA_LIB}
    ${OpenCV_LIBS}
    Threads::Threads
    dl
)

# 运行时优先在当前目录的 lib/ 下找动态库
set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")
