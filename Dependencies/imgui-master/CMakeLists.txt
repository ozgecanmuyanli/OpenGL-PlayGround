add_library(IMGUI
    STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_impl_opengl3.cpp
    imgui_impl_glfw.cpp
)

target_include_directories(IMGUI
    PUBLIC
    "${PROJECT_SOURCE_DIR}/Dependencies/imgui-master"
    "${PROJECT_SOURCE_DIR}/Dependencies/glfw-3.3.2/include"
    "${PROJECT_SOURCE_DIR}/Dependencies"
)
