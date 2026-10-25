# 3rdparty 依赖路径定义（供根 CMakeLists.txt 和 utils/CMakeLists.txt 使用）
# 目录布局：
#   3rdparty/rknpu2/include                     rknn_api.h 等
#   3rdparty/rknpu2/Linux/aarch64/librknnrt.so
#   3rdparty/librga/include                     rga.h / im2d.h / drmrga.h 等
#   3rdparty/librga/Linux/aarch64/librga.so
#   3rdparty/jpeg_turbo/include                 turbojpeg.h
#   3rdparty/jpeg_turbo/Linux/aarch64/libturbojpeg.so
#   3rdparty/stb_image                          stb_image.h / stb_image_write.h
cmake_minimum_required(VERSION 3.10)

set(LIB_ARCH "aarch64")

# rknn runtime
set(RKNN_API_PATH ${CMAKE_CURRENT_SOURCE_DIR}/rknpu2)
set(RKNN_RT_LIB ${RKNN_API_PATH}/Linux/${LIB_ARCH}/librknnrt.so)
set(RKNN_RT_INCLUDES ${RKNN_API_PATH}/include)

# rga
set(LIBRGA ${CMAKE_CURRENT_SOURCE_DIR}/librga/Linux/${LIB_ARCH}/librga.so)
set(LIBRGA_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/librga/include)

# jpeg-turbo
set(LIBJPEG ${CMAKE_CURRENT_SOURCE_DIR}/jpeg_turbo/Linux/${LIB_ARCH}/libturbojpeg.so)
set(LIBJPEG_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/jpeg_turbo/include)

# stb_image（仅头文件）
set(STB_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
