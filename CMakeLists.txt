cmake_minimum_required(VERSION 3.20)
project(StockPredictor)

set(CMAKE_CXX_STANDARD 17)

# 1. Setup Include Directories
include_directories(include)
include_directories(include/imgui)
include_directories(include/implot)

# 2. Find OpenGL (Usually pre-installed on Windows)
find_package(OpenGL REQUIRED)

# 3. Setup GLFW (Using FetchContent is easiest for CLion)
include(FetchContent)
FetchContent_Declare(
        glfw
        URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(glfw)

# 4. Define Source Files
set(SOURCES
        src/main.cpp
        src/predictor.cpp
        src/data_loader.cpp

        # ImGui Sources
        include/imgui/imgui.cpp
        include/imgui/imgui_draw.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/imgui_widgets.cpp
        include/imgui/imgui_demo.cpp
        include/imgui/imgui_impl_glfw.cpp
        include/imgui/imgui_impl_opengl3.cpp

        # ImPlot Sources
        include/implot/implot.cpp
        include/implot/implot_items.cpp
        include/implot/implot_demo.cpp
)

add_executable(StockPredictor ${SOURCES})

# 5. Link Libraries
target_link_libraries(StockPredictor
        PRIVATE
        glfw
        OpenGL::GL
)