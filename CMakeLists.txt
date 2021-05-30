# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
cmake_minimum_required(VERSION 3.9)

# set the project name
project (AbaRender)

# Qt
list(APPEND CMAKE_PREFIX_PATH "C:/Qt/5.15.0/msvc2019_64")
#set(CMAKE_PREFIX_PATH "C:/Qt/5.15.0/msvc2019_64")
#list(APPEND CMAKE_INCLUDE_PATH "C:/Qt/5.15.0/msvc2019_64/bin")
#set(CMAKE_INCLUDE_PATH "C:/Qt/5.15.0/msvc2019_64/bin")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Qt
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# ---- Create library ----
add_library(core STATIC
	core/Shader.cpp
	core/tgaimage.cpp
	core/geometry.cpp
	core/model.cpp
	core/zbuffer.cpp
	core/CoordinateTranform.cpp
	core/FrameBuffer.cpp
    core/Camera.cpp
)

target_include_directories(core PUBLIC 
	${PROJECT_SOURCE_DIR}/core
)

# Qt GUI
set(HEADERS 
    "renderGui/RenderUi.h"
	"renderGui/RenderMainLoop.h"
	"renderGui/RenderMainLoop.cpp"
 )

set(SOURCES 
	"renderGui/RenderUi.cpp"
	"renderGui/RenderMainLoop.h"
	"renderGui/RenderMainLoop.cpp"
)

# add the executable
add_executable (${PROJECT_NAME} "main.cpp"  ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC
	#redner
	core
	#Qt lib
	Qt5::Widgets 
	Qt5::Core 
	Qt5::Gui	
)