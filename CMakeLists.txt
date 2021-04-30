# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
cmake_minimum_required(VERSION 3.9)

# set the project name
project (AbaRender)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

# ---- Create library ----
add_library(core STATIC
	core/tgaimage.cpp
)

target_include_directories(core PUBLIC 
	${PROJECT_SOURCE_DIR}/core
)


# add the executable
add_executable (render "AbaRender.cpp")

target_link_libraries(render PRIVATE core)
